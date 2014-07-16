#include "Utility.h"
#include <ctime>
#include <cstdarg>
#include <cstdio>

#ifdef __linux
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
}