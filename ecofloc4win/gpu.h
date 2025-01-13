#pragma once

#include <vector>

namespace GPU
{
	std::vector<int> getGpuUsages(std::vector<int> pids);
	int getGpuPower();
	int getGpuJoules(std::vector<int> pids, int ms);
};
