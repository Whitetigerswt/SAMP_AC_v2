#include "../../Shared/Network/BitStream.h"

#include "../s0beit/stdtypes.h"
#include "RakClient.h"
#include "HookedRakClient.h"
#include "CRPCCallback.h"
#include "../../Shared/Network/Network.h"
#include "CRakClientHandler.h"
#include "../s0beit/samp.h"
#include "../CClientUpdater.h"
#include "../CLog.h"
#include "../md5.h"

#include <Windows.h>
#include "../Enigma/enigma_ide.h"

bool hasSentInitInfo = false;

HookedRakClientInterface::HookedRakClientInterface(RakClient * rakclient) : client(rakclient)
{

}

HookedRakClientInterface::~HookedRakClientInterface()
{

}

bool HookedRakClientInterface::Connect( const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer )
{
	hasSentInitInfo = false;
	return client->GetRakClientInterface()->Connect( host, serverPort, clientPort, depreciated, threadSleepTimer );
}

void HookedRakClientInterface::Disconnect( unsigned int blockDuration, unsigned char orderingChannel )
{
	CRakClientHandler::SetConnected(false);
	hasSentInitInfo = false;
	client->GetRakClientInterface()->Disconnect(blockDuration, orderingChannel);
}

void HookedRakClientInterface::InitializeSecurity( const char *privKeyP, const char *privKeyQ )
{
	client->GetRakClientInterface()->InitializeSecurity(privKeyP, privKeyQ);
}

void HookedRakClientInterface::SetPassword( const char *_password )
{
	client->GetRakClientInterface()->SetPassword(_password);
}

bool HookedRakClientInterface::HasPassword( void ) const
{
	return client->GetRakClientInterface()->HasPassword();
}

bool HookedRakClientInterface::Send( const char *data, const int length, int priority, int reliability, char orderingChannel )
{
	return client->GetRakClientInterface()->Send( data, length, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel );
}

void HookedRakClientInterface::SendInitialInfo()
{

	if (hasSentInitInfo)
		return;

	hasSentInitInfo = true;

	// Send the server our hardware ID incase they wanna ban us.
	RakNet::BitStream bsData;

	// Add header info
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(ON_INITIAL_INFO);

	std::string hwid = "";

	// Get the hardware ID
	if (EP_CheckupIsProtected())
	{
		hwid = EP_RegHardwareID();
	}
	BYTE digest[16];

	MD5 md5 = MD5();
	hwid = std::string(md5.digestString((char*)hwid.c_str()));

	// string to byte
	for (int i = 0; i < 16; ++i)
	{
		std::string bt = hwid.substr(i * 2, 2);
		digest[i] = static_cast<BYTE>(strtoul(bt.c_str(), NULL, 16));
		bsData.Write(digest[i]);
	}

	// Write the user's AC version to the packet.
	bsData.Write(CURRENT_MAJOR_VERSION);

	// Send the info to the server.
	Send(&bsData, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0);
}

bool HookedRakClientInterface::Send( RakNet::BitStream * bitStream, int priority, int reliability, char orderingChannel )
{
	BYTE packetId;
	bitStream->Read(packetId);
	/*CLog log = CLog("packets_sent.txt");
	log.Write("< [Packet Send] %d, len: %d", packetId, bitStream->GetNumberOfBytesUsed());*/

	if (packetId == ID_AUTH_KEY)
	{
		CRPCCallback::Initialize();
	}

	return client->GetRakClientInterface()->Send( bitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel );
}

Packet* HookedRakClientInterface::Receive( void )
{
	Packet* p = NULL;
	if ((p = client->GetRakClientInterface()->Receive()))
	{
		if (!p->bitSize)
		{
			return p;
		}

		switch (p->data[0])
		{
			case PACKET_RPC:
			{

				if (!CRakClientHandler::IsConnected())
				{
					CRakClientHandler::SetConnected(true);
				}

				// Read the data sent
				RakNet::BitStream bsData(&p->data[1], p->length, false);
				unsigned short usRpcId;

				if (bsData.Read(usRpcId))
				{
					// Process the RPC
					CRPC::Process(usRpcId, bsData);
				}
			}
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				SendInitialInfo();
			}
		}
	}

	return p;
}

