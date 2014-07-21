#include "CRPCCallback.h"
#include "../CLog.h"
#include "../CLoader.h"

#include <Boost\thread.hpp>


void CRPCCallback::Initialize()
{

	boost::thread ResendFilesThread(&ResendFileInformation);
}

void CRPCCallback::ResendFileInformation()
{
	CLoader::Processes.ResendFiles();
	CLoader::Modules.ResendFiles();
}