#include <Windows.h>
#include <string>


namespace Misc
{
	// PURPOSE: MD5 a portion of memory
	// REQUIRES: Size less than 256.
	std::string MD5_Memory(int address, int size);

	// PURPOSE: Get gta sa directory path, not necissarily the location where the .asi plugin is.
	// REQUIRES: nothing.
	std::string GetGTADirectory();
}