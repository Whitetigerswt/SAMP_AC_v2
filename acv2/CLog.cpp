#include "CLog.h"
#include <ctime>

CLog::CLog(char* filename) {
	m_stream.open(filename, std::ofstream::out | std::ofstream::app);
}

void CLog::Write(std::string logline){
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	m_stream << "[" << (tm->tm_year + 1900) << '/' << (tm->tm_mon + 1) << '/' << tm->tm_mday
		<< ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << "]: ";
	m_stream << logline << std::endl;
}

void CLog::Write(std::string logline, int notime){
	m_stream << logline;
}

CLog::~CLog(){
	m_stream.close();
}