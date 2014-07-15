#pragma once
#include <fstream>
#include <string>

class CLog {
public:
	CLog(char* filename);
	~CLog();
	void Write(std::string logline);
	void Write(std::string logline, int notime);
private:
	std::ofstream m_stream;
};