#pragma once

#define SAFE_FREE(ptr) if(ptr) delete ptr;

#include <string>
#include "SDK/samp-sdk/plugincommon.h"

typedef void(*logprintf_t)(char* format, ...);

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;
#else
#endif

namespace Utility
{
	void Initialize(void** ppData);
	void Printf(const char* szFormat, ...);
}