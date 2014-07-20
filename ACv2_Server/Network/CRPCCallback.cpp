#include "CRPCCallback.h"
#include "../Utility.h"
#include "Callback.h"
#include "Network.h"
#include "../CPlayer.h"

#include <string>

void CRPCCallback::Initialize()
{
	// Add RPC Callback functions.
	CRPC::Add(eRPC::ON_FILE_EXECUTED, OnFileExecuted);
}


RPC_CALLBACK CRPCCallback::OnFileExecuted(RakNet::BitStream& bsData, int iExtra)
{
	// Allocate space for the process path and md5 parameters.
	unsigned char processpath[256];
	unsigned char md5[256];

	// Reset those variables values.
	memset(processpath, 0, sizeof(processpath));
	memset(md5, 0, sizeof(md5));

	// Read new values for those variables.
	if (bsData.Read((char*)processpath) && bsData.Read((char*)md5))
	{
		// Call the main OnFileExecuted function.
		Network::GetPlayerFromPlayerid(iExtra)->OnFileExecuted((char*)processpath, (char*)md5);
	}
}