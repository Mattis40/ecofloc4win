#include "CPU.h"

typedef float* (*GetCPUVoltagesFunc)(int* size);
typedef float* (*GetCPUClocksFunc)(int* size);
typedef float* (*GetCPUCoresPowerFunc)(int* size);

namespace CPU {
    uint64_t FromFileTime(const FILETIME& ft) {
        ULARGE_INTEGER uli = { 0 };
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        return uli.QuadPart;
    }

    uint64_t getCPUTime() {
        FILETIME idleTime, kernelTime, userTime;
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            uint64_t cpuTime = FromFileTime(kernelTime) + FromFileTime(userTime) + FromFileTime(idleTime);
            //cpuTime /= 1000000000; // Convertir en s
            return cpuTime;
        }
        else {
            std::cerr << "Failed to get CPU Time : Error" << GetLastError() << std::endl;
            return -1;
        }
    }

    uint64_t getPidTime(DWORD pid) {
        // Open an handle for the specified process with the required permissions
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess == NULL) {
            std::cerr << "Failed to open process handle. Error : " << GetLastError() << std::endl;
            return -1;
        }

        FILETIME creationTime, exitTime, kernelTime, userTime;

        // Get the process times
        if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
            uint64_t kernel = FromFileTime(kernelTime);
            uint64_t user = FromFileTime(userTime);

            uint64_t totalTime = kernel + user;
            //totalTime /= 1000000000; // Convertir en s
            CloseHandle(hProcess);
            return totalTime;
        }
        else {
            std::cerr << "Failed to get process times. Error : " << GetLastError() << std::endl;
            CloseHandle(hProcess);
            return -1;
        }

    }


    bool getCurrentPower(double& power) {
        // Load the DLL
        HMODULE hModule = LoadLibrary(L"Wrapper.dll");
        if (!hModule) {
            std::cerr << "Failed to load Wrapper.dll. Error code: " << GetLastError() << std::endl;
            return 1;
        }

        // Get the address of the function getCPUCoresPower
        GetCPUCoresPowerFunc getCPUCoresPower = (GetCPUCoresPowerFunc)GetProcAddress(hModule, "getCPUCoresPower");
        if (!getCPUCoresPower) {
            std::cerr << "Failed to get function address for getCPUCoresPower. Error code: " << GetLastError() << std::endl;
            FreeLibrary(hModule);
            return 1;
        }

        int powerSize = 0;
		int cpuCount = 0;
        float* powerArray = getCPUCoresPower(&powerSize);

        if (powerArray) {
            for (int i = 0; i < powerSize; i++) {
                power += powerArray[i];
                cpuCount++;
            }
            delete[] powerArray;
        }
        else {
            std::cerr << "Failed to retrieve CPU power." << std::endl;
        }


        power = power / cpuCount;
        cpuCount = 0;

        // Free the memory allocated by the DLL
        FreeLibrary(hModule);

        return true;
    }
}