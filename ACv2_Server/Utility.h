#pragma once

#define AC_WEBSITE "https://whitetigerswt.github.io/SAMP_AC_v2"

#define MAX_CHEAT_PATH	20

#define SAFE_FREE(ptr) if(ptr) delete ptr;

#include <string>
#include "SDK/samp-sdk/plugincommon.h"

typedef void(*logprintf_t)(char* format, ...);

#ifdef WIN32
#include <Windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;
#else
#endif

#ifdef WIN32
#ifndef snprintf
#define snprintf sprintf_s
#endif
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

	// PURPOSE: Crossplatform GetTickCount.
	// REQUIRES: NULL
	unsigned getTickCount();

	// PURPOSE: Truncate a given file path if it exeeds MAX_CHEAT_PATH.
	// REQUIRES: Original file path.
	// RETURNS: char* - a safe (truncated) file path or the original file path if it does not exceed MAX_CHEAT_PATH.
	// NOTES: This function is mostly used with samp client messages since they have length limit.
	char* GetSafeFilePath(char* orginalFilePath);
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