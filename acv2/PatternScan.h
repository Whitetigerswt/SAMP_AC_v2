#include <Windows.h>

// PURPOSE: Get an address from a pattern inside an module.
// REQUIRES: A pattern.
// REQUIRES: A mask.
// REQUIRES: A base address of the module you want to search.
// REQUIRES: The total size of the module.
DWORD FindPattern(char *pattern, char *mask, DWORD base, DWORD size);

//DWORD FindPattern(char *pattern, char *mask);