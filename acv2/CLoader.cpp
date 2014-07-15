#include "CLoader.h"
#include <map>
#include "../Shared/Network/Network.h"
#include "CParseCommandLine.h"
#include "Network/Network.h"

void CLoader::Initialize()
{
	std::map < std::string, std::string > cmdline;
	cmdline = CParseCommandLine::parseCmdLine(GetCommandLineA());
	Network::Initialize(cmdline["Host"], atoi(cmdline["Port"].c_str()) - 1);
	Network::Connect();
}