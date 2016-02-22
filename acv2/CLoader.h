#include "CDirectX.h"
#include "CProcessList.h"
#include "CInjectedLibraries.h"
#include "CDirectoryScanner.h"

typedef HMODULE(WINAPI* LoadLibrary_t)(LPCSTR);

class CLoader 
{
public:

	// PURPOSE: Initialize main program.
	// REQUIRES: The current .asi's HMODULE.
	// PROMISE: Program main facilities will be initiated.
	static void Initialize(HMODULE hMod);

	// PURPOSE: Relaunch gta_sa as an elevated process.
	// REQUIRES: NULL
	// PROMISE: The program will be relaunched as an elevated process, and then the current process will exit.
	static void RunElevated();

	// PURPOSE: Check if we've already loaded
	// REQUIRES: NULL
	static bool IsLoaded();

	// PURPOSE: Determine if the process is elevated.
	// REQUIRES: NULL
	static BOOL IsProcessElevated();

	static CInjectedLibraries Modules;
	static CProcessList Processes;
	static CDirectoryScanner GtaDirectory;
	static HMODULE ThishMod;

private:
	static bool isLoaded;

	static int isElevated;

};