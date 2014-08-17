#pragma once

#include <RakPeer.h>
#include <MessageIdentifiers.h>
#include "../../Shared/Network/Network.h"

#include <SecureHandshake.h>

class CRakServer
{
public:
	CRakServer();
	virtual ~CRakServer();

	// PURPOSE: Startup the RakNet server.
	// REQUIRES: The current IP.
	// REQUIRES: The port.
	// REQUIRES: The numer of max connections.
	RakNet::StartupResult Startup(const char* szHostAddress, t_port usPort, unsigned short iConnections);

	// PURPOSE: Shutdown the RakServer.
	// REQUIRES: The duration the server will wait for all packets to get here after turning off the server.
	// REQUIRES: The disconnect notification packet priority to all clients.
	void Shutdown(unsigned int blockDuration, PacketPriority priority);

	// PURPOSE: Send a packet to the client.
	// REQUIRES: A packet type.
	// REQUIRES: A RakNet system address.
	unsigned int Send(Network::ePacketType packetType, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F, bool bBroadcast = false);
	
	// PURPOSE: Send an RPC to the client.
	// REQUIRES: An RPC ID.
	// REQUIRES: A RakNet system address.
	unsigned int SendRPC(unsigned short usRPCId, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F, bool bBroadcast = false);

	// PURPOSE: Receive a RakNet packet.
	// REQUIRES: NULL
	RakNet::Packet* Receive();

	// PURPOSE: Deallocate a packet.
	// REQUIRES: A packet to deallocate.
	void DeallocatePacket(RakNet::Packet* pPacket);

	// PURPOSE: Close a connection.
	// REQUIRES: A raknet system address to close.
	void CloseConnection(const RakNet::SystemAddress& pSystemAddress);

private:
	RakNet::RakPeer* m_pPeer;
	RakNet::SocketDescriptor* m_pSocketDescriptor;

	cat::EasyHandshake handshake;
	char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
};