#include <vector>
#include <map>
#include <string>

class CParseCommandLine {

public:
	static std::map <std::string, std::string> parseCmdLine(std::string CommandLine);
private:
	static std::map<std::string, std::string> CmdLine;
};