#include "gpu.h"

#include <iostream>
#include <vector>
#include <nvml.h>

namespace GPU {
    int gpu_usage(int pid) {
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
    std::vector<int> getGPUUsage(std::vector<int> pids) {
        std::vector<int> results;
        for (int pid : pids) {
            results.push_back(gpu_usage(pid));
        }
        return results;
    }

	// Function to retrieve GPU power usage
    int getGPUPower() {
        nvmlReturn_t result;
        nvmlDevice_t device;

        // Initialisation de NVML
        result = nvmlInit();
        if (NVML_SUCCESS != result) {
            std::cout << "Erreur lors de l'initialisation de NVML : " << nvmlErrorString(result) << std::endl;
            return 1;
        }

        // Obtenir le handle du GPU 0 (ou un autre GPU selon l'index)
        result = nvmlDeviceGetHandleByIndex(0, &device);
        if (NVML_SUCCESS != result) {
            std::cout << "Erreur lors de la récupération du handle du GPU : " << nvmlErrorString(result) << std::endl;
            nvmlShutdown();
            return 1;
        }

        // Lire la puissance utilisée (en milliwatts)
        unsigned int power;
        result = nvmlDeviceGetPowerUsage(device, &power);
        if (NVML_SUCCESS != result) {
            std::cout << "Erreur lors de la lecture de la puissance : " << nvmlErrorString(result) << std::endl;
            nvmlShutdown();
            return 1;
        }
        // La puissance est donnée en milliwatts, donc on la convertit en watts
        //std::cout << "Puissance utilisée par le GPU : " << power / 1000.0 << " W" << std::endl;


        // Nettoyage et arrêt de NVML
        nvmlShutdown();
        return power / 1000.0;
    }

	std::vector<int> getGPUJoules(std::vector<int> pids) {
		std::vector<int> results;
		for (int pid : pids) {
			results.push_back(gpu_usage(pid));
		}
		return results;
	}
}