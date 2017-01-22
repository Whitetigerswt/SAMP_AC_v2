#include <windows.h>

// The purpose of this program is just to show the basics of a custom launcher that people can make in SA-MP.

#include <iostream>

// entry point
int main(int argc, char* argv[])
{
	// Prepare to create a new process.
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));

	// Tell the user to enter an IP.
	/*std::cout << "Please enter the IP you would like to connect to.\n";

	// Get the IP.
	char ip[24];
	std::cin >> ip;

	// Tell the user to enter a port
	std::cout << "Please enter the port.\n";

	// Get the port they typed.
	int port;
	std::cin >> port;
	*/
	// Get the user's gta_sa location
	char exeLocation[256];
	DWORD buffer = sizeof(exeLocation);

	// Open registry key
	HKEY hKey;
	long lError = RegOpenKeyEx(HKEY_CURRENT_USER,
		"Software\\SAMP",
		0,
		KEY_READ,
		&hKey);

	// Get value
	DWORD dwRet = RegQueryValueEx(hKey, "gta_sa_exe", NULL, NULL, (LPBYTE)&exeLocation, &buffer);

	// Make sure we got a good value for the gta_sa path
	if (dwRet != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Could not get the location of your GTA:SA installation. Is SA-MP installed correctly?", "SA:MP Launcher", MB_ICONERROR);
		return 0;
	}

	// remove \gta_sa.exe in a new variable (leaving just the directory path)
	char path[256];
	strcpy_s(path, sizeof(path), exeLocation);
	path[strlen(path) - 11] = '\0';

	// Create a new process, but don't let it run yet, it's suspended.
	if (CreateProcess(exeLocation, GetCommandLine(), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, path, &StartupInfo, &ProcessInfo))
	{
		// Create a new string that will hold the path to the file samp.dll
		char szWithSampdll[256] = "";
		sprintf_s(szWithSampdll, sizeof(szWithSampdll), "%s\\samp.dll", path);

		// Get the module handle to kernal32.dll
		HMODULE hMod = GetModuleHandle("kernel32.dll");

		// Create address variable to hold the address of the LoadLibrary function.
		void* addr = NULL;

		// If it was a valid handle.
		if (hMod)
			// Get the address of the LoadLibrary function so we can load samp.dll
			addr = (void*)GetProcAddress(hMod, "LoadLibraryA");
		else
		{
			MessageBoxA(NULL, "Could not find kernel32.dll", "SA:MP Launcher", MB_ICONERROR);
			return 0;
		}

		// Allocate memory in the new process we just created to store the string of the samp.dll file path.
		void* arg = (void*)VirtualAllocEx(ProcessInfo.hProcess, NULL, strlen(szWithSampdll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// Make sure the space was allocated.
		if (arg != NULL)
			// Write to the memory we just allocated the file path to samp.dll including directory.
			WriteProcessMemory(ProcessInfo.hProcess, arg, szWithSampdll, strlen(szWithSampdll), NULL);
		else
		{
			// arg is null, and we can't continue then.
			// Let the user know there was a problem and exit.
			MessageBoxA(NULL, "Memory could not be allocated to inject samp.dll", "SA:MP Launcher", MB_ICONERROR);
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
			MessageBoxA(NULL, "Could not find the address of LoadLibraryA", "SA:MP Launcher", MB_ICONERROR);
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
			MessageBoxA(NULL, "the ID returned from CreateRemoteThread was invalid.", "SA:MP Launcher", MB_ICONERROR);
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

