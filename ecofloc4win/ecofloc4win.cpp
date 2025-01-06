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
#include <mutex>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "gpu.h"
#include "MonitoringData.h"

using namespace ftxui;

std::vector<MonitoringData> monitoringData = {};
std::condition_variable new_data_cv;
std::mutex data_mutex;

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
    string str;
    size_t size;
    str.resize(wide_string.length());
    wcstombs_s(&size, &str[0], str.size() + 1, wide_string.c_str(), wide_string.size());
    return str;
}

void readCommand(string);

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

unordered_map<string, pair<vector<Process>, bool>> comp = {{"CPU", {{}, false}}, {"GPU", {{}, false} }, {"SD", {{}, false }}, {"NIC", {{}, false }}};

int interval = 500;
	
auto CreateTableRows() -> std::vector<std::vector<std::string>> {
	std::vector<std::vector<std::string>> rows;
	std::lock_guard<std::mutex> lock(data_mutex);

	rows.emplace_back(std::vector<std::string>{"Application Name", "CPU", "GPU", "SD", "NIC"});
	for (const auto& data : monitoringData) {
		rows.emplace_back(std::vector<std::string>{
			data.getName(),
				std::to_string(data.cpuEnergy),
				std::to_string(data.gpuEnergy),
				std::to_string(data.sdEnergy),
				std::to_string(data.nicEnergy)
		});
	}

	return rows;
}

auto RenderTable(int scroll_position) -> Element {
	auto table_data = CreateTableRows();
	int terminal_height = GetTerminalHeight();
	int visible_rows = terminal_height - 8; // Adjust for input box and borders

	// Prepare rows for the visible portion
	std::vector<std::vector<std::string>> visible_table_data;
	visible_table_data.push_back(table_data[0]); // Header row
	for (int i = scroll_position + 1;
		i < std::min(scroll_position + visible_rows + 1, (int)table_data.size());
		++i) {
		visible_table_data.push_back(table_data[i]);
	}

	auto table = Table(visible_table_data);

	// Style the table
	table.SelectAll().Border(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).DecorateCells(center);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Border();
	table.SelectColumn(0).Decorate(flex);
	table.SelectColumns(0, -1).SeparatorVertical(LIGHT);
	auto content = table.SelectRows(1, -1);
	content.DecorateCellsAlternateRow(color(Color::Red), 3, 0);
	content.DecorateCellsAlternateRow(color(Color::RedLight), 3, 1);
	content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

	return table.Render() | flex;
}

int main()
{
    std::string input;
	Component input_box = Input(&input, "Type here");
    input_box |= CatchEvent([&](Event event) {
        if (event == Event::Return) {
            if (!input.empty()) {
                std::cout << "Command: " << input << std::endl;
                readCommand(input);
                input.clear();
            }
            return true;
        }
        return false;
        });

	auto screen = ScreenInteractive::Fullscreen();

	// State variables for scrolling
	int scroll_position = 0;

	// Component to handle input and update the scroll position
	auto component = Renderer(input_box, [&] {
		return vbox({
			RenderTable(scroll_position),
			separator(),
			hbox({
				text("Command: "), input_box->Render()
			}),
			}) | border;
		});

	component = CatchEvent(component, [&](Event event) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8;

		if ((int)monitoringData.size() <= visible_rows) {
			scroll_position = 0; // Disable scrolling if all rows fit
			return false;
		}

		// Handle mouse wheel and arrow key events
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::WheelDown) {
				scroll_position = std::min(scroll_position + 1, (int)monitoringData.size() - visible_rows - 1);
				return true;
			}
			if (event.mouse().button == Mouse::WheelUp) {
				scroll_position = std::max(scroll_position - 1, 0);
				return true;
			}
		}

		if (event == Event::ArrowDown) {
			scroll_position = std::min(scroll_position + 1, (int)monitoringData.size() - visible_rows - 1);
			return true;
		}
		if (event == Event::ArrowUp) {
			scroll_position = std::max(scroll_position - 1, 0);
			return true;
		}

		return false;
		});
	
	std::thread gpu_thread([&screen] {
		while (true) {
            {
			    std::unique_lock<std::mutex> lock(data_mutex);
                new_data_cv.wait(lock, [] { return !monitoringData.empty(); });
			    for (auto& data : monitoringData)
			    {
				    int gpu_joules = GPU::getGPUJoules(data.getPids(), 500);
				    data.updateGPUEnergy(gpu_joules);
			    }
            }

			screen.Post(Event::Custom);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	std::thread redraw_thread([&screen] {
		while (true) {
			screen.PostEvent(Event::Custom);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	// Run the application
	screen.Loop(component);

	redraw_thread.join();
	gpu_thread.join();
	return 0;
}

void readCommand(string commandHandle)
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
            // to do
            break;

        default:
            cout << "error first argument (list command: add/remove/enable/disable/interval/start/quit)" << endl;
            break;
    }
}

void addProcPid(string pid, string component)
{
    // auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&pid](process o) {return o.getPid() == pid; });
    
    //if (it != comp[component].first.end())
    //{
    //    cout << "The process is already active" << endl;
    //}
    //else
    //{
        wstring processName = GetProcessNameByPID(stoi(pid));

        if (processName != L"<unknown>" /*&& check list*/)
        {
            {
                std::unique_lock<std::mutex> lock(data_mutex);
                MonitoringData data(wstring_to_string(processName), { stoi(pid) });
                monitoringData.push_back(data);
			    new_data_cv.notify_one();
            }
        }
        else
        {
            wcout << L"Failed to retrieve process name or process does not exist." << endl;
        }
    //}
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
                auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&ss](Process o) {return o.getPid() == ss.str(); });
                if (it >= comp[component].first.end())
                {
                    comp[component].first.push_back(Process(ss.str(), name));
                    cout << name << " (Pid: " << ss.str() << ") has been added" << endl;
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
}

void removeProcPid(string pid, string component)
{
    auto it = find_if(comp[component].first.begin(), comp[component].first.end(), [&pid](Process o) { return o.getPid() == pid; });

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