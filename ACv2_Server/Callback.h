#pragma once

#include <list>
#include "SDK/samp-sdk/amx/amx.h"

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
}