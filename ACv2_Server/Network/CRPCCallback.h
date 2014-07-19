#pragma once

#include "../../Shared/Network/CRPC.h"


class CRPCCallback
{
public:
	static void Initialize();

	static RPC_CALLBACK OnProcessStarted(RPC_ARGS);

};
