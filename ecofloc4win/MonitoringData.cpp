#include "MonitoringData.h"

std::string MonitoringData::getName() const
{
	return name;
}

std::vector<int> MonitoringData::getPids() const
{
	return pids;
}

void MonitoringData::setCpuEnabled(bool enabled)
{
	cpuEnabled = enabled;
}

void MonitoringData::setGpuEnabled(bool enabled)
{
	gpuEnabled = enabled;
}

void MonitoringData::setSdEnabled(bool enabled)
{
	sdEnabled = enabled;
}

void MonitoringData::setNicEnabled(bool enabled)
{
	nicEnabled = enabled;
}

bool MonitoringData::isCpuEnabled() const
{
	return cpuEnabled;
}

bool MonitoringData::isGpuEnabled() const
{
	return gpuEnabled;
}

bool MonitoringData::isSdEnabled() const
{
	return sdEnabled;
}

bool MonitoringData::isNicEnabled() const
{
	return nicEnabled;
}

void MonitoringData::setCpuEnergy(double energy)
{
	cpuEnergy = energy;
}

void MonitoringData::setGpuEnergy(double energy)
{
	gpuEnergy = energy;
}

void MonitoringData::setSdEnergy(double energy)
{
	sdEnergy = energy;
}

void MonitoringData::setNicEnergy(double energy)
{
	nicEnergy = energy;
}

void MonitoringData::updateCpuEnergy(double energy)
{
	cpuEnergy += energy;
}

void MonitoringData::updateGpuEnergy(double energy)
{
	gpuEnergy += energy;
}

void MonitoringData::updateSdEnergy(double energy)
{
	sdEnergy += energy;
}

void MonitoringData::updateNicEnergy(double energy)
{
	nicEnergy += energy;
}

void MonitoringData::addIrp(ULONGLONG irpAddress, const IoEventInfo& info) {
	irpMap[irpAddress] = info;
}

void MonitoringData::updateIrp(ULONGLONG irpAddress, ULONG bytesTransferred) {
	if (irpMap.find(irpAddress) != irpMap.end()) {
		irpMap[irpAddress].bytesTransferred += bytesTransferred;
	}
}

void MonitoringData::removeIrp(ULONGLONG irpAddress) {
	irpMap.erase(irpAddress);
}