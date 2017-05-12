#include "Utility.h"
#include "../Shared/MD5_Info/Cmd5Info.h"

#include <ctime>
#include <cstdarg>
#include <cstdio>

#include "GDK/sampgdk.h"

#ifdef __linux
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#endif

extern void* pAMXFunctions;

namespace Utility
{
	static char* m_szPath;
	static logprintf_t logprintf;

	void Initialize(void** ppData)
	{
#ifdef WIN32
		m_szPath = new char[MAX_PATH + 1];
		GetModuleFileNameA(NULL, m_szPath, MAX_PATH + 1);
#else
		m_szPath = new char[4096 + 1];
		readlink("/proc/self/exe", m_szPath, sizeof(m_szPath));
#endif
		pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
		logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	}

	char* GetServerPath()
	{
		return m_szPath;
	}


	void Printf(const char* szFormat, ...)
	{
		va_list vaArgs;
		char szBuffer[2048];
		va_start(vaArgs, szFormat);
		vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
		va_end(vaArgs);

		logprintf("[SAMP_AC_V2] %s", szBuffer);
	}

	char* GetSafeFilePath(char* orginalFilePath)
	{
		// If the given file path exceeds MAX_CHEAT_PATH.
		if (strlen(orginalFilePath) > MAX_CHEAT_PATH)
		{
			// Create a string to hold the safe file path.
			char shortFilePath[MAX_CHEAT_PATH];

			// Copy the given file path to sized buffer (truncate to fight against overflow).
			strncpy(shortFilePath, orginalFilePath, MAX_CHEAT_PATH - 1);

			// Add dots at the end to indicate that the file path has been truncated.
			shortFilePath[MAX_CHEAT_PATH - 3] = '.';
			shortFilePath[MAX_CHEAT_PATH - 2] = '.';

			// Add NULL character.
			shortFilePath[MAX_CHEAT_PATH - 1] = '\0';
			
			orginalFilePath = shortFilePath;
		}
		return orginalFilePath;
	}

	unsigned getTickCount()
	{
#ifdef WIN32
		return GetTickCount();
#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return unsigned((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
	}
}
