#define NOMINMAX

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>

#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <locale>
#include <codecvt>
#include <tlhelp32.h>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include "process.h"
#include <windows.h>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;

// Function to get terminal size
int GetTerminalHeight() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// Calculate the height of the terminal window.
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}
	// Default to 24 rows if size cannot be determined.
	return 24;
}

string wstring_to_string(const wstring& wide_string)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_string);
}

void readCommand(string, bool&);

void addProcPid(string, string);
void addProcName(string, string);

void removeProcPid(string, string);
void removeProcName(string, string);

void enable(string);
void disable(string);

unordered_map<string, int> actions = 
{
    {"enable", 1},
    {"disable", 2},
    {"add", 3},
    {"remove", 4},
    {"interval", 5},
    {"quit", 6},
};

//see to do remove all from component and watch processes in component

wstring GetProcessNameByPID(DWORD processID) {
    TCHAR processName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Check if we successfully got a handle to the process
    if (hProcess) {
        // Get the process name
        if (GetModuleBaseName(hProcess, nullptr, processName, sizeof(processName) / sizeof(TCHAR))) {
            CloseHandle(hProcess); // Close handle when done
            return processName;
        }
        CloseHandle(hProcess); // Close handle even if we fail
    }

    return L"<unknown>";
}

unordered_map<string, pair<vector<process>, bool>> comp = {{"CPU", {{}, false}}, {"GPU", {{}, false} }, {"SD", {{}, false }}, {"NIC", {{}, false }}};

int interval = 500;

int main()
{
	std::string input;
	auto input_box = Input(&input, "Type here");
	auto cell = [](const char* t) { return text(t) | border; };

	auto screen = ScreenInteractive::Fullscreen();

	std::vector<std::vector<std::string>> table_data = {
		{"Application Name", "CPU", "GPU", "SD", "NIC"},
		{"Firefox", "5120", "9852", "4563", "845"},
		{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
	};

	auto table = Table(table_data);

	// State variables for scrolling
	int scroll_position = 0;

	auto render_table = [&](int scroll_position) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8; // Adjust for input box and borders

		// Prepare table header
		std::vector<std::string> header = table_data[0];
		std::vector<std::vector<std::string>> rows;

		rows.push_back(header);
		// Prepare table rows
		for (int i = scroll_position + 1; i < std::min(scroll_position + visible_rows + 1, (int)table_data.size()); ++i) {
			rows.push_back(table_data[i]);
		}

		auto table = Table(rows);

		// Decorate the table
		table.SelectAll().Border(LIGHT);
		table.SelectRow(0).Decorate(bold);
		table.SelectRow(0).DecorateCells(center);
		table.SelectRow(0).SeparatorVertical(LIGHT);
		table.SelectRow(0).Border();
		table.SelectColumn(0).Decorate(flex);
		table.SelectColumns(0, -1).SeparatorVertical(LIGHT);
		auto content = table.SelectRows(1, -1);
		content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
		content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
		content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

		// Create the table with consistent column widths
		return table.Render() | flex;
		};

	// Component to handle input and update the scroll position
	auto component = Renderer(input_box, [&] {
		return vbox({
			render_table(scroll_position),
			separator(),
			hbox({
				text("Command: "),
				input_box->Render(),
				}),
			}) | border;
		});

	component = CatchEvent(component, [&](Event event) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8;

		// Disable scrolling if all rows fit within the visible area
		if ((int)table_data.size() <= visible_rows) {
			scroll_position = 0;
			return false;
		}

		// Handle mouse wheel events and arrow key events
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::WheelDown) {
				scroll_position = std::min(scroll_position + 1, (int)table_data.size() - visible_rows - 1);
				return true;
			}
			if (event.mouse().button == Mouse::WheelUp) {
				scroll_position = std::max(scroll_position - 1, 0);
				return true;
			}
		}

		if (event == Event::ArrowDown) {
			scroll_position = std::min(scroll_position + 1, (int)table_data.size() - visible_rows - 1);
			return true;
		}
		if (event == Event::ArrowUp) {
			scroll_position = std::max(scroll_position - 1, 0);
			return true;
		}

		return false;
		});

	// Run the application
	screen.Loop(component);
	return 0;
}

