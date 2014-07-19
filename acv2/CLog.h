#pragma once
#include <fstream>
#include <string>

class CLog {
public:
	CLog(char* filename);
	~CLog();
	void Write(std::string logline);
	//void Write(std::string logline, int notime);
	void Write(const char* format, ...);
private:
	std::ofstream m_stream;
};