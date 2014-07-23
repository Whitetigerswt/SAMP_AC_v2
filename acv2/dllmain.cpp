#include <Windows.h>
#include "CLoader.h"
#include "../Shared/Boost/thread.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		
		
		boost::thread theThread(&CLoader::Initialize, hModule);
		break;
	}
	return TRUE;
}