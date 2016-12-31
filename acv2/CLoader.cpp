#include "CLoader.h"
#include "../Shared/Network/Network.h"
#include "CParseCommandLine.h"
#include "Addresses.h"
#include "CClientUpdater.h"
#include "Misc.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "CHookManager.h"
#include "Enigma/enigma_ide.h"
#include "../Shared/Network/CRPC.h"
#include "Detours\detours.h"
#include "CModuleSecurity.h"
#include "Network\CRakClientHandler.h"
#include "VersionHelpers.h"
#include "ManualInjection.h"
#include "s0beit\samp.h"

#include <map>
#include <Aclapi.h>
#include <Shellapi.h>
#include <Boost\thread.hpp>
#include <Psapi.h>

#include <process.h>
#include <Tlhelp32.h>
#include <windows.h>
#include <string.h>
#include <tchar.h>

CInjectedLibraries CLoader::Modules = CInjectedLibraries();
CProcessList CLoader::Processes = CProcessList();
CDirectoryScanner CLoader::GtaDirectory = CDirectoryScanner();
int CLoader::isElevated = false;
bool CLoader::isLoaded = false;
HMODULE CLoader::ThishMod = NULL;

void CLoader::Initialize(HMODULE hMod)
{
	boost::this_thread::yield();
	CHookManager::Load();

	if (EP_CheckupIsEnigmaOk() || !EP_CheckupIsProtected())
	{
		/*
 		    Make sure other GTA:SA processes are not running.
			This prevents crashing while game is starting.
		*/
		
        TerminateOtherProcesses();

		/* 
			Force process elevation check. This will terminate the current process and run a new one if it's 
			not elevated (if it's not running as admin).

			Check https://github.com/Whitetigerswt/SAMP_AC_v2/issues/133 if you wonder why this is necessary!
		*/

		// Check if Windows version is vista or greater...
		if (IsWindowsVistaOrGreater())
		{
			// Check if process is not elevated (not running as admin)
			if (!IsProcessElevated())
			{
				CheckElevation();
                ExitProcess(0);
				return;
			}
		}

		// Make sure samp.dll is loaded BEFORE we go ANY further!!
		HMODULE L = LoadLibrary(TEXT("samp.dll"));

		MODULEINFO mInfo = { 0 };

		GetModuleInformation(GetCurrentProcess(), L, &mInfo, sizeof(MODULEINFO));

		setSampBaseAddress((DWORD)mInfo.lpBaseOfDll);
		setSampSize((DWORD)mInfo.SizeOfImage);

		// Hide samp.dll and this .asi from the loaded module list.
		PELPEB peb = EL_GetPeb();
		EL_HideModule(peb, TEXT("samp.dll"));
		wchar_t path[MAX_PATH];
		GetModuleFileName(hMod, path, sizeof(path));
		EL_HideModule(peb, path);

		CHookManager::Load();

		// Hook LoadLibrary function.
		CModuleSecurity::HookLoadLibrary();

		// Record that we're loaded
		isLoaded = true;

		// Keep track of the hModule.
		ThishMod = hMod;

		// Hook the D3D9Device functions.
		CDirectX::HookD3DFunctions();

		// Wait until the game is loaded.
		while (ADDRESS_LOADED < 6)
		{
			// Stop CLEO from loading, and other memory hooks.
			CHookManager::Load();

			// Wait until the game is loaded in an infinite loop.
			Sleep(5);
		}

		CModuleSecurity::AddAllowedModules();

		// Make sure we're using the latest version of this mod.
		CClientUpdater::CheckForUpdate(hMod);
	}

	while (true)
	{
		// Scan for new processes.
		Processes.Scan();

		// Scan for new injected modules.
		Modules.Scan();

		// Sleep
		Sleep(1000);
	}
}

std::wstring CLoader::GetProcessFileName(DWORD processID)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
	
    while (hRes)
    {
		if(processID == pEntry.th32ProcessID)
		{
			// Process ID matches. Let's return it's file name.
			std::wstring pFileName(pEntry.szExeFile);
		    return pFileName;
		}
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);	
	return L"NULL";
}