void readCommand(string commandHandle, bool& breaker)
{
    istringstream tokenStream(commandHandle);

    vector<string> chain;

    while (getline(tokenStream, commandHandle, ' ')) {
        chain.push_back(commandHandle);
    }

    switch (actions[chain[0]])  //see line 70
    {
        case 1: //to do
            if (chain.size() == 2)
            {
                enable(chain[1]);
            }
            else
            {
                cout << "error, need 2 in total for enable and disable" << endl;
            }
            break;

        case 2://to do
            if (chain.size() == 2)
            {
                disable(chain[1]);
            }
            else
            {
                cout << "error, need 2 in total for enable and disable" << endl;
            }
            break;

        case 3:
            if (chain.size() == 4)
            {
                if (chain[1] == "-p")
                {
                    if (all_of(chain[2].begin(), chain[2].end(), ::isdigit))
                    {
                        if (chain[3] == "CPU" || chain[3] == "GPU" || chain[3] == "SD" || chain[3] == "NIC")
                        {
                            addProcPid(chain[2], chain[3]);
                        }
                        else
                        {
                            cout << "error fourth argument (must be CPU, GPU, SD or NIC)" << endl;
                        }
                    }
                    else
                    {
                        cout << "error third argument (must be an integer)" << endl;
                    }
                }
                else if (chain[1] == "-n")
                {
                    if (chain[3] == "CPU" || chain[3] == "GPU" || chain[3] == "SD" || chain[3] == "NIC")
                    {
                        addProcName(chain[2], chain[3]);
                    }
                    else
                    {
                        cout << "error fourth argument (must be CPU, GPU, SD or NIC)" << endl;
                    }
                }
                else
                {
                    cout << "error second argument (-p for pid / -n for name)" << endl;
                }
            }
            else
            {
                cout << "error, need 4 in total for add and remove" << endl;
            }
            break;

        case 4:
            if (chain.size() == 4)
            {
                if (chain[1] == "-p")
                {
                    if (all_of(chain[2].begin(), chain[2].end(), ::isdigit))
                    {
                        if (chain[3] == "CPU" || chain[3] == "GPU" || chain[3] == "SD" || chain[3] == "NIC")
                        {
                            removeProcPid(chain[2], chain[3]); //to do: check if component
                        }
                        else
                        {
                            cout << "error fourth argument (must be CPU, GPU, SD or NIC)" << endl;
                        }
                    }
                    else
                    {
                        cout << "error third argument (must be an integer)" << endl;
                    }
                }
                else if (chain[1] == "-n")
                {
                    if (chain[3] == "CPU" || chain[3] == "GPU" || chain[3] == "SD" || chain[3] == "NIC")
                    {
                        removeProcName(chain[2], chain[3]); //to do: check if component
                    }
                    else
                    {
                        cout << "error fourth argument (must be CPU, GPU, SD or NIC)" << endl;
                    }
                }
                else
                {
                    cout << "error second argument (-p for pid / -n for name)" << endl;
                }
            }
            else
            {
                cout << "error, need 4 in total for add and remove" << endl;
            }
            break;

        case 5:
            if (chain.size() == 2)
            {
                if (all_of(chain[1].begin(), chain[1].end(), ::isdigit))
                {
                    interval = stoi(chain[1]);
                    cout << "Interval has been changed" << endl;
                }
                else
                {
                    cout << "error second argument (must be an integer)" << endl;
                }
            }
            else
            {
                cout << "error, need 2 in total for interval" << endl;
            }
            break;

        case 6:
            breaker = true;
            break;

        default:
            cout << "error first argument (list command: add/remove/enable/disable/interval/start/quit)" << endl;
            break;
    }
}

void addProcPid(string pid, string component)
{
    auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&pid](process o) {return o.getPid() == pid; });
    
    if (it != comp[component].first.end())
    {
        cout << "The process is already active" << endl;
    }
    else
    {
        wstring processName = GetProcessNameByPID(stoi(pid));

        if (processName != L"<unknown>" /*&& check list*/)
        {
            comp[component].first.push_back(process(pid, wstring_to_string(processName)));

            //add to list

            wcout << L"Process Name: " << processName << " has been added" << endl;
        }
        else
        {
            wcout << L"Failed to retrieve process name or process does not exist." << endl;
        }
    }
    
    /*auto it = find_if(pids.begin(), pids.end(), pid);

    if (it != pids.end())
    {
        cout << "Addition of the process via the pid: " << pid << endl;
    }
    else
    {
        cout << "Invalid pid";
    }*/
    //to do add via pid
}

void addProcName(string name, string component)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    wstring wstr(name.begin(), name.end());

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, wstr.c_str()) == 0) { // Case-insensitive comparison
                std::stringstream ss;
                ss << pe32.th32ProcessID;
                auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&ss](process o) {return o.getPid() == ss.str(); });
                if (it >= comp[component].first.end())
                {
                    comp[component].first.push_back(process(ss.str(), name));
                    cout << name << " (Pid: " << ss.str() << ") has been added" << endl;
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
}

void removeProcPid(string pid, string component)
{
    auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&pid](process o) { return o.getPid() == pid; });

    if (it != comp[component].first.end())
    {
        comp[component].first.erase(it);

        cout << "The process has been removed" << endl;
    }
    else
    {
        cout << "No process has been found" << endl;
    }
}

void removeProcName(string name, string component)
{
    for (auto it = comp[component].first.begin(); it != comp[component].first.end();)
    {
        if (it->getName() == name)
        {
            cout << it->getName() << " (Pid: " << it->getPid() << ") has been removed" << endl;
            it = comp[component].first.erase(it);
        }
        else
        {
            ++it;
        }
    }
    //to do remove via name
}

void enable(string component)
{
    if (!comp[component].second)
    {
        comp[component].second = true;
        cout << component << " is now enable" << endl;
    }
    else
    {
        cout << component << " was already enable" << endl;
    }
    //to do enable component
}

void disable(string component)
{
    if (comp[component].second)
    {
        comp[component].second = false;
        cout << component << " is now disable" << endl;
    }
    else
    {
        cout << component << " was already disable" << endl;
    }
    //to do disable component
}