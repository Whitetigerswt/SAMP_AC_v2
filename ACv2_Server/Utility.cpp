#include "Utility.h"
#include <ctime>

extern void* pAMXFunctions;

namespace Utility
{
	static char* m_szPath;
	static logprintf_t logprintf;

	void Initialize(void** ppData)
	{
		m_szPath = new char[MAX_PATH + 1];
#ifdef WIN32
		GetModuleFileNameA((HINSTANCE)&__ImageBase, m_szPath, MAX_PATH + 1);
#else
		readlink("/proc/self/exe", szAppPath, sizeof(szAppPath));
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

		return logprintf("[SAMP_AC_V2] [%u] %s", (unsigned __int32)time(NULL), szBuffer);
	}
}