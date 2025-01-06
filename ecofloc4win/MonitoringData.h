#pragma once

#include <vector>
#include <string>

/**
 * @file MonitoringData.h
 * @brief Header file for the MonitoringData class.
 *
 * This file contains the declaration of the MonitoringData class, which
 * encapsulates data and functionalities related to monitoring energy usage
 * and enabling/disabling monitoring features for an application.
 *
 * @author [Your Name]
 * @date 2025-01-06
 */

/**
 * @class MonitoringData
 * @brief Class for storing and managing monitoring data of an application.
 *
 * This class encapsulates data related to an application's monitoring, such as
 * process IDs (PIDs), energy usage of various components, and whether certain
 * features (e.g., CPU, GPU monitoring) are enabled.
 */
class MonitoringData
{
public:
    /**
     * @brief Name of the application being monitored.
     */
    std::string name;

    /**
     * @brief List of process IDs associated with the application.
     */
    std::vector<int> pids;

    /**
     * @brief Flags to indicate whether monitoring is enabled for specific components.
     */
    bool cpuEnabled = false; ///< Indicates if CPU monitoring is enabled.
    bool gpuEnabled = false; ///< Indicates if GPU monitoring is enabled.
    bool sdEnabled = false;  ///< Indicates if storage device (SD) monitoring is enabled.
    bool nicEnabled = false; ///< Indicates if network interface controller (NIC) monitoring is enabled.

    /**
     * @brief Energy consumption values for various components.
     */
    double cpuEnergy = 0.0; ///< Energy consumed by the CPU.
    double gpuEnergy = 0.0; ///< Energy consumed by the GPU.
    double sdEnergy = 0.0;  ///< Energy consumed by the storage device.
    double nicEnergy = 0.0; ///< Energy consumed by the network interface controller.

    /**
     * @brief Constructs a MonitoringData object.
     * @param appName The name of the application (default: empty string).
     * @param pids The list of process IDs associated with the application (default: empty vector).
     */
    MonitoringData(const std::string& appName = "", const std::vector<int>& pids = {})
        : name(appName), pids(pids) {}

    /**
     * @brief Retrieves the name of the monitored application.
     * @return The application's name.
     */
    std::string getName() const;

    /**
     * @brief Retrieves the list of process IDs associated with the application.
     * @return A vector containing the process IDs.
     */
    std::vector<int> getPids() const;

    /**
     * @brief Enables or disables CPU monitoring.
     * @param enabled True to enable, false to disable.
     */
    void setCPUEnabled(bool enabled);

    /**
     * @brief Enables or disables GPU monitoring.
     * @param enabled True to enable, false to disable.
     */
    void setGPUEnabled(bool enabled);

    /**
     * @brief Enables or disables storage device monitoring.
     * @param enabled True to enable, false to disable.
     */
    void setSDEnabled(bool enabled);

    /**
     * @brief Enables or disables network interface controller monitoring.
     * @param enabled True to enable, false to disable.
     */
    void setNICEnabled(bool enabled);

    /**
     * @brief Checks if CPU monitoring is enabled.
     * @return True if enabled, false otherwise.
     */
    bool isCPUEnabled() const;

    /**
     * @brief Checks if GPU monitoring is enabled.
     * @return True if enabled, false otherwise.
     */
    bool isGPUEnabled() const;

    /**
     * @brief Checks if storage device monitoring is enabled.
     * @return True if enabled, false otherwise.
     */
    bool isSDEnabled() const;

    /**
     * @brief Checks if network interface controller monitoring is enabled.
     * @return True if enabled, false otherwise.
     */
    bool isNICEnabled() const;

    /**
     * @brief Sets the energy consumed by the CPU.
     * @param energy Energy value in appropriate units.
     */
    void setCPUEnergy(double energy);

    /**
     * @brief Sets the energy consumed by the GPU.
     * @param energy Energy value in appropriate units.
     */
    void setGPUEnergy(double energy);

    /**
     * @brief Sets the energy consumed by the storage device.
     * @param energy Energy value in appropriate units.
     */
    void setSDEnergy(double energy);

    /**
     * @brief Sets the energy consumed by the network interface controller.
     * @param energy Energy value in appropriate units.
     */
    void setNICEnergy(double energy);

    /**
     * @brief Updates the energy consumed by the CPU by adding to the current value.
     * @param energy Energy value to add.
     */
    void updateCPUEnergy(double energy);

    /**
     * @brief Updates the energy consumed by the GPU by adding to the current value.
     * @param energy Energy value to add.
     */
    void updateGPUEnergy(double energy);

    /**
     * @brief Updates the energy consumed by the storage device by adding to the current value.
     * @param energy Energy value to add.
     */
    void updateSDEnergy(double energy);

    /**
     * @brief Updates the energy consumed by the network interface controller by adding to the current value.
     * @param energy Energy value to add.
     */
    void updateNICEnergy(double energy);
};
