#include "CProcessList.h"

#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

typedef void (WINAPI *QFPIN)(HANDLE hProcess, DWORD dwFlags, LPWSTR lpExeName, PDWORD lpdwSize);


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
	QFPIN pQueryFullProcessImageName = (QFPIN)GetProcAddress(GetModuleHandle((LPCWSTR)"Kernel32.dll"), "QueryFullProcessImageNameW");

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
			// Ignore the first process as it's always bullshit (non english characters, and NULL md5.)
			while (Process32Next(hProcessSnap, &pe32))
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
					pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, false, pe32.th32ProcessID);
				} 
				else
				{
					pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, false, pe32.th32ProcessID);
				}

				if (pHandle != NULL)
				{
					// Create a char object to hold the path of the process later.
					wchar_t processpath[MAX_PATH];

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
					// The follow code checks if this process really exists on a drive
					// If you wonder why this is important, see https://github.com/Whitetigerswt/SAMP_AC_v2/issues/134

					// Get last error code
					DWORD err = GetLastError();

					// Check if error code means, "The volume for a file has been externally altered so that the opened file is no longer valid"
					if (err == ERROR_FILE_INVALID)
					{
						// It's an invalid file. It doesn't seem to exist on a drive anymore. So it probably was a bypass attempt..

						// Kill the process
						TerminateProcess(pHandle, 0);

						// Make sure we close the handle to the open process.
						CloseHandle(pHandle);

						// Continue on to the next process.
						continue;
					}
					if (processpath != NULL) 
					{
						std::wstring path(processpath);

						if (!DoesFileExist(path))
						{
							// Add process to the process list.
							AddFile(path);
						}
					}
					
					// Make sure we close the handle to the open process.
					CloseHandle(pHandle);
				}
				// Continue on to the next process.
			}
		}
	}
}

void CProcessList::AdjustTokens(HANDLE process) {

	// Create our variables to be used in the function.
	TOKEN_PRIVILEGES token_privileges;
	DWORD dwSize;

	// Make sure the memory is empty.
	ZeroMemory(&token_privileges, sizeof(token_privileges));

	// Make sure the HANDLE passed as a parameter is valid.
	if (process != NULL)
	{
		// Create a handle
		HANDLE hToken;

		// Open the process, so we can change the tokens.
		OpenProcessToken(process, TOKEN_ALL_ACCESS, &hToken);

		// Check the current value of SE_DEBUG_NAME
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &token_privileges.Privileges[0].Luid);

		// Set the SE_DEBUG_NAME value to true.
		AdjustTokenPrivileges(hToken, FALSE, &token_privileges, 0, NULL, &dwSize);

		// Close the hToken handle, but not the process handle!
		CloseHandle(hToken);
	}
}