#include "CInjectedLibraries.h"
#include <Windows.h>
#include <psapi.h>

CInjectedLibraries::CInjectedLibraries()
{
}


CInjectedLibraries::~CInjectedLibraries()
{
}

void CInjectedLibraries::Scan()
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			char szModName[MAX_PATH];
			if (GetModuleFileNameEx(GetCurrentProcess(), hMods[i], szModName,
				sizeof(szModName) / sizeof(char)))
			{
				std::string file(szModName);
				if (!DoesFileExist(file))
				{
					AddFile(file);
				}
			}
		}
	}
}