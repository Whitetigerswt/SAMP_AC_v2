#include "Misc.h"
#include "../Shared/MD5_Info/md5.h"
#include "Addresses.h"

#include <winternl.h>
#include <TlHelp32.h>
#include <Windows.h>
#include <Psapi.h>

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

	std::wstring GetGTADirectory()
	{
		// Create a new variable to hold the location to gta_sa.exe - but not the directory.
		wchar_t exe_path[MAX_PATH + 1];
		
		// Get the path of the gta_sa.exe and place it into our exe_path variable.
		GetModuleFileName(NULL, exe_path, sizeof(exe_path));

		// Change it to an std::string.
		std::wstring szExePath(exe_path);

		// Find the last instances of \, so we can remove everything after it and have our GTA directory.
		int i = szExePath.rfind(L"\\");

		// Create a new variable to hold the directory path after we've cut out the gta_sa.exe part.
		std::wstring DirectoryPath;
		
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

	int CrouchBug = 9999;

	void SetCrouchBug(int toggle)
	{
		CrouchBug = toggle;
	}

	int GetCrouchBug()
	{
		return CrouchBug;
	}

	bool MacroLocks = true;

	void SetMacroLocks(bool toggle)
	{
		MacroLocks = toggle;
	}

	bool GetMacroLocks()
	{
		return MacroLocks;
	}

	float SprintSpeed = 0.0f;

	void SetSprintLimit(float speed)
	{
		SprintSpeed = speed;
	}

	float GetSprintLimit()
	{
		return SprintSpeed;
	}

	bool SprintOnAllSurfaces = false;

	void SetSprintOnAllSurfaces(bool toggle)
	{
		SprintOnAllSurfaces = toggle;
	}

	bool GetSprintOnAllSurfaces()
	{
		return SprintOnAllSurfaces;
	}

	bool VehicleBlips = true;

	void SetVehicleBlips(bool toggle)
	{
		VehicleBlips = toggle;
	}

	bool GetVehicleBlips()
	{
		return VehicleBlips;
	}

	int GetGameVersion() {
		unsigned char ucA = *reinterpret_cast < unsigned char* > (0x748ADD);
		unsigned char ucB = *reinterpret_cast < unsigned char* > (0x748ADE);
		if (ucA == 0xFF && ucB == 0x53)
		{
			return 1; // usa
		}
		else if (ucA == 0x0F && ucB == 0x84)
		{
			return 2; // eu
		}
		else if (ucA == 0xFE && ucB == 0x10)
		{
			return 3;
		}
		else
		{
			return -1;
		}

		return -1;
	}

	DWORD FindPattern(char *pattern, char *mask)
	{
#ifdef WIN32
		MODULEINFO mInfo = { 0 };

		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mInfo, sizeof(MODULEINFO));

		DWORD base = (DWORD)mInfo.lpBaseOfDll;
		DWORD size = (DWORD)mInfo.SizeOfImage;
#else

#endif

		DWORD patternLength = (DWORD)strlen(mask);

		for (DWORD i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (DWORD j = 0; j < patternLength; j++)
			{
				found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
			}

			if (found)
			{
				return base + i;
			}
		}

		return NULL;
	}

	bool AltTabbed = false;

	void SetAltTabState(bool toggle)
	{
		AltTabbed = toggle;
	}

	bool GetAltTabState()
	{
		return AltTabbed;
	}

	// Convert a wide Unicode string to an UTF8 string
	std::string utf8_encode(const std::wstring &wstr)
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	// Convert an UTF8 string to a wide Unicode String
	std::wstring utf8_decode(const std::string &str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}
}