void HookedRakClientInterface::DeallocatePacket( Packet *packet )
{
	client->GetRakClientInterface()->DeallocatePacket( packet );
}

void HookedRakClientInterface::PingServer( void )
{
	client->GetRakClientInterface()->PingServer();
}

void HookedRakClientInterface::PingServer( const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections )
{
	client->GetRakClientInterface()->PingServer( host, serverPort, clientPort, onlyReplyOnAcceptingConnections );
}

int HookedRakClientInterface::GetAveragePing( void )
{
	return client->GetRakClientInterface()->GetAveragePing();
}

int HookedRakClientInterface::GetLastPing( void ) const
{
	return client->GetRakClientInterface()->GetLastPing();
}

int HookedRakClientInterface::GetLowestPing( void ) const
{		
	return client->GetRakClientInterface()->GetLowestPing();
}

int HookedRakClientInterface::GetPlayerPing( const PlayerID playerId )
{
	return client->GetRakClientInterface()->GetPlayerPing( playerId );
}

void HookedRakClientInterface::StartOccasionalPing( void )
{
	client->GetRakClientInterface()->StartOccasionalPing();
}

void HookedRakClientInterface::StopOccasionalPing( void )
{
	client->GetRakClientInterface()->StopOccasionalPing();
}

bool HookedRakClientInterface::IsConnected( void ) const
{
	return client->GetRakClientInterface()->IsConnected();
}

unsigned int HookedRakClientInterface::GetSynchronizedRandomInteger( void ) const
{
	return client->GetRakClientInterface()->GetSynchronizedRandomInteger();
}

bool HookedRakClientInterface::GenerateCompressionLayer( unsigned int inputFrequencyTable[ 256 ], bool inputLayer )
{
	return client->GetRakClientInterface()->GenerateCompressionLayer( inputFrequencyTable, inputLayer );
}

bool HookedRakClientInterface::DeleteCompressionLayer( bool inputLayer )
{
	return client->GetRakClientInterface()->DeleteCompressionLayer( inputLayer );
}

void HookedRakClientInterface::RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )
{
	client->GetRakClientInterface()->RegisterAsRemoteProcedureCall( uniqueID, functionPointer );
}

void HookedRakClientInterface::RegisterClassMemberRPC( int* uniqueID, void *functionPointer )
{
	client->GetRakClientInterface()->RegisterClassMemberRPC( uniqueID, functionPointer );
}

void HookedRakClientInterface::UnregisterAsRemoteProcedureCall( int* uniqueID )
{ 
	// Only called when the game is closing.
	// Fix a crash when the game closes by setting the raknet interface back to the one SA-MP expects.

	stSAMP* samp = stGetSampInfo();
	samp->pRakClientInterface = client->GetRakClientInterface();

	client->GetRakClientInterface()->UnregisterAsRemoteProcedureCall( uniqueID );
}

bool HookedRakClientInterface::RPC( int* uniqueID, const char *data, unsigned int bitLength, int priority, int reliability, char orderingChannel, bool shiftTimestamp )
{
	return client->GetRakClientInterface()->RPC(uniqueID, data, bitLength, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, shiftTimestamp);
}

bool HookedRakClientInterface::RPC( int* uniqueID, RakNet::BitStream *parameters, int priority, int reliability, char orderingChannel, bool shiftTimestamp )
{	
	// use this if you wanna log outgoing RPCs
	/*if ( *uniqueID != RPC_UpdateScoresPingsIPs )
	{
		int len = parameters ? parameters->GetNumberOfBytesUsed() : 0;
		Log( "< [RPC Send] %d, len: %d", *uniqueID, len );
	}*/

	return client->GetRakClientInterface()->RPC(uniqueID, parameters, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, shiftTimestamp);
}

void HookedRakClientInterface::SetTrackFrequencyTable( bool b )
{
	client->GetRakClientInterface()->SetTrackFrequencyTable( b );
}

bool HookedRakClientInterface::GetSendFrequencyTable( unsigned int outputFrequencyTable[ 256 ] )
{
	return client->GetRakClientInterface()->GetSendFrequencyTable( outputFrequencyTable );
}

