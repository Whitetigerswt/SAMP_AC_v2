#include "CProcessList.h"

#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include "CLog.h"

typedef void (WINAPI *QFPIN)(HANDLE hProcess, DWORD dwFlags, LPSTR lpExeName, PDWORD lpdwSize);


CProcessList::CProcessList()
{
	AdjustTokens(GetCurrentProcess());
}


CProcessList::~CProcessList()
{

}



void CProcessList::Scan()
{

	// Check if QueryFullProcessImageName exists.
	// It doesn't exist on Windows XP.
	QFPIN pQueryFullProcessImageName = (QFPIN)GetProcAddress(GetModuleHandle("Kernel32.dll"), (LPCSTR)"QueryFullProcessImageNameA");

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe32;

	// Get a snapchat of the processes running
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		pe32.dwSize = sizeof(PROCESSENTRY32);
		// Make sure the process list is valid and at least one process exists.
		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				// Create handle object to get the processid later.
				HANDLE pHandle = NULL;
				
				// Open the process and get a handle to it so we can call QueryFullProcessImageName,
				// or GetModuleFileNameEx if we're on windows XP.
				HANDLE process = OpenProcess(TOKEN_ADJUST_PRIVILEGES, FALSE, pe32.th32ProcessID);
				AdjustTokens(process);
				CloseHandle(process);
				

				// If we're on windows other than XP, we only need PROCESS_QUERY_INFORMATION priv.
				if (pQueryFullProcessImageName != NULL)
				{
					pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pe32.th32ProcessID);
				} 
				else
				{
					pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pe32.th32ProcessID);
				}

				if (pHandle != NULL)
				{
					// Create a char object to hold the path of the process later.
					char processpath[256];

					if (NULL != pQueryFullProcessImageName) 
					{
						// Create a variable to store the process path size
						DWORD size = (DWORD)sizeof(processpath);

						// Call QueryFullProcessImageName since we're not on windows XP.
						pQueryFullProcessImageName(pHandle, 0, processpath, &size);
					} 
					else
					{
						// We're on Windows XP.
						GetModuleFileNameEx(pHandle, NULL, processpath, sizeof(processpath));
					}

					if (processpath != NULL) 
					{
						std::string path(processpath);
						if (!DoesFileExist(path))
						{
							// Add process to the process list.
							AddFile(path);
						}
					}
				}
				
				// Make sure we close the handle to the open process.
				CloseHandle(pHandle);
			// Continue on to the next process.
			} while (Process32Next(hProcessSnap, &pe32));
		}
	}
}

void CProcessList::AdjustTokens(HANDLE process) {

	HANDLE hToken;
	TOKEN_PRIVILEGES token_privileges;
	DWORD dwSize;

	ZeroMemory(&token_privileges, sizeof(token_privileges));

	if (process != NULL)
	{
		OpenProcessToken(process, TOKEN_ALL_ACCESS, &hToken);
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &token_privileges.Privileges[0].Luid);
		AdjustTokenPrivileges(hToken, FALSE, &token_privileges, 0, NULL, &dwSize);
	}

	CloseHandle(hToken);
}