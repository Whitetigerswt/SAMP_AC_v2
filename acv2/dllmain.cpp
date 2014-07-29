#include <Windows.h>
#include <Boost\thread.hpp>

#include "CCleoManager.h"
#include "CLoader.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (!CLoader::IsLoaded())
			{
				// Load the CLEO hooks so CLEO will not load scripts properly.
				CCleoManager::Load();
				
				// Do the main loading procedure in a new thread.
				boost::thread theThread(&CLoader::Initialize, hModule);
				break;
			}
		}
	}
	return TRUE;
}