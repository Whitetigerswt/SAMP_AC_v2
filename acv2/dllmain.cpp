#include <Windows.h>
#include <Boost\thread.hpp>

#include "CHookManager.h"
#include "CLoader.h"
#include "Addresses.h"
#include "CCrashHandler.h"

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
				CHookManager::Load();

				// Make sure we aren't loading this DLL at sometime other than init
				if (ADDRESS_LOADED >= 6)
				{
					FreeLibraryAndExitThread(hModule, 0);
				}
		
				// Do the main loading procedure in a new thread.
				boost::thread theThread(&CLoader::Initialize, hModule);
				break;
			}
		}
	}
	return TRUE;
}