#include "Utility.h"
#include "../Shared/MD5_Info/Cmd5Info.h"

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
		GetModuleFileNameA(NULL, m_szPath, MAX_PATH + 1);
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

	void CheckForUpdate()
	{
		// Get the website's response
		std::string updatestring = Cmd5Info::GetWebsiteText(AC_UPDATE_URL);

		// Create new variables to split up the version and the update URL.
		float version = 0.0f;
		char* site = new char[256];

		// Split up the version and update URL.
		if (sscanf(updatestring.c_str(), "%f %s", &version, site) != EOF)
		{
			// If the version returned from the website is greater than the current version
			if (version > CURRENT_VERSION)
			{
				// Force the user to update.
				Utility::Printf("_____________________________________________________________");
				Utility::Printf("");
				Utility::Printf("SAMP_AC Server plugin is out of date, and will not continue to work after this point.");
				Utility::Printf("The current version is %.2f and the latest version is %.2f!", CURRENT_VERSION, version);
				Utility::Printf("You can download the latest version for both Windows and Linux at: %s", site);
				Utility::Printf("To continue using this plugin, you must update it.");
				Utility::Printf("If you no longer wish to use this plugin, remove it from the server.cfg and adjust your scripts accordingly.");
				Utility::Printf("The server will now close.");
				Utility::Printf("_____________________________________________________________");

				exit(0);
			}
			else
			{
				Utility::Printf("_____________________________________________________________");
				Utility::Printf("");
				Utility::Printf("** Already up to date!");
				Utility::Printf("");
				Utility::Printf("_____________________________________________________________");
			}
		}
		delete[] site;
	}
}