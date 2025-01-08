// PIDRecup.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <random>
#include <algorithm>
#include <iterator>
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

std::string wstringToString(const WCHAR* wideString) {
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, nullptr, 0, nullptr, nullptr);
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &result[0], sizeNeeded, nullptr, nullptr);
    result.pop_back(); // Supprimer le caractère nul final
    return result;
}

struct ProcessInfo {
    std::string name;
    std::vector<std::pair<DWORD, bool>> pids; // PID et statut "checked"
    std::string categorie;
    std::string couleur;
};

std::string generateRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(20, 225);

    int r, g, b;
    do {
        r = dis(gen);
        g = dis(gen);
        b = dis(gen);
    } while (r == g && g == b); // Éviter les niveaux de gris où r == g == b

    char hexColor[8];
    snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", r, g, b);
    return std::string(hexColor);
}
std::string determineCategory(const std::string& processName) {
    // Liste des noms de navigateurs
    std::vector<std::string> browsers = {
        "chrome",
        "firefox",
        "msedge",
        "opera"
    };

    std::vector<std::string> offices = {
    "word",
    "soffice",
    "powerpoint",
    };

    // Vérifier si le nom du processus correspond à l'un des navigateurs
    for (const auto& browser : browsers) {
        std::regex pattern(browser, std::regex_constants::icase);
        if (std::regex_search(processName, pattern)) {
            return "Browser";
        }
    }
    // Vérifier si le nom du processus correspond à l'un des navigateurs
    for (const auto& browser : browsers) {
        std::regex pattern(browser, std::regex_constants::icase);
        if (std::regex_search(processName, pattern)) {
            return "Browser";
        }
    }

    return ""; // Retourne une catégorie vide par défaut
}
// Fonction pour récupérer tous les processus et leurs PID
std::vector<ProcessInfo> getProcesses() {
    std::vector<ProcessInfo> processes;

    // Prendre un instantané des processus
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            // Vérifier si le processus existe déjà dans la liste
            auto it = std::find_if(processes.begin(), processes.end(), [&pe32](const ProcessInfo& proc) {
                return proc.name == wstringToString(pe32.szExeFile);
                });

            if (it != processes.end()) {
                // Ajouter le PID à un processus existant
                it->pids.emplace_back(pe32.th32ProcessID, true);
            }
            else {
                // Ajouter un nouveau processus
                ProcessInfo info;
                info.name = wstringToString(pe32.szExeFile);
                info.pids.emplace_back(pe32.th32ProcessID, true);
                info.categorie = "Other";
                processes.push_back(info);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return processes;
}

std::vector<std::vector<std::string>> getDiffVector(const std::vector<std::string>& list1, const std::vector<std::string>& list2) {
    std::vector<std::string> diffTemp;
    std::vector<std::vector<std::string>> output;
    std::set_difference(list1.begin(), list1.end(), list2.begin(), list2.end(),std::inserter(diffTemp, diffTemp.begin()));
    
    for (int i = 0; i < diffTemp.size(); i++) {
        if (std::find(list2.begin(), list2.end(), diffTemp[i]) != list2.end()) {// Added
            output[0].push_back(diffTemp[i]);
        }
        else { // Removed
            output[1].push_back(diffTemp[i]);
        }
    }

    return output;
}

json removeElemFromJsonFromKeyValue(json input, const std::string key, const std::string value) {
    for (auto it = input.begin(); it != input.end(); ) {
        if (it->contains(key) && (*it)[key] == value) {
            it = input.erase(it); // Remove the entry and update iterator
        }
        else {
            ++it; // Move to the next entry
        }
    }
    return input;
}

// Fonction pour écrire les processus dans un fichier JSON
void writeProcessesToJsonExistingJson(const std::vector<ProcessInfo>& processes, const std::string& filename) {
    std::ifstream f(filename);
    json input = json::parse(f);
    json output;
    std::vector<std::string> listProcessNameNew;
    std::vector<std::string> listProcessNameOld;

    for (auto it = input.begin(); it != input.end(); ++it)
    {
        listProcessNameOld.push_back(it->at("name"));
        std::cout << *it << std::endl;
    }
    for (const auto& process : processes) {
        listProcessNameNew.push_back(process.name);
    }

    std::vector<std::vector<std::string>> diff = getDiffVector(listProcessNameOld, listProcessNameNew);
    for (const auto& process : processes) {
        for (int i = 0; i < diff[0].size(); i++) {
            //addElemProcessToJson();
        }
        for (int i = 0; i < diff[1].size(); i++) {
            removeElemFromJsonFromKeyValue(input,"name", diff[1][i]);
        }

    }
    
    if (listProcessNameOld.size() > listProcessNameNew.size()) {//Removes processes from the json

    }
    else {//Add processes to the json

    }
    
    for (const auto& process : processes) {
        json pidArray = json::array();
        for (const auto& pid : process.pids) {
            pidArray.push_back({ {"numeroPid", pid.first}, {"checked", pid.second} });
        }

        // Création d'un objet JSON avec l'ordre souhaité
        json processJson = json::object({
            {"name", process.name},
            {"pid", pidArray},
            {"categorie", determineCategory(process.name)},
            {"couleur", generateRandomColor()}
            });

        output.push_back(processJson);
    }
/*
    for (const auto& process : processes) {
        json pidArray = json::array();
        for (const auto& pid : process.pids) {
            pidArray.push_back({ {"numeroPid", pid.first}, {"checked", pid.second} });
        }

        // Création d'un objet JSON avec l'ordre souhaité
        json processJson = json::object({
            {"name", process.name},
            {"pid", pidArray},
            {"categorie", determineCategory(process.name)},
            {"couleur", generateRandomColor()}
            });

        output.push_back(processJson);
    }*/

    // Écrire dans le fichier
    std::ofstream file(filename);
    if (file.is_open()) {
        file << output.dump(4); // Beautifier avec une indentation de 4
        file.close();
    }
}

int main()
{
    // Récupérer les processus
    std::vector<ProcessInfo> processes = getProcesses();

    // Écrire dans un fichier JSON
    std::string filename = "processes.json";
    std::ifstream f(filename);
    json lol = json::parse(f);
    //writeProcessesToJsonExistingJson(processes, filename);
    std::cout << removeElemFromJsonFromKeyValue(lol,"name","Registry").dump(4) << std::endl;
    std::cout << "Fichier JSON généré : " << filename << std::endl;
    return 0;
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
