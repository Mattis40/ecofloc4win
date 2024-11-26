// ecofloc4win.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <sstream>
#include <list>

/*ypedef enum
{
    enable,
    disable,
    add,
    remove
}Command;*/

void startCPU(int, int, int);

void readCommand(string);

void addProc(int);
void addProc(string);

void removeProc(int);
void removeProc(string);

void enable(string);
void disable(string);

vector<int> pids = { 195, 215, 45103 };//see to put pids in string
vector<string> names = { "Firefox", "Valorant", "Ecofloc"};

int main()
{
    string Input;

    //getline();

    getline(cin, Input);

    //cout << Input << endl;

    readCommand(Input);
}

void startCPU(int pid, int time, int interval)
{

}

void readCommand(string commandHandle)
{
    //vector<string> tokens;
    //string token;
    istringstream tokenStream(commandHandle);

    vector<string> chain;

    while (getline(tokenStream, commandHandle, ' ')) {
        chain.push_back(commandHandle);
    }

    if (chain[0] == "enable")
    {
        enable(chain[1]);
    }
    else if (chain[0] == "disable")
    {
        disable(chain[1]);
    }
    else if (chain[0] == "add")
    {
        if (chain[1] == "-p")
        {
            addProc(stoi(chain[2]));
        }
        else if (chain[1] == "-n")
        {
            addProc(chain[2]);
        }
        else
        {
            cout << "error second argument (-p for pid / -n for name)" << endl;
        }
    }
    else if (chain[0] == "remove")
    {
        if (chain[1] == "-p")
        {
            removeProc(stoi(chain[2]));
        }
        else if (chain[1] == "-n")
        {
            removeProc(chain[2]);
        }
        else
        {
            cout << "error second argument (-p for pid / -n for name)" << endl;
        }
    }
    else
    {
        cout << "error first argument (list command: add/remove/enable/disable)" << endl;
    }
    
    //Find a solution with switch later
    /*Command myCommand = chain[0];

    switch ()
    {
    default:
        break;
    }*/
}

void addProc(int pid)
{
    auto it = find(pids.begin(), pids.end(), pid);

    if (it != pids.end())
    {
        cout << "Addition of the process via the pid: " << pid << endl;
    }
    else
    {
        cout << "Invalid pid";
    }
    //to do add via pid
}

void addProc(string name)
{
    auto it = find(names.begin(), names.end(), name);

    if (it != names.end())
    {
        cout << "Addition of the process via the name: " << name << endl;
    }
    else
    {
        cout << "Invalid name";
    }
    //to do add via name
}

void removeProc(int pid)
{
    auto it = find(pids.begin(), pids.end(), pid);

    if (it != pids.end())
    {
        cout << "The process was removed via the pid: " << pid << endl;
    }
    else
    {
        cout << "Invalid pid";
    }
    //to do remove via pid
}

void removeProc(string name)
{
    auto it = find(names.begin(), names.end(), name);

    if (it != names.end())
    {
        cout << "The process was removed via the name: " << name << endl;
    }
    else
    {
        cout << "Invalid name";
    }
    //to do remove via name
}

void enable(string comp)
{
    cout << comp << " is now enable" << endl;
    //to do enable component
}

void disable(string comp)
{
    cout << comp << " is now disable" << endl;
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
