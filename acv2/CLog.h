#pragma once
#include <fstream>
#include <string>

<<<<<<< HEAD
#define AC_LOG_FILE_PATH	"acv2_log.txt"

=======
>>>>>>> parent of b0562b3... Addendum to c1349c6bfb18535d30f6869b351d7a7f6cd03b11
class CLog {
public:

	// PURPOSE: Open a log file and prepare to write to it.
	// REQUIERS: NULL
	CLog(char* filename);

	// PURPOSE: Open a log file and prepare to write to it.
	// REQUIERS: NULL
	CLog(char* filename, bool append);
	~CLog();

	// PURPOSE: Write to the log file.
	// REQUIRES: NULL
	void Write(std::string logline);
	void Write(const char* format, ...);
private:
	std::ofstream m_stream;
};