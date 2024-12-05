#pragma once

#include <vector>

namespace GPU
{
	std::vector<int> getGPUUsage(std::vector<int> pids);
	int getGPUPower();
	std::vector<int> getGPUJoules(std::vector<int> pids);
};
