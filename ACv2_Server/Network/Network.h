#pragma once

#include <map>
#include <list>

#include "CRakServer.h"
#include "CClientSocketInfo.h"
#include "../../Shared/Network/Network.h"
#include "../CAntiCheat.h"

namespace Network
{
	// PURPOSE: Initialize the network.
	// REQUIRES: A character pointer to an IP.
	// REQUIRES: A port number.
	// REQUIRES: A maximum number of connections.
	// PROMISE: The network will be initialized.
	void Initialize(const char* szHostAddress, t_port usPort, int iConnections);

	// PURPOSE: Get a pointer to the CRakServer.
	// REQUIRES: NULL
	CRakServer* GetRakServer();

	// PURPOSE: Check if the network is initialized.
	// REQUIRES: NULL
	bool IsInitialized();

	// PURPOSE: Get a list of unhandled connections.
	// REQUIRES: NULL
	std::list<CClientSocketInfo*>& GetUnhandledConnections();

	// PURPOSE: Get a list of CAntiCheat pointers of all players. (with their ids)
	// REQUIRES: NULL
	std::map<unsigned int, CAntiCheat*>& GetPlayers();

	// PURPOSE: Get a players CAntiCheat pointer from their playerid.
	// REQUIRES: a playerid.
	CAntiCheat* GetPlayerFromPlayerid(unsigned int uiPlayerid);

	// PURPOSE: Get the player's ID from their RakNet system address.
	// REQUIRES: A RakNet system address.
	int GetPlayeridFromSystemAddress(const RakNet::SystemAddress& systemAddress);

	// PURPOSE: Check if the player is connected to the AC.
	// REQUIRES: A playerid.
	bool IsPlayerConnectedToAC(unsigned int uiPlayerid);

	// PURPOSE: Close the connection of a playerid.
	// REQUIRES: A playerid.
	void CloseConnection(unsigned int uiPlayerid);

	// PURPOSE: Close an unhandled connection (A connection that connected to the AC server, that didn't connect to the SA:MP server)
	// REQUIRES: A RakNet system address.
	// REQUIRES: The disconnection type.
	void CloseUnhandledConnection(const RakNet::SystemAddress& systemAddress, int type);

	// PURPOSE: Cleanup a connection from a playerid.
	// REQUIRES: A playerid.
	// REQUIRES: A disconnection type
	void Cleanup(unsigned int uiPlayerid, int type);

	// PURPOSE: Cleanup an unhandled connection by system address.
	// REQUIRES: A RakNet system address.
	void CleanupUnhandledConnection(const RakNet::SystemAddress& systemAddress);

	// PURPOSE: Add an unhandled connection to a handled one, meaning, associate an AC connection to a playerid connection on the SA:MP server.
	// REQUIRES: A playerid to try and associate with an AC connection.
	bool HandleConnection(unsigned int uiPlayerid);

	// PURPOSE: Check if a connecton is handled by system address.
	// REQUIRES: A RakNet system address.
	bool IsConnectionHandled(const RakNet::SystemAddress& systemAddress);

	// PURPOSE: Send a packet to a specified playerid.
	// REQUIRES: A Packet Type.
	// REQUIRES: A playerid.
	unsigned int PlayerSend(ePacketType packetType, unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to a playerid.
	// REQUIRES: an RPC id.
	// REQUIRES: a playerid.
	unsigned int PlayerSendRPC(unsigned short usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send a packet to all the players on the server.
	// REQUIRES: A packet type.
	void Broadcast(ePacketType packetType, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to all players on the server.
	// REQUIRES: an RPC id.
	void BroadcastRPC(unsigned short usRPCId, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);

	// PURPOSE: Process packets sent to the server.
	// REQUIRES: The server is initialized.
	void Process();
}