float HookedRakClientInterface::GetCompressionRatio( void ) const 
{
	return client->GetRakClientInterface()->GetCompressionRatio();
}

float HookedRakClientInterface::GetDecompressionRatio( void ) const
{
	return client->GetRakClientInterface()->GetDecompressionRatio();
}

void HookedRakClientInterface::AttachPlugin( void *messageHandler )
{
	client->GetRakClientInterface()->AttachPlugin(messageHandler);
}

void HookedRakClientInterface::DetachPlugin( void *messageHandler )
{
	client->GetRakClientInterface()->DetachPlugin(messageHandler);
}

RakNet::BitStream * HookedRakClientInterface::GetStaticServerData(void)
{
	return client->GetRakClientInterface()->GetStaticServerData();
}

void HookedRakClientInterface::SetStaticServerData( const char *data, const int length )
{
	client->GetRakClientInterface()->SetStaticServerData( data, length );
}

RakNet::BitStream * HookedRakClientInterface::GetStaticClientData(const PlayerID playerId)
{	
	return client->GetRakClientInterface()->GetStaticClientData( playerId );
}

void HookedRakClientInterface::SetStaticClientData( const PlayerID playerId, const char *data, const int length )
{
	client->GetRakClientInterface()->SetStaticClientData( playerId, data, length );
}

void HookedRakClientInterface::SendStaticClientDataToServer( void )
{
	client->GetRakClientInterface()->SendStaticClientDataToServer();
}

PlayerID HookedRakClientInterface::GetServerID( void ) const
{
	return client->GetRakClientInterface()->GetServerID();
}

PlayerID HookedRakClientInterface::GetPlayerID( void ) const
{
	return client->GetRakClientInterface()->GetPlayerID();
}

PlayerID HookedRakClientInterface::GetInternalID( void ) const
{
	return client->GetRakClientInterface()->GetInternalID();
}

const char* HookedRakClientInterface::PlayerIDToDottedIP( const PlayerID playerId ) const
{
	return client->GetRakClientInterface()->PlayerIDToDottedIP( playerId );
}

void HookedRakClientInterface::PushBackPacket( Packet *packet, bool pushAtHead )
{
	client->GetRakClientInterface()->PushBackPacket(packet, pushAtHead);
}

void HookedRakClientInterface::SetRouterInterface( void *routerInterface )
{
	client->GetRakClientInterface()->SetRouterInterface(routerInterface);
}
void HookedRakClientInterface::RemoveRouterInterface( void *routerInterface )
{
	client->GetRakClientInterface()->RemoveRouterInterface(routerInterface);
}

void HookedRakClientInterface::SetTimeoutTime( RakNetTime timeMS )
{
	client->GetRakClientInterface()->SetTimeoutTime( timeMS );
}

bool HookedRakClientInterface::SetMTUSize( int size )
{
	return client->GetRakClientInterface()->SetMTUSize( size );
}

int HookedRakClientInterface::GetMTUSize( void ) const
{
	return client->GetRakClientInterface()->GetMTUSize();
}

void HookedRakClientInterface::AllowConnectionResponseIPMigration( bool allow )
{
	client->GetRakClientInterface()->AllowConnectionResponseIPMigration( allow );
}

void HookedRakClientInterface::AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength )
{
	client->GetRakClientInterface()->AdvertiseSystem( host, remotePort, data, dataLength );
}

RakNetStatisticsStruct* const HookedRakClientInterface::GetStatistics( void )
{
	return client->GetRakClientInterface()->GetStatistics();
}

void HookedRakClientInterface::ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)
{
	client->GetRakClientInterface()->ApplyNetworkSimulator( maxSendBPS, minExtraPing, extraPingVariance );
}

bool HookedRakClientInterface::IsNetworkSimulatorActive( void )
{
	return client->GetRakClientInterface()->IsNetworkSimulatorActive();
}

PlayerIndex HookedRakClientInterface::GetPlayerIndex( void )
{
	return client->GetRakClientInterface()->GetPlayerIndex();
}

bool HookedRakClientInterface::RPC_(int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID)
{
	return client->GetRakClientInterface()->RPC_(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
}