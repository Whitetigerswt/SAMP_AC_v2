#pragma once

#include <map>
#include "../RakNet/BitStream.h"

#define RPC_CALLBACK void
#define RPC_ARGS RakNet::BitStream& bsData, int iExtra

typedef RPC_CALLBACK(*RPCFunct_t)(RPC_ARGS);

enum eRPC : unsigned short
{
	// -- CLIENT RPCs --

	// -- SERVER RPCs --
	ON_PROCESS_STARTED

};

class CRPC
{
public:
	static void Add(unsigned short usRPCId, RPCFunct_t func);
	static void Process(unsigned short usRPCId, RakNet::BitStream& bsData, int iExtra = NULL);

private:
	static std::map<unsigned short, RPCFunct_t> m_functions;

};