void CLoader::TerminateOtherProcesses()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);

	// Get current process ID.
	DWORD currentProcessID = GetCurrentProcessId();

	// Get current process file name. (GetModuleFileName doesn't work with mklink, so we need to do a snapshot.)
	std::wstring currentProcessName = GetProcessFileName(currentProcessID);

	if (currentProcessName == L"NULL")
		return;

	while (hRes)
	{
		if (currentProcessID != pEntry.th32ProcessID && wcscmp(pEntry.szExeFile, currentProcessName.c_str()) == 0)
		{
			// Other GTA:SA process has been found. Terminate it!
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}


void CLoader::CheckElevation()
{
	// Check if process is elevated already
	if (!IsProcessElevated())
	{

		while (ADDRESS_LOADED < 6)
		{
			Sleep(5);
		}

		// If it's not, we need to elevate it.
		RunElevated();
		ExitProcess(0);
	}

	// Set this processes security descriptor, so other processes cannot interfere with this process.
	ProtectProcess();
}

DWORD CLoader::ProtectProcess()
{
	// Returned to caller
	DWORD dwResult = (DWORD)-1;

	// Released on exit
	HANDLE hToken = NULL;
	PVOID pTokenInfo = NULL;

	PSID psidEveryone = NULL;
	PSID psidSystem = NULL;
	PSID psidAdmins = NULL;

	PACL pDacl = NULL;
	PSECURITY_DESCRIPTOR pSecDesc = NULL;

	__try
	{
		// Scratch
		DWORD dwSize = 0;
		BOOL bResult = FALSE;

		// If this fails, you can try to fallback to OpenThreadToken
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken)) {
			dwResult = GetLastError();
			assert(FALSE);
			__leave; /*failed*/
		}

		bResult = GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
		dwResult = GetLastError();
		assert(bResult == FALSE && ERROR_INSUFFICIENT_BUFFER == dwResult);
		if (!(bResult == FALSE && ERROR_INSUFFICIENT_BUFFER == dwResult)) { __leave; /*failed*/ }

		if (dwSize) {
			pTokenInfo = HeapAlloc(GetProcessHeap(), 0, dwSize);
			dwResult = GetLastError();
			assert(NULL != pTokenInfo);
			if (NULL == pTokenInfo) { __leave; /*failed*/ }
		}

		bResult = GetTokenInformation(hToken, TokenUser, pTokenInfo, dwSize, &dwSize);
		dwResult = GetLastError();
		assert(bResult && pTokenInfo);
		if (!(bResult && pTokenInfo)) { __leave; /*failed*/ }

		PSID psidCurUser = ((TOKEN_USER*)pTokenInfo)->User.Sid;

		SID_IDENTIFIER_AUTHORITY sidEveryone = SECURITY_WORLD_SID_AUTHORITY;
		bResult = AllocateAndInitializeSid(&sidEveryone, 1,
			SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &psidEveryone);
		dwResult = GetLastError();
		assert(bResult && psidEveryone);
		if (!(bResult && psidEveryone)) { __leave; /*failed*/ }

		SID_IDENTIFIER_AUTHORITY sidSystem = SECURITY_NT_AUTHORITY;
		bResult = AllocateAndInitializeSid(&sidSystem, 1,
			SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &psidSystem);
		dwResult = GetLastError();
		assert(bResult && psidSystem);
		if (!(bResult && psidSystem)) { __leave; /*failed*/ }

		SID_IDENTIFIER_AUTHORITY sidAdministrators = SECURITY_NT_AUTHORITY;
		bResult = AllocateAndInitializeSid(&sidAdministrators, 2,
			SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0, &psidAdmins);
		dwResult = GetLastError();
		assert(bResult && psidAdmins);
		if (!(bResult && psidAdmins)) { __leave; /*failed*/ }

		const PSID psidArray[] = {
			psidEveryone,    /* Deny most rights to everyone */
			psidCurUser,    /* Allow what was not denied */
			psidSystem,        /* Full control */
			psidAdmins,        /* Full control */
		};

		// Determine required size of the ACL
		dwSize = sizeof(ACL);

		// First the DENY, then the ALLOW
		dwSize += GetLengthSid(psidArray[0]);
		dwSize += sizeof(ACCESS_DENIED_ACE) - sizeof(DWORD);

		for (UINT i = 1; i<_countof(psidArray); i++) {
			// DWORD is the SidStart field, which is not used for absolute format
			dwSize += GetLengthSid(psidArray[i]);
			dwSize += sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);
		}

		pDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwSize);
		dwResult = GetLastError();
		assert(NULL != pDacl);
		if (NULL == pDacl) { __leave; /*failed*/ }

		bResult = InitializeAcl(pDacl, dwSize, ACL_REVISION);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		// Mimic Protected Process
		// http://www.microsoft.com/whdc/system/vista/process_vista.mspx
		// Protected processes allow PROCESS_TERMINATE, which is
		// probably not appropriate for high integrity software.
		static const DWORD dwPoison =
			/*READ_CONTROL |*/ WRITE_DAC | WRITE_OWNER |
			PROCESS_CREATE_PROCESS | PROCESS_CREATE_THREAD |
			PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION |
			PROCESS_SET_QUOTA | PROCESS_SET_INFORMATION |
			PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE |
			// In addition to protected process
			PROCESS_SUSPEND_RESUME | PROCESS_TERMINATE;
		bResult = AddAccessDeniedAce(pDacl, ACL_REVISION, dwPoison, psidArray[0]);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		// Standard and specific rights not explicitly denied
		static const DWORD dwAllowed = ~dwPoison & 0x1FFF;
		bResult = AddAccessAllowedAce(pDacl, ACL_REVISION, dwAllowed, psidArray[1]);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		// Because of ACE ordering, System will effectively have dwAllowed even
		// though the ACE specifies PROCESS_ALL_ACCESS (unless software uses
		// SeDebugPrivilege or SeTcbName and increases access).
		// As an exercise, check behavior of tools such as Process Explorer under XP,
		// Vista, and above. Vista and above should exhibit slightly different behavior
		// due to Restricted tokens.
		bResult = AddAccessAllowedAce(pDacl, ACL_REVISION, PROCESS_ALL_ACCESS, psidArray[2]);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		// Because of ACE ordering, Administrators will effectively have dwAllowed
		// even though the ACE specifies PROCESS_ALL_ACCESS (unless the Administrator
		// invokes 'discretionary security' by taking ownership and increasing access).
		// As an exercise, check behavior of tools such as Process Explorer under XP,
		// Vista, and above. Vista and above should exhibit slightly different behavior
		// due to Restricted tokens.
		bResult = AddAccessAllowedAce(pDacl, ACL_REVISION, PROCESS_ALL_ACCESS, psidArray[3]);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		pSecDesc = (PSECURITY_DESCRIPTOR)HeapAlloc(GetProcessHeap(), 0, SECURITY_DESCRIPTOR_MIN_LENGTH);
		dwResult = GetLastError();
		assert(NULL != pSecDesc);
		if (NULL == pSecDesc) { __leave; /*failed*/ }

		// InitializeSecurityDescriptor initializes a security descriptor in
		// absolute format, rather than self-relative format. See
		// http://msdn.microsoft.com/en-us/library/aa378863(VS.85).aspx
		bResult = InitializeSecurityDescriptor(pSecDesc, SECURITY_DESCRIPTOR_REVISION);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		bResult = SetSecurityDescriptorDacl(pSecDesc, TRUE, pDacl, FALSE);
		dwResult = GetLastError();
		assert(TRUE == bResult);
		if (FALSE == bResult) { __leave; /*failed*/ }

		dwResult = SetSecurityInfo(
			GetCurrentProcess(),
			SE_KERNEL_OBJECT, // process object
			OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
			psidCurUser, // NULL, // Owner SID
			NULL, // Group SID
			pDacl,
			NULL // SACL
			);
		dwResult = GetLastError();
		assert(ERROR_SUCCESS == dwResult);
		if (ERROR_SUCCESS != dwResult) { __leave; /*failed*/ }

		dwResult = ERROR_SUCCESS;
	}
	__finally
	{
		if (NULL != pSecDesc) {
			HeapFree(GetProcessHeap(), 0, pSecDesc);
			pSecDesc = NULL;
		}
		if (NULL != pDacl) {
			HeapFree(GetProcessHeap(), 0, pDacl);
			pDacl = NULL;
		}
		if (psidAdmins) {
			FreeSid(psidAdmins);
			psidAdmins = NULL;
		}
		if (psidSystem) {
			FreeSid(psidSystem);
			psidSystem = NULL;
		}
		if (psidEveryone) {
			FreeSid(psidEveryone);
			psidEveryone = NULL;
		}
		if (NULL != pTokenInfo) {
			HeapFree(GetProcessHeap(), 0, pTokenInfo);
			pTokenInfo = NULL;
		}
		if (NULL != hToken) {
			CloseHandle(hToken);
			hToken = NULL;
		}
	}

	return dwResult;
}

void CLoader::RunElevated()
{
	// Set our info to run gta_sa.exe with admin permissions
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = TEXT("runas");
	sei.lpFile = TEXT("gta_sa.exe");
	sei.nShow = SW_NORMAL;
	sei.lpParameters = GetCommandLine();

	// Execute (this literally relaunches gta_sa.exe as admin)
	ShellExecuteEx(&sei);

	// Exit process since we're not elevated.
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