#pragma once

#include "../../Shared/Network/CRPC.h"


class CRPCCallback
{
public:
	
	// PURPOSE: Load the RPC's and link them to an RPC Callback.
	// REQUIRES: NULL
	static void Initialize();

private:
	// PURPOSE: RPC Callback for eRPC::ON_FILE_EXECUTED
	// REQUIRES: NULL
	static RPC_CALLBACK OnFileExecuted(RPC_ARGS);

	// PURPOSE: RPC Callback for eRPC::ON_MD5_CALCULATED
	// REQUIRES: NULL
	static RPC_CALLBACK OnMD5Calculated(RPC_ARGS);

};
