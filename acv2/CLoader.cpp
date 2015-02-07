#include "CLoader.h"
#include "../Shared/Network/Network.h"
#include "CParseCommandLine.h"
#include "Addresses.h"
#include "CClientUpdater.h"
#include "Misc.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "CHookManager.h"
#include "enigma_ide.h"
#include "../Shared/Network/CRPC.h"
#include "CCrashHandler.h"
#include "Detours\detours.h"
#include "CModuleSecurity.h"
#include "Network\CRakClientHandler.h"
#include "VersionHelpers.h"

#include <map>
#include <Shellapi.h>
#include <Boost\thread.hpp>

CInjectedLibraries CLoader::Modules = CInjectedLibraries();
CProcessList CLoader::Processes = CProcessList();
CDirectoryScanner CLoader::GtaDirectory = CDirectoryScanner();
int CLoader::isElevated = false;
bool CLoader::isLoaded = false;
HMODULE CLoader::ThishMod = NULL;

void CLoader::Initialize(HMODULE hMod)
{
	// Install the crash handler.
	new CCrashHandler();

	// Make sure samp.dll is loaded BEFORE we go ANY further!!
	LoadLibrary("samp.dll");

	// Hook LoadLibrary function.
	CModuleSecurity::HookLoadLibrary();

	// Record that we're loaded
	isLoaded = true;

	// Keep track of the hModule.
	ThishMod = hMod;

	// Hook the D3D9Device functions.
	CDirectX::HookD3DFunctions();
	
	// Load the command line in a string (mostly the host, and port so we can connect later)
	std::map < std::string, std::string > cmdline;
	cmdline = CParseCommandLine::parseCmdLine(GetCommandLineA());
	
	// Wait until the game is loaded.
	while (ADDRESS_LOADED < 6)
	{
		// Stop CLEO from loading, and other memory hooks.
		CHookManager::Load();

		//CDirectX::LoadImages();

		// Wait until the game is loaded in an infinite loop.
		Sleep(5);
	}

	CModuleSecurity::AddAllowedModules();
	
	// Make sure we're using the latest version of this mod.
	CClientUpdater::CheckForUpdate(hMod);

	// Force process elevation once the game has loaded. This will terminate the current process and run a new one.
	if (IsWindowsVistaOrGreater())
	{
		CheckElevation();
	}

	// Setup memory one more time.
	CHookManager::Load();

	while (true)
	{
		// Scan for new processes.
		Processes.Scan();

		// Scan for new injected modules.
		Modules.Scan();

		// Check for a tamper attempt.
		/*if (!EP_CheckupIsProtected() || !EP_CheckupIsEnigmaOk() || EP_CheckupVirtualizationTools())
		{
			// Make sure the user is connected.
			if (!CRakClientHandler::IsConnected())
			{
				// Close the process.
				ExitProcess(0);
			}

			RakNet::BitStream bsData;

			bsData.Write((unsigned char)PACKET_RPC);
			bsData.Write(ON_TAMPER_ATTEMPT);
			// Tell the server we've done some naughty stuff.
			CRakClientHandler::CustomSend(&bsData);
		}*/

		// Sleep
		Sleep(1000);
	}
}

void CLoader::CheckElevation()
{
	// Check if process is elevated already
	if (!IsProcessElevated())
	{
		// If it's not, we need to elevate it.
		// run our elevator .exe program
		RunElevated();
		ExitProcess(0);
	}
}

void CLoader::RunElevated()
{
	// Set our info to run the samp_elevator.exe
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = "runas";
	sei.lpFile = "gta_sa.exe";
	sei.nShow = SW_NORMAL;
	sei.lpParameters = GetCommandLineA();

	ShellExecuteEx(&sei);

	// Exit process since we're not elevated, and the elevator.exe will create a new process with elevation permissions.
	ExitProcess(0);
}

BOOL CLoader::IsProcessElevated()
{
	if (isElevated) return 1;

	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:
	// Centralized cleanup for all allocated resources.
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	// Throw the error if something failed in the function.
	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	isElevated = fIsRunAsAdmin;

	return fIsRunAsAdmin;
}

bool CLoader::IsLoaded()
{
	return isLoaded;
}