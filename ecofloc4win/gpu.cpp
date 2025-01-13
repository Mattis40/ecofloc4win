#include "gpu.h"

#include <iostream>
#include <vector>
#include <nvml.h>

namespace GPU {
    int getOneGpuUsage(int pid) {
        nvmlReturn_t result;
        unsigned int deviceCount;
        nvmlDevice_t device;
        unsigned int infoCount = 32;
        nvmlProcessInfo_t processInfo[32];
        nvmlUtilization_t utilization;
        int usage = -1; // Default to -1 if PID is not found

        // Initialize NVML
        result = nvmlInit();
        if (result != NVML_SUCCESS) {
            std::cerr << "Failed to initialize NVML: " << nvmlErrorString(result) << std::endl;
            return -1;
        }

        // Get the number of devices
        result = nvmlDeviceGetCount(&deviceCount);
        if (result != NVML_SUCCESS) {
            std::cerr << "Failed to get device count: " << nvmlErrorString(result) << std::endl;
            nvmlShutdown();
            return -1;
        }

        // Iterate over each device
        for (unsigned int i = 0; i < deviceCount; ++i) {
            result = nvmlDeviceGetHandleByIndex(i, &device);
            if (result != NVML_SUCCESS) {
                std::cerr << "Failed to get handle for device " << i << ": " << nvmlErrorString(result) << std::endl;
                continue;
            }

            // Get GPU utilization rates
            result = nvmlDeviceGetUtilizationRates(device, &utilization);
            if (result != NVML_SUCCESS) {
                std::cerr << "Failed to get utilization rates for device " << i << ": " << nvmlErrorString(result) << std::endl;
                continue;
            }

            // Get running processes on the device
            result = nvmlDeviceGetComputeRunningProcesses(device, &infoCount, processInfo);
            if (result != NVML_SUCCESS) {
                std::cerr << "Failed to get running processes for device " << i << ": " << nvmlErrorString(result) << std::endl;
                continue;
            }

            // Search for the process by PID
            for (unsigned int j = 0; j < infoCount; ++j) {
                if (processInfo[j].pid == pid) {
                    // Approximation: Report total GPU utilization
                    usage = utilization.gpu; // Aggregate GPU utilization
                    goto cleanup; // Exit after finding the PID
                }
            }
        }

    cleanup:
        // Cleanup NVML
        nvmlShutdown();
        return usage;
    }

    // Function to retrieve GPU usage for a list of PIDs
    std::vector<int> getGpuUsages(std::vector<int> pids) {
        std::vector<int> results;
        for (int pid : pids) {
            results.push_back(getOneGpuUsage(pid));
        }
        return results;
    }

	// Function to retrieve GPU power usage
    int getGpuPower() {
        nvmlReturn_t result;
        nvmlDevice_t device;

        // Initializing NVML
        result = nvmlInit();
        if (NVML_SUCCESS != result) {
            std::cout << "Error initializing NVML: " << nvmlErrorString(result) << std::endl;
            return 1;
        }

        // Get the handle of GPU 0 (or another GPU depending on the index)
        result = nvmlDeviceGetHandleByIndex(0, &device);
        if (NVML_SUCCESS != result) {
            std::cout << "Error retrieving GPU handle: " << nvmlErrorString(result) << std::endl;
            nvmlShutdown();
            return 1;
        }

        // Read the power used (in milliwatts)
        unsigned int power;
        result = nvmlDeviceGetPowerUsage(device, &power);
        if (NVML_SUCCESS != result) {
            std::cout << "Error reading power: " << nvmlErrorString(result) << std::endl;
            nvmlShutdown();
            return 1;
        }
        // Power is given in milliwatts, so we convert it to watts

        // Cleaning and stopping NVML
        nvmlShutdown();
        return power / 1000.0;
    }

	int getGpuJoules(std::vector<int> pids, int ms) {
        double interval_s = (double)ms / 1000.0;
	    int results = 0;
		for (int pid : pids) {
			int usage = getOneGpuUsage(pid);
			int power = getGpuPower();
            double power_used = ((usage / 100.0) * power);
            results += power_used * interval_s;
		}
		return results;
	}
}