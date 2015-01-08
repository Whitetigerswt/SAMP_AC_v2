#pragma once

#include <map>
#include <list>

#include "../../Shared/Network/Network.h"
#include "../CAntiCheat.h"
#include <RakNet/BitStream.h>

namespace Network
{
	// PURPOSE: Initialize the network.
	// REQUIRES: Pointer of data sent at plugin load
	// PROMISE: The network will be initialized.
	void Initialize(void **ppData);

	// PURPOSE: Check if the network is initialized.
	// REQUIRES: NULL
	bool IsInitialized();

	// PURPOSE: Get a list of CAntiCheat pointers of all players. (with their ids)
	// REQUIRES: NULL
	std::map<unsigned int, CAntiCheat*>& GetPlayers();

	// PURPOSE: Get a players CAntiCheat pointer from their playerid.
	// REQUIRES: a playerid.
	CAntiCheat* GetPlayerFromPlayerid(unsigned int uiPlayerid);

	// PURPOSE: Check if the player is connected to the AC.
	// REQUIRES: A playerid.
	bool IsPlayerConnectedToAC(unsigned int uiPlayerid);

	// PURPOSE: Cleanup a connection from a playerid.
	// REQUIRES: A playerid.
	// REQUIRES: A disconnection type
	void Cleanup(unsigned int uiPlayerid, int type);

	// PURPOSE: Send a packet to a specified playerid.
	// REQUIRES: A Packet Type.
	// REQUIRES: A playerid.
	unsigned int PlayerSend(unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, int priority = 1, int reliability = 9, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to a playerid.
	// REQUIRES: an RPC id.
	// REQUIRES: a playerid.
	unsigned int PlayerSendRPC(int usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, int priority = 1, int reliability = 9, char cOrderingChannel = 0x7F);

	// PURPOSE: Process packets sent to the server.
	// REQUIRES: The server is initialized.
	void Process();

	// temp
	int HandleConnection(int playerid);
}
