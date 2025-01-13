#pragma once

#include <iostream>
#include <Windows.h>

namespace CPU
{
	uint64_t convertFromFileTime(const FILETIME& ft);
	uint64_t getCpuTime();
	uint64_t getPidTime(DWORD pid);
	bool getCurrentPower(double& power);
};

