#pragma once

#include <vector>
#include <string>
#include <windows.h>
#include <map>

struct IoEventInfo {
	DWORD pid;
	std::wstring processName;
	USHORT operationType;
	ULONG bytesTransferred = 0;  // Track total bytes transferred for each IRP
};

class MonitoringData
{
public:
	std::string name;
	std::vector<int> pids;
	std::map<ULONGLONG, IoEventInfo> irpMap;

	bool cpuEnabled = false;
	bool gpuEnabled = false;
	bool sdEnabled = false;
	bool nicEnabled = false;

	double cpuEnergy = 0.0;
	double gpuEnergy = 0.0;
	double sdEnergy = 0.0;
	double nicEnergy = 0.0;

	MonitoringData(const std::string& appName = "", const std::vector<int>& pids = {}) : name(appName), pids(pids) {}

	std::string getName() const;
	std::vector<int> getPids() const;

	void setCpuEnabled(bool enabled);
	void setGpuEnabled(bool enabled);
	void setSdEnabled(bool enabled);
	void setNicEnabled(bool enabled);

	bool isCpuEnabled() const;
	bool isGpuEnabled() const;
	bool isSdEnabled() const;
	bool isNicEnabled() const;

	void setCpuEnergy(double energy);
	void setGpuEnergy(double energy);
	void setSdEnergy(double energy);
	void setNicEnergy(double energy);

	void updateCpuEnergy(double energy);
	void updateGpuEnergy(double energy);
	void updateSdEnergy(double energy);
	void updateNicEnergy(double energy);

	void addIrp(ULONGLONG irpAddress, const IoEventInfo& info);
	void updateIrp(ULONGLONG irpAddress, ULONG bytesTransferred);
	void removeIrp(ULONGLONG irpAddress);
};

