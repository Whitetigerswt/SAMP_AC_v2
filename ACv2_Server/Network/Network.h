#pragma once

#include <map>
#include <list>

#include "../../Shared/Network/Network.h"
#include "../CAntiCheat.h"
#include "../../Shared/Network/BitStream.h"

namespace Network
{
	// PURPOSE: Initialize the network.
	// REQUIRES: Pointer of data sent at plugin load
	// PROMISE: The network will be initialized.
	void Initialize(void **ppData);

	// PURPOSE: Send a packet to a specified playerid.
	// REQUIRES: A Packet Type.
	// REQUIRES: A playerid.
	unsigned int PlayerSend(unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, int priority = 1, int reliability = 9, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to a playerid.
	// REQUIRES: an RPC id.
	// REQUIRES: a playerid.
	unsigned int PlayerSendRPC(int usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, int priority = 1, int reliability = 9, char cOrderingChannel = 0x7F);

}
