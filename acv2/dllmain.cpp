#include <Windows.h>
#include <Boost\thread.hpp>

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
				boost::thread theThread(&CLoader::Initialize, hModule);
				break;
			}
		}
	}
	return TRUE;
}