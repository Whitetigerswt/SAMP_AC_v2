#include <windows.h>
#include <cstdio>
#include <iostream>

int main(int argc, char* argv[])
{
	std::string ExeFile(argv[1]);

	int i = ExeFile.rfind("\\");

	std::string path = ExeFile.substr(0, i + 1);

	PROCESS_INFORMATION ProcessInfo;

	STARTUPINFO StartupInfo;

	memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));

	if (CreateProcess(argv[1], GetCommandLineA(), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, path.c_str(), &StartupInfo, &ProcessInfo))
	{
		char szWithSampdll[256] = "";
		sprintf_s(szWithSampdll, sizeof(szWithSampdll), "%s\\samp.dll", path);

		void* addr = (void*)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
		void* arg = (void*)VirtualAllocEx(ProcessInfo.hProcess, NULL, strlen(szWithSampdll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(ProcessInfo.hProcess, arg, szWithSampdll, strlen(szWithSampdll), NULL);
		HANDLE id = CreateRemoteThread(ProcessInfo.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, CREATE_SUSPENDED, NULL);

		ResumeThread(id);
		WaitForSingleObject(id, INFINITE);
		VirtualFreeEx(ProcessInfo.hProcess, arg, strlen(szWithSampdll), MEM_RELEASE);

		ResumeThread(ProcessInfo.hThread);

		CloseHandle(ProcessInfo.hProcess);

	}
	return 0;
}

