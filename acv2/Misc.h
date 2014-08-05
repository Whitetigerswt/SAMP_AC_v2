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
	std::string GetGTADirectory();

	// PURPOSE: Get the players frame limit.
	// REQUIRES: NULL
	int GetFPSLimit();

	// PURPOSE: Set the players frame limit.
	// REQUIRES: A new frame limit int
	void SetFPSLimit(int newlimit);
}