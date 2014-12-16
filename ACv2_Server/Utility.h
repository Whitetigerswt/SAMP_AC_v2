#pragma once

#define SAFE_FREE(ptr) if(ptr) delete ptr;

#include <string>
#include "SDK/samp-sdk/plugincommon.h"

typedef void(*logprintf_t)(char* format, ...);

#ifdef WIN32
#include <Windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;
#else
#endif

namespace Utility
{
	// PURPOSE: Load the plugin data, and get logprintf address.
	// REQUIRES: pointer to amx data
	void Initialize(void** ppData);

	// PURPOSE: Print a line out to the server console.
	// REQUIRES: A string to print.
	void Printf(const char* szFormat, ...);

	// PURPOSE: Get path to main server program.
	// REQUIRES: NULL
	char* GetServerPath();
}

#ifndef CHECK_PARAMS
#define CHECK_PARAMS(m,n)                                                                                           \
		do                                                                                                              \
				{                                                                                                               \
			if (pParams[0] != (m * 4))                                                                                   \
						{                                                                                                           \
				Utility::Printf(" Error: Incorrect parameter count on \"" n "\", %d != %d\n", m, pParams[0] / 4); \
				return 0;                                                                                               \
						}                                                                                                           \
				}                                                                                                               \
						while (0)
#endif