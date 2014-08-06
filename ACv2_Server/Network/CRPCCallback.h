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

	// PURPOSE: RPC Callback for eRPC::ON_FILE_CALCULATED
	// REQUIRES: NULL
	static RPC_CALLBACK OnFileCalculated(RPC_ARGS);

	// PURPOSE: RPC Callback for eRPC::ON_IMG_FILE_MODIFIED.
	// REQUIRES: NULL
	static RPC_CALLBACK OnImgFileModified(RPC_ARGS);

	// PURPOSE: RPC Callback for eRPC::ON_MACRO_DETECTED
	// REQUIRES: NULL
	static RPC_CALLBACK OnMacroDetected(RPC_ARGS);

	// PURPOSE: RPC Callback for eRPC::ON_HARDWAREID_SENT	
	// REQUIRES: NULL
	static RPC_CALLBACK OnHardwareIDGotten(RPC_ARGS);

	// PURPOSE: RPC Callback for eRPC::ON_TAMPER_ATTEMPT
	// REQUIRES: NULL
	static RPC_CALLBACK OnTamperAttempt(RPC_ARGS);

};
