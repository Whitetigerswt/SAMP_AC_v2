#include "CModuleSecurity.h"
#include "Detours\detours.h"
#include <psapi.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <Windows.h>

LoadLibrary_t CModuleSecurity::m_pLoadLibrary = NULL;
std::vector<std::string> CModuleSecurity::m_AllowedModules;

typedef NTSTATUS(WINAPI *pNtQIT)(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);

CModuleSecurity::CModuleSecurity()
{
}


CModuleSecurity::~CModuleSecurity()
{
}

void CModuleSecurity::HookLoadLibrary()
{
	PBYTE addr = DetourFindFunction("kernel32.dll", "LoadLibraryA");

	if (addr != NULL)
	{
		m_pLoadLibrary = (LoadLibrary_t)DetourFunction(addr, (BYTE*)HOOK_LoadLibrary);
	}
}

HMODULE WINAPI CModuleSecurity::HOOK_LoadLibrary(LPCTSTR lpFileName)
{
	AddAllowedModule(lpFileName);

	return m_pLoadLibrary(lpFileName);
}

void CModuleSecurity::AddAllowedModule(LPCTSTR lpFileName)
{
	m_AllowedModules.push_back(lpFileName);
}

void CModuleSecurity::AddAllowedModules()
{
	MODULEENTRY32 me32;

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (hModuleSnap != INVALID_HANDLE_VALUE)
	{
		me32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hModuleSnap, &me32))
		{
			do
			{
				m_AllowedModules.push_back(me32.szModule);
			} while (Module32Next(hModuleSnap, &me32));
		}
	}
}


bool CModuleSecurity::IsAllowedModule(std::string szModule)
{
	for (std::vector<std::string>::iterator it = m_AllowedModules.begin(); it != m_AllowedModules.end(); ++it)
	{
		if (it->compare(szModule) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CModuleSecurity::IsAddressInAllowedModule(DWORD address)
{
	MODULEENTRY32 me32;
	bool range = false;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (hModuleSnap != INVALID_HANDLE_VALUE) 
	{
		me32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hModuleSnap, &me32)) 
		{
			while (Module32Next(hModuleSnap, &me32)) 
			{
				if (IsAllowedModule(me32.szModule))
				{
					if (address >= (DWORD)me32.modBaseAddr && address < (DWORD)(me32.modBaseAddr + me32.modBaseSize))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

DWORD WINAPI CModuleSecurity::GetThreadStartAddress(DWORD tid)
{

	NTSTATUS ntStatus;
	HANDLE hDupHandle;
	DWORD dwStartAddress;

	HANDLE hProcess;

	HANDLE hTread;

	hTread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, tid);

	pNtQIT NtQueryInformationThread;
	NtQueryInformationThread = (pNtQIT)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread");

	if (NtQueryInformationThread == NULL)
		return 0;

	hProcess = GetCurrentProcess();
	if (!DuplicateHandle(hProcess, hTread, hProcess, &hDupHandle, THREAD_QUERY_INFORMATION, FALSE, 0)){

		SetLastError(ERROR_ACCESS_DENIED);

		return 0;

	}

	ntStatus = NtQueryInformationThread(hDupHandle, (THREADINFOCLASS)9, &dwStartAddress, sizeof(DWORD), NULL);
	CloseHandle(hTread);
	CloseHandle(hProcess);

	CloseHandle(hDupHandle);

	if (ntStatus != 0)
		return 0;

	return dwStartAddress;
}