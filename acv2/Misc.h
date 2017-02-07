#include <Windows.h>
#include <string>
#include <vector>

namespace Misc
{
	// PURPOSE: MD5 a portion of memory
	// REQUIRES: Size less than 256.
	std::string MD5_Memory(int address, int size);

	// PURPOSE: Get gta sa directory path, not necissarily the location where the .asi plugin is.
	// REQUIRES: NULL
	std::wstring GetGTADirectory();

	// PURPOSE: Get the players frame limit.
	// REQUIRES: NULL
	int GetFPSLimit();

	// PURPOSE: Set the players frame limit.
	// REQUIRES: A new frame limit int
	void SetFPSLimit(int newlimit);

	// PURPOSE: Set the players crouch bug status.
	// REQUIRES: NULL
	void SetCrouchBug(int toggle);

	// PURPOSE: Get the crouch bug status.
	// REQUIRES: NULL
	int GetCrouchBug();

	// PURPOSE: Set the players macro lock status.
	// REQUIRES: NULL
	void SetMacroLocks(bool toggle);

	// PURPOSE: Get the macro lock status.
	// REQUIRES: NULL
	bool GetMacroLocks();

	// PURPOSE: Set the player's sprint speed limit.
	// REQUIRES: NULL
	void SetSprintLimit(float speed);

	// PURPOSE: Get the sprint speed limit.
	// REQUIRES: NULL
	float GetSprintLimit();

	// PURPOSE: Set the sprinting on all surfaces status.
	// REQUIRES: NULL
	void SetSprintOnAllSurfaces(bool toggle);

	// PURPOSE: Get the sprinting on all surfaces status.
	// REQUIRES: NULL
	bool GetSprintOnAllSurfaces();

	// PURPOSE: Get the players GTA SA version
	// REQUIRES: NULL
	int GetGameVersion();

	// PURPOSE: Set vehicle blips status
	// REQUIRES: NULL
	void SetVehicleBlips(bool toggle);

	// PURPOSE: Get whether vehicle blips are enabled or not.
	// REQUIRES: NULL
	bool GetVehicleBlips();

	// PURPOSE: Get an address inside a potentially updated .dll
	// REQUIRES: A pattern.
	// REQUIRES: A mask.
	DWORD FindPattern(char *pattern, char *mask);

	// PURPOSE: Get an address from a pattern inside an module.
	// REQUIRES: A pattern.
	// REQUIRES: A mask.
	// REQUIRES: A base address of the module you want to search.
	// REQUIRES: The total size of the module.
	//DWORD FindPattern(char *pattern, char *mask, DWORD base, DWORD size);

	// PURPOSE: Set the games state as alt tabbed
	// REQUIRES: a state to set
	void SetAltTabState(bool toggle);

	// PURPOSE: Get the alt tab state of the game.
	// REQUIRES: NULL
	bool GetAltTabState();

	std::string utf8_encode(const std::wstring &wstr);
	std::wstring utf8_decode(const std::string &str);
}