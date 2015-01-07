#pragma once

#include <RakNet/RakPeer.h>
#include "../../Shared/Network/Network.h"

class CRakClient
{
public:
	CRakClient();
	virtual ~CRakClient();

	// PURPOSE: Start up the RakNet client.
	// REQUIRES: NULL
	RakNet::StartupResult Startup();

	// PURPOSE: Shut down the rak client.
	// REQUIRES: NULL
	void Shutdown(int iBlockDuration);

	// PURPOSE: Attempt a connection to the server.
	// REQUIRES: The servers IP.
	// REQUIRES: The servers port.
	// REQUIRES: The servers password.
	RakNet::ConnectionAttemptResult Connect(const char* szAddress, unsigned short usPort, const char* szPassword);

	// PURPOSE: Send a packet to the server.
	// REQUIRES: The packet type
	// REQUIRES: The system address of the server.
	unsigned int Send(Network::ePacketType packetType, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Send an RPC to the server.
	// REQUIRES: RPC id.
	// REQUIRES: the system address of the server.
	unsigned int SendRPC(unsigned short usRPCId, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F);
	
	// PURPOSE: Recieve information from the server
	// REQUIRES: You're connected to the server.
	RakNet::Packet* Receive();

	// PURPOSE: Deallocate a packet.
	// REQUIRES: a RakNet packet to deallocate.
	void DeallocatePacket(RakNet::Packet* pPacket);

	// PURPOSE: Disconnect from the RakNet server.
	// REQUIRES: NULL
	// PROMISE: You will be disconnected from the RakServer.
	void Disconnect();

	// PURPOSE: Check if you're connected to the RakServer.
	// REQUIRES: NULL
	bool IsConnected();

	// PURPOSE: Get the servers System Address pointer.
	// REQUIRES: Be connected to the server.
	RakNet::SystemAddress* GetRemoteAddress();

private:
	RakNet::RakPeer* m_pPeer;
	RakNet::SocketDescriptor* m_pSocketDescriptor;
	RakNet::SystemAddress* m_pSystemAddress;
	bool m_bConnected;

};