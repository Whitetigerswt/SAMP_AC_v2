#include "stdafx.h"
#include <windows.h>

// The purpose of this program is just to elevate gta_sa.exe if it's not elevated.
// This program will be launched by the *.ASI mod in this solution if gta_sa.exe is not elevated.
// This *.EXE is expected to be placed inside the GTA_SA directory so it can be accessed by the *.ASI mod.

#include <iostream>

// entry point
int main(int argc, char* argv[])
{
	// our .ASI mod copys the parameters of gta_sa.exe that is running.
	// and pastes it into the command line of this program.
	// so our command line in this program will look something like:
	// "C:\Program Files\Rockstar Games\GTA San Andreas\gta_sa.exe" -c -h 127.0.0.1 -p 7777 -n Whitetiger
	// argv[0] -> this is the current process path. i.e: samp_elevator.exe, not gta_sa.exe.
	// argv[1] is the GTA_SA.exe path.
	// argv[2] in this example, would be "-c".
	// however, we don't bother parsing all the command line since it's not necessary, we can just paste another GetCommandLine.
	// and SA:MP is smart enough to skip passed the samp_elevator.exe path, and go to the -h -p etc for host and port.

	// Turn the gta_sa.exe path into an std::string instead of a C char.
	std::string ExeFile(argv[1]);

	// Find the last instance of a \, so we can remove gta_sa.exe and just get the path
	int i = ExeFile.rfind("\\");
	std::string path = ExeFile.substr(0, i + 1);

	// Prepare to create a new process.
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));

	// Create a new process, but don't let it run yet, it's suspended.
	if (CreateProcess(argv[1], GetCommandLineA(), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, path.c_str(), &StartupInfo, &ProcessInfo))
	{
		// Create a new string that will hold the path to the file samp.dll
		char szWithSampdll[256] = "";
		sprintf_s(szWithSampdll, sizeof(szWithSampdll), "%s\\samp.dll", path.c_str());

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
			MessageBoxA(NULL, "Could not find kernel32.dll", "SA:MP Elevator", MB_ICONERROR);
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
			MessageBoxA(NULL, "Memory could not be allocated to inject samp.dll", "SA:MP Elevator", MB_ICONERROR);
			return 0;
		}

		// Create new handle to our remote thread.
		HANDLE id = NULL;

		// Make sure The address of LoadLibrary isn't NULL
		if (addr != NULL)
			// Create a remote thread that calls LoadLibrary, and as the parameter, the memory location we just wrote the samp.dll path to.
			// also don't execute this thread, but just create.
			id = CreateRemoteThread(ProcessInfo.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, CREATE_SUSPENDED, NULL);
		else
		{
			MessageBoxA(NULL, "Could not find the address of LoadLibraryA", "SA:MP Elevator", MB_ICONERROR);
			return 0;
		}

		// Make sure id is a valid handle
		if (id)
		{
			// Resume the remote thread.
			ResumeThread(id);

			// Wait for the remote thread to finish executing.
			WaitForSingleObject(id, INFINITE);
		}
		else
		{
			MessageBoxA(NULL, "the ID returned from CreateRemoteThread was invalid.", "SA:MP Elevator", MB_ICONERROR);
			return 0;
		}

		// Free the memory we just allocated that stores the samp.dll file path since LoadLibrary has been called and it's not needed anymore.
		VirtualFreeEx(ProcessInfo.hProcess, arg, 0, MEM_RELEASE);

		// Resume the process (It was suspended, remember?)
		ResumeThread(ProcessInfo.hThread);

		// Close the handle to the process we created.
		CloseHandle(ProcessInfo.hProcess);

	}
	
	// We're done since all we needed to do was create a process, and assuming that this .exe was run under elevation (which it's required in the manifest)
	// then gta_sa.exe will also be running under elevation.
	return 0;
}

