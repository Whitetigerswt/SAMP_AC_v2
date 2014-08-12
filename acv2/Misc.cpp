#include "Misc.h"
#include "md5.h"
#include "CLog.h"
#include "Addresses.h"

#include <winternl.h>
#include <TlHelp32.h>
#include <Windows.h>

namespace Misc
{
	std::string MD5_Memory(int address, int size)
	{
		// let's calculate the MD5 hash.
		char* hash;
		MD5 md5 = MD5();
		hash = md5.digestMemory((BYTE*)address, size);

		// convert it to an std::string finally
		std::string result(hash);

		// and return the result.
		return result;
	}

	std::string GetGTADirectory()
	{
		// Create a new variable to hold the location to gta_sa.exe - but not the directory.
		char exe_path[MAX_PATH + 1];
		
		// Get the path of the gta_sa.exe and place it into our exe_path variable.
		GetModuleFileName(NULL, exe_path, sizeof(exe_path));

		// Change it to an std::string.
		std::string szExePath(exe_path);

		// Find the last instances of \, so we can remove everything after it and have our GTA directory.
		int i = szExePath.rfind("\\");

		// Create a new variable to hold the directory path after we've cut out the gta_sa.exe part.
		std::string DirectoryPath;
		
		// Put the directory path in our directory path variable.
		DirectoryPath = szExePath.substr(0, i);

		return DirectoryPath;
	}

	// PURPOSE: The variable that holds the players frame limiter.
	int FrameLimit = 9999;

	int GetFPSLimit()
	{
		return FrameLimit;
	}

	void SetFPSLimit(int newlimit)
	{
		FrameLimit = newlimit;
	}

	bool CrouchBug = true;

	void SetCrouchBug(bool toggle)
	{
		CrouchBug = toggle;
	}

	bool GetCrouchBug()
	{
		return CrouchBug;
	}
}