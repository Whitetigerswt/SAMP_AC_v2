#pragma once
#include <fstream>
#include <string>

#define AC_LOG_FILE_NAME	"acv2_log.txt"

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