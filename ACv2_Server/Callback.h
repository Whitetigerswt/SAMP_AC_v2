#pragma once

#include <list>
#include "SDK/samp-sdk/amx/amx.h"
#include <string>

struct PAWNArray
{
	cell* address;
	size_t length;
};

namespace Callback
{
	// push arguments in reversed order
	cell Execute(const char* szFunction, const char* szFormat, ...);

	std::list<AMX*>& GetAMXList();

	void OnACClosed(std::string ip);
	void OnACClosed(unsigned int playerid);

}