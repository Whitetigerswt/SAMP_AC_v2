#include <windows.h>

// The purpose of this program is just to show the basics of a custom launcher that people can make in SA-MP.

#include <iostream>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Shlwapi.h>
#include <tchar.h>

BOOL SetDebugPrivilege()
{
	BOOL bRet = FALSE;
	HANDLE hToken = NULL;
	LUID luid = { 0 };

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			TOKEN_PRIVILEGES tokenPriv = { 0 };
			tokenPriv.PrivilegeCount = 1;
			tokenPriv.Privileges[0].Luid = luid;
			tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			bRet = AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		}
	}

	return bRet;
}

void KillProcessByName(TCHAR *filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (StrCmpI(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				WaitForSingleObject(hProcess, INFINITE);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

// entry point
int main(int argc, char* argv[])
{
	// Set debug privilege (so we can close other processes)
	SetDebugPrivilege();

	// Kill all gta_sa.exe processes
	KillProcessByName(TEXT("gta_sa.exe"));

	// Prepare to create a new process.
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));

	wchar_t exeLocation[256];
	DWORD buffer = sizeof(exeLocation);

	// Open registry key
	HKEY hKey;
	long lError = RegOpenKeyEx(HKEY_CURRENT_USER,
		L"Software\\SAMP",
		0,
		KEY_READ,
		&hKey);

	// Get value
	DWORD dwRet = RegQueryValueEx(hKey, L"gta_sa_exe", NULL, NULL, (LPBYTE)&exeLocation, &buffer);

	// Make sure we got a good value for the gta_sa path
	if (dwRet != ERROR_SUCCESS)
	{
		MessageBox(NULL, L"Could not get the location of your GTA:SA installation. Is SA-MP installed correctly?", L"SA:MP Launcher", MB_ICONERROR);
		return 0;
	}

	// remove \gta_sa.exe in a new variable (leaving just the directory path)
	wchar_t path[256];
	_tcscpy_s(path, sizeof(path), exeLocation);
	path[_tcslen(path) - 11] = '\0';

	// Create a new process, but don't let it run yet, it's suspended.
	if (CreateProcess(exeLocation, GetCommandLine(), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, path, &StartupInfo, &ProcessInfo))
	{
		// Create a new string that will hold the path to the file samp.dll
		wchar_t szWithSampdll[256] = L"";
		_stprintf_s(szWithSampdll, sizeof(szWithSampdll), L"%ls\\samp.dll", path);

		// Get the module handle to kernal32.dll
		HMODULE hMod = GetModuleHandle(L"kernel32.dll");

		// Create address variable to hold the address of the LoadLibrary function.
		void* addr = NULL;

		// If it was a valid handle.
		if (hMod)
			// Get the address of the LoadLibrary function so we can load samp.dll
			addr = (void*)GetProcAddress(hMod, "LoadLibraryW");
		else
		{
			MessageBox(NULL, L"Could not find kernel32.dll", L"SA:MP Launcher", MB_ICONERROR);
			return 0;
		}

		// Allocate memory in the new process we just created to store the string of the samp.dll file path.
		void* arg = (void*)VirtualAllocEx(ProcessInfo.hProcess, NULL, _tcslen(szWithSampdll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// Make sure the space was allocated.
		if (arg != NULL)
			// Write to the memory we just allocated the file path to samp.dll including directory.
			WriteProcessMemory(ProcessInfo.hProcess, arg, szWithSampdll, _tcslen(szWithSampdll), NULL);
		else
		{
			// arg is null, and we can't continue then.
			// Let the user know there was a problem and exit.
			MessageBox(NULL, L"Memory could not be allocated to inject samp.dll", L"SA:MP Launcher", MB_ICONERROR);
			return 0;
		}

		// Create new handle to our remote thread.
		HANDLE id = NULL;

		// Make sure The address of LoadLibrary isn't NULL
		if (addr != NULL)
		{
			// Create a remote thread that calls LoadLibrary, and as the parameter, the memory location we just wrote the samp.dll path to.
			// also don't execute this thread, but just create.
			id = CreateRemoteThread(ProcessInfo.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, CREATE_SUSPENDED, NULL);
		}
		else
		{
			MessageBox(NULL, L"Could not find the address of LoadLibraryA", L"SA:MP Launcher", MB_ICONERROR);
			return 0;
		}

		// Make sure id is a valid handle
		if (id)
		{
			// Resume the remote thread.
			ResumeThread(id);

			std::cout << "...patience..." << std::endl;

			// Wait for the remote thread to finish executing.
			WaitForSingleObject(id, INFINITE);
		}
		else
		{
			MessageBox(NULL, L"the ID returned from CreateRemoteThread was invalid.", L"SA:MP Launcher", MB_ICONERROR);
			return 0;
		}

		// Free the memory we just allocated that stores the samp.dll file path since LoadLibrary has been called and it's not needed anymore.
		VirtualFreeEx(ProcessInfo.hProcess, arg, 0, MEM_RELEASE);

		// Resume the process (It was suspended, remember?)
		ResumeThread(ProcessInfo.hThread);

		// Close the handle to the process we created.
		CloseHandle(ProcessInfo.hProcess);

	}

	// Done!
	return 0;
}

