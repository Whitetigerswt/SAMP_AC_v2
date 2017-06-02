#include "CDirectX.h"
#include "CProcessList.h"
#include "CInjectedLibraries.h"
#include "CDirectoryScanner.h"

typedef HMODULE(WINAPI* LoadLibrary_t)(const wchar_t *);

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
	
	// PURPOSE: Get process file name by ID.
	// REQUIRES: Process ID.
	// PROMISE: A returned file name.
	static std::wstring GetProcessFileName(DWORD processID);
	
	// PURPOSE: Terminates other GTA:SA processes.
	// REQUIRES: NULL
    static void TerminateOtherProcesses();

	// PURPOSE: Determine if the process is elevated.
	// REQUIRES: NULL
	static BOOL IsProcessElevated();

	// PURPOSE: Determine if the game is fully loaded (past loading screen)
	// REQUIRES: NULL
	static BOOL IsGameLoaded();

	static CInjectedLibraries Modules;
	static CProcessList Processes;
	static CDirectoryScanner GtaDirectory;
	static HMODULE ThishMod;

private:
	// PURPOSE: Check elevation of the program, and elevate if necessary.
	// REQUIRES: NULL
	// PROMISE: An elevated process after it returns.
	static void CheckElevation();

	// PURPOSE: Attempt to protect the process from use of outside process manipulation.
	static DWORD ProtectProcess();

	static bool isLoaded;
	static BOOL isGameLoaded;

	static int isElevated;

};