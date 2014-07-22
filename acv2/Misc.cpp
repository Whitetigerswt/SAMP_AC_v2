#include "Misc.h"
#include "md5.h"
#include "CLog.h"

namespace Misc
{
	std::string MD5_Memory(int address, int size)
	{
		if (size > 256) return "Size to large";

		// Create variable to hold the string at this address.
		char mem[256];

		// Unprotect the memory
		VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, NULL);

		// Copy the memory at the specified address to our buffer
		memcpy((void*)mem, (void*)address, size);

		// Now let's calculate the MD5 hash.
		char* hash;
		MD5 md5 = MD5();
		hash = md5.digestString(mem);

		// convert it to an std::string finally
		std::string result(hash);

		// and return the result.
		return result;
	}



}