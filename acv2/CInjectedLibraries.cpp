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
	// Create variables
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	// Get all modules handle in the current process.
	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		// Loop through all modules
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Create a variable to hold the path name on each module
			wchar_t szModName[MAX_PATH];
			
			// Get the module file name for the variable we just created.
			if (GetModuleFileNameEx(GetCurrentProcess(), hMods[i], szModName,
				sizeof(szModName) / sizeof(char)))
			{

				// Convert the path to an std::string
				std::wstring file(szModName);

				// Make sure the module isn't already on the file list.
				if (!DoesFileExist(file))
				{
					// Add the file to the file list.
					AddFile(file);
				}
			}
		}
	}
}