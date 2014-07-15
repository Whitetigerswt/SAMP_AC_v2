#include <WinSock2.h>
#include <Windows.h>
#include "CLoader.h"
#include "../Shared/Boost/thread.hpp"

void WINAPI Load();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		
		if (CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Load, NULL, 0, 0) == NULL) {
			ExitProcess(GetLastError());
			return FALSE;
		}
		//boost::thread theThread(CLoader::Initialize);
		break;
	}
	return TRUE;
}

void WINAPI Load()
{
	CLoader::Initialize();
}