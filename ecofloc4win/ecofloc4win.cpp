// ecofloc4win.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
using namespace std;
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
    string Input;
    bool breaker = false;

    while (!breaker)
    {
        cout << ">";

        getline(cin, Input);

        //todo defineInterval function

        if (Input.size() > 0)
        {
            readCommand(Input, breaker);
        }
        else
        {
            cout << "error, need arguments" << endl;
        }
    }

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

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
