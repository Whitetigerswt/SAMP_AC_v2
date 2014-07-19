#include "CLoader.h"
#include "../Shared/Network/Network.h"
#include "CParseCommandLine.h"
#include "Network/Network.h"
#include "CLog.h"
#include "CProcessList.h"

#include <map>
#include <Detours/detours.h>

void CLoader::Initialize()
{

	std::map < std::string, std::string > cmdline;
	cmdline = CParseCommandLine::parseCmdLine(GetCommandLineA());

	Network::Initialize(cmdline["Host"], atoi(cmdline["Port"].c_str()) - 1);
	Network::Connect();

	CDirectX::HookD3DFunctions();

	CProcessList Processes = CProcessList();
	while (true)
	{
		Processes.Scan();
		Sleep(1000);
	}
}

