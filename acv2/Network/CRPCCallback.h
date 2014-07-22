#pragma once

#include "../../Shared/Network/CRPC.h"

class CRPCCallback
{
public:
	static void Initialize();

private:
	static void ResendFileInformation();
	static RPC_CALLBACK MD5_Memory_Region(RPC_ARGS);

};