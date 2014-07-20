#include "CDirectX.h"

#define ADDRESS_LOADED *(int*)0xC8D4C0

class CLoader 
{
public:

	// PURPOSE: Initialize main program.
	// REQUIRES: NULL
	// PROMISE: Program main facilities will be initiated.
	static void Initialize();

private:
	// PURPOSE: Check elevation of the program, and elevate if necessary.
	// REQUIRES: NULL
	// PROMISE: An elevated process after it returns.
	static void CheckElevation();

	// PURPOSE: Determine if the process is elevated.
	// REQUIRES: NULL
	static BOOL IsProcessElevated();
};