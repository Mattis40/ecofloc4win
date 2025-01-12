#include "CPU.h"

typedef float* (*GetCPUVoltagesFunc)(int* size);
typedef float* (*GetCPUClocksFunc)(int* size);
typedef float* (*GetCPUCoresPowerFunc)(int* size);

namespace CPU {
	// Function to convert FILETIME to uint64_t
    uint64_t fromFileTime(const FILETIME& ft) {
        ULARGE_INTEGER uli = { 0 };
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        return uli.QuadPart;
    }

	// Function to get the CPU time
    uint64_t getCPUTime() {
        FILETIME idleTime, kernelTime, userTime;
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            uint64_t cpuTime = fromFileTime(kernelTime) + fromFileTime(userTime) + fromFileTime(idleTime);
            return cpuTime;
        }
        else {
            std::cerr << "Failed to get CPU Time : Error" << GetLastError() << std::endl;
            return -1;
        }
    }

	// Function to get the time spent by a process
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
            uint64_t kernel = fromFileTime(kernelTime);
            uint64_t user = fromFileTime(userTime);

            uint64_t totalTime = kernel + user;
            CloseHandle(hProcess);
            return totalTime;
        }
        else {
            std::cerr << "Failed to get process times. Error : " << GetLastError() << std::endl;
            CloseHandle(hProcess);
            return -1;
        }

    }

	// Function to get the current power of the CPU
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