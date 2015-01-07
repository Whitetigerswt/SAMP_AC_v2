#pragma once

#include "CRakClient.h"
#include "../../Shared/Network/Network.h"

namespace Network
{
	// PURPOSE: Initialize the network connection to the host.
	// REQUIRES: The hosts IP.
	// REQUIRES: The hosts port.
	// PROMISE: Initialized the host.
	void Initialize(std::string address, unsigned short port);

	// PURPOSE: Check if the network connection is initialized.
	// REQUIRES: NULL
	bool IsInitialized();

	// PURPOSE: Connect to the server.
	// REQUIRES: The network is initialized.
	void Connect();

	// PURPOSE: Reconnect to the server - do not call before connecting to the server!
	// REQUIRES: The network is initialized, and has already connected once to the server.
	void Reconnect();

	// PURPOSE: Disconnect from the host server.
	// REQUIRES: Be connected.
	void Disconnect();

	// PURPOSE: Check if the client is connected.
	// REQUIRES: NULL
	bool IsConnected();

	// PURPOSE: Check if the server has a plugin
	// REQUIRES: NULL
	bool ServerHasPlugin();

	// PURPOSE: Process network RPC's
	// REQUIRES: The user is connected.
	void Process();

	// PURPOSE: Send a packet to the server.
	// REQUIRES: The user is connected.
	unsigned int Send(ePacketType packetType, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to the server.
	// REQUIRES: The user is connected.
	unsigned int SendRPC(unsigned short usRPCId, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Get the CRakClient pointer
	// REQUIRES: NULL
	CRakClient* GetRakClient();
}
