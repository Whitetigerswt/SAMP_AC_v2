#include "Utility.h"
#include <ctime>
#include <cstdarg>
#include <cstdio>

#include "GDK/a_http.h"

#ifdef __linux
#include <stdlib.h>
#include <unistd.h>
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
		GetModuleFileNameA((HINSTANCE)&__ImageBase, m_szPath, MAX_PATH + 1);
#else
		m_szPath = new char[4096 + 1];
		readlink("/proc/self/exe", m_szPath, sizeof(m_szPath));
#endif
		pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
		logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
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

	void OnHTTPResponse(int index, int response_code, const char * data)
	{
		switch (index) 
		{
			case AC_UPDATE_CHECK:
			{
				float version = 0.0f;
				char* site = new char[256];
				sscanf(data, "%f %s", &version, site);
				if (version > CURRENT_VERSION)
				{
					Utility::Printf("---------------------------------------");
					Utility::Printf("SAMP_AC Server plugin is out of date, and will not continue to work after this point.");
					Utility::Printf("The current version is %f and the latest version is %f!", CURRENT_VERSION, version);
					Utility::Printf("You can download the latest version for both Windows and Linux at: %s", site);
					Utility::Printf("To continue using this plugin, you must update it.");
					Utility::Printf("If you no longer wish to use this plugin, remove it from the server.cfg and adjust your scripts accordingly.");
					Utility::Printf("The server will now close.");
					Utility::Printf("---------------------------------------");

					exit(0);
				}
				break;
			}
		}
	}

	void CheckForUpdate()
	{
		HTTP(AC_UPDATE_CHECK, HTTP_GET, AC_UPDATE_URL, "");
	}
}