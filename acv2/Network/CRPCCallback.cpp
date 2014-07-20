#include "CRPCCallback.h"
#include "../CLog.h"
#include "../CLoader.h"


void CRPCCallback::Initialize()
{
	CLoader::Processes.ResendFiles();
	CLoader::Modules.ResendFiles();
}