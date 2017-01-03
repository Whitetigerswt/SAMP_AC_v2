#include <ctype.h>

unsigned long GTASA_CRC32_fromString(const char* string);

unsigned long GTASA_CRC32_fromUpCaseString(const char* string);

unsigned long GTASA_CRC32_fromBlock(const void* ptr, size_t len);