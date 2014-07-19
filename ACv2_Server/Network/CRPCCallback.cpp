#include "CRPCCallback.h"
#include "../Utility.h"
#include "Callback.h"
#include "Network.h"
#include "../CPlayer.h"

#include <string>

void CRPCCallback::Initialize()
{
	CRPC::Add(eRPC::ON_PROCESS_STARTED, OnProcessStarted);
}


RPC_CALLBACK CRPCCallback::OnProcessStarted(RakNet::BitStream& bsData, int iExtra)
{
	unsigned char processpath[256];
	unsigned char md5[256];

	memset(processpath, 0, sizeof(processpath));
	memset(md5, 0, sizeof(md5));

	if (bsData.Read((char*)processpath) && bsData.Read((char*)md5))
	{
		Network::GetPlayerFromPlayerid(iExtra)->OnProcessStarted((char*)processpath, (char*)md5);
	}
}