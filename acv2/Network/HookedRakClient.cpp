#include "../../Shared/Network/BitStream.h"

#include "../s0beit/stdtypes.h"
#include "RakClient.h"
#include "HookedRakClient.h"
#include "CRPCCallback.h"
#include "../../Shared/Network/Network.h"
#include "../../Shared/Network/ACVerifiedPacket.h"
#include "CRakClientHandler.h"
#include "../s0beit/samp.h"
#include "../VersionHelper.h"
#include "../../Shared/MD5_Info/md5.h"
#include "../Misc.h"

#include <Windows.h>
#include "../Enigma/enigma_ide.h"

bool hasSentInitInfo = false;

HookedRakClientInterface::HookedRakClientInterface(RakClient * rakclient) : client(rakclient)
{

}

HookedRakClientInterface::~HookedRakClientInterface()
{

}

bool HookedRakClientInterface::Connect(const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer)
{
	hasSentInitInfo = false;
	return client->GetInterface()->Connect(host, serverPort, clientPort, depreciated, threadSleepTimer);
}

void HookedRakClientInterface::Disconnect(unsigned int blockDuration, unsigned char orderingChannel)
{
	CRakClientHandler::SetConnected(false);
	hasSentInitInfo = false;
	client->GetInterface()->Disconnect(blockDuration, orderingChannel);
}

void HookedRakClientInterface::InitializeSecurity(const char *privKeyP, const char *privKeyQ)
{
	client->GetInterface()->InitializeSecurity(privKeyP, privKeyQ);
}

void HookedRakClientInterface::SetPassword(const char *_password)
{
	client->GetInterface()->SetPassword(_password);
}

bool HookedRakClientInterface::HasPassword(void) const
{
	return client->GetInterface()->HasPassword();
}

bool HookedRakClientInterface::Send(const char *data, const int length, int priority, int reliability, char orderingChannel)
{
	return client->GetInterface()->Send(data, length, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel);
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

	// Calculate verified packet
	/*std::string rawVerifiedP = ACVerifiedPacket::RawVerifiedPacket();

	// Convert verified packet from string to byte
	BYTE digest[16];
	for (int i = 0; i < 16; ++i)
	{
		std::string bt = rawVerifiedP.substr(i * 2, 2);
		digest[i] = static_cast<BYTE>(strtoul(bt.c_str(), NULL, 16));

		// Write this byte
		bsData.Write(digest[i]);
	}*/

	std::wstring hwid = TEXT("");
	if (EP_CheckupIsProtected())
	{
		hwid = EP_RegHardwareID();
	}

	MD5 md5 = MD5();
	std::string hardware = std::string(md5.digestString((unsigned char*)Misc::utf8_encode(hwid.c_str()).c_str()));

	// string to byte
	for (int i = 0; i < 16; ++i)
	{
		bsData.Write(md5.digestRaw[i]);
	}

	// Write the user's AC version to the packet.
	bsData.Write((char*)&VersionHelper::AC_CLIENT_VERSION, sizeof(CSelfUpdater::stVersion));

	// Send the info to the server.
	Send(&bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

bool HookedRakClientInterface::Send(RakNet::BitStream * bitStream, int priority, int reliability, char orderingChannel)
{
	BYTE packetId;
	bitStream->Read(packetId);
	/*CLog log = CLog("packets_sent.txt");
	log.Write("< [Packet Send] %d, len: %d", packetId, bitStream->GetNumberOfBytesUsed());*/

	if (packetId == ID_AUTH_KEY)
	{
		CRPCCallback::Initialize();
	}

	return client->GetInterface()->Send(bitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel);
}

Packet* HookedRakClientInterface::Receive(void)
{
	Packet* p = NULL;
	if ((p = client->GetInterface()->Receive()))
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

void HookedRakClientInterface::DeallocatePacket(Packet *packet)
{
	client->GetInterface()->DeallocatePacket(packet);
}

void HookedRakClientInterface::PingServer(void)
{
	client->GetInterface()->PingServer();
}

void HookedRakClientInterface::PingServer(const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections)
{
	client->GetInterface()->PingServer(host, serverPort, clientPort, onlyReplyOnAcceptingConnections);
}

int HookedRakClientInterface::GetAveragePing(void)
{
	return client->GetInterface()->GetAveragePing();
}

int HookedRakClientInterface::GetLastPing(void) const
{
	return client->GetInterface()->GetLastPing();
}

int HookedRakClientInterface::GetLowestPing(void) const
{
	return client->GetInterface()->GetLowestPing();
}

int HookedRakClientInterface::GetPlayerPing(const PlayerID playerId)
{
	return client->GetInterface()->GetPlayerPing(playerId);
}

void HookedRakClientInterface::StartOccasionalPing(void)
{
	client->GetInterface()->StartOccasionalPing();
}

void HookedRakClientInterface::StopOccasionalPing(void)
{
	client->GetInterface()->StopOccasionalPing();
}

bool HookedRakClientInterface::IsConnected(void) const
{
	return client->GetInterface()->IsConnected();
}

unsigned int HookedRakClientInterface::GetSynchronizedRandomInteger(void) const
{
	return client->GetInterface()->GetSynchronizedRandomInteger();
}

bool HookedRakClientInterface::GenerateCompressionLayer(unsigned int inputFrequencyTable[256], bool inputLayer)
{
	return client->GetInterface()->GenerateCompressionLayer(inputFrequencyTable, inputLayer);
}

bool HookedRakClientInterface::DeleteCompressionLayer(bool inputLayer)
{
	return client->GetInterface()->DeleteCompressionLayer(inputLayer);
}

void HookedRakClientInterface::RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms))
{
	client->GetInterface()->RegisterAsRemoteProcedureCall(uniqueID, functionPointer);
}

void HookedRakClientInterface::RegisterClassMemberRPC(int* uniqueID, void *functionPointer)
{
	client->GetInterface()->RegisterClassMemberRPC(uniqueID, functionPointer);
}

void HookedRakClientInterface::UnregisterAsRemoteProcedureCall(int* uniqueID)
{
	// Only called when the game is closing.
	// Fix a crash when the game closes by setting the raknet interface back to the one SA-MP expects.

	stSAMP* samp = stGetSampInfo();
	samp->pRakClientInterface = client->GetInterface();

	client->GetInterface()->UnregisterAsRemoteProcedureCall(uniqueID);
}

bool HookedRakClientInterface::RPC(int* uniqueID, const char *data, unsigned int bitLength, int priority, int reliability, char orderingChannel, bool shiftTimestamp)
{
	return client->GetInterface()->RPC(uniqueID, data, bitLength, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, shiftTimestamp);
}

bool HookedRakClientInterface::RPC(int* uniqueID, RakNet::BitStream *parameters, int priority, int reliability, char orderingChannel, bool shiftTimestamp)
{
	// use this if you wanna log outgoing RPCs
	/*if ( *uniqueID != RPC_UpdateScoresPingsIPs )
	{
	int len = parameters ? parameters->GetNumberOfBytesUsed() : 0;
	Log( "< [RPC Send] %d, len: %d", *uniqueID, len );
	}*/

	/*if (*uniqueID == RPC_ClientJoin)
	{
		SendInitialInfo();
	}*/

	return client->GetInterface()->RPC(uniqueID, parameters, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, shiftTimestamp);
}

void HookedRakClientInterface::SetTrackFrequencyTable(bool b)
{
	client->GetInterface()->SetTrackFrequencyTable(b);
}

bool HookedRakClientInterface::GetSendFrequencyTable(unsigned int outputFrequencyTable[256])
{
	return client->GetInterface()->GetSendFrequencyTable(outputFrequencyTable);
}

float HookedRakClientInterface::GetCompressionRatio(void) const
{
	return client->GetInterface()->GetCompressionRatio();
}

float HookedRakClientInterface::GetDecompressionRatio(void) const
{
	return client->GetInterface()->GetDecompressionRatio();
}

void HookedRakClientInterface::AttachPlugin(void *messageHandler)
{
	client->GetInterface()->AttachPlugin(messageHandler);
}

void HookedRakClientInterface::DetachPlugin(void *messageHandler)
{
	client->GetInterface()->DetachPlugin(messageHandler);
}

RakNet::BitStream * HookedRakClientInterface::GetStaticServerData(void)
{
	return client->GetInterface()->GetStaticServerData();
}

void HookedRakClientInterface::SetStaticServerData(const char *data, const int length)
{
	client->GetInterface()->SetStaticServerData(data, length);
}

RakNet::BitStream * HookedRakClientInterface::GetStaticClientData(const PlayerID playerId)
{
	return client->GetInterface()->GetStaticClientData(playerId);
}

void HookedRakClientInterface::SetStaticClientData(const PlayerID playerId, const char *data, const int length)
{
	client->GetInterface()->SetStaticClientData(playerId, data, length);
}

void HookedRakClientInterface::SendStaticClientDataToServer(void)
{
	client->GetInterface()->SendStaticClientDataToServer();
}

PlayerID HookedRakClientInterface::GetServerID(void) const
{
	return client->GetInterface()->GetServerID();
}

PlayerID HookedRakClientInterface::GetPlayerID(void) const
{
	return client->GetInterface()->GetPlayerID();
}

PlayerID HookedRakClientInterface::GetInternalID(void) const
{
	return client->GetInterface()->GetInternalID();
}

const char* HookedRakClientInterface::PlayerIDToDottedIP(const PlayerID playerId) const
{
	return client->GetInterface()->PlayerIDToDottedIP(playerId);
}

void HookedRakClientInterface::PushBackPacket(Packet *packet, bool pushAtHead)
{
	client->GetInterface()->PushBackPacket(packet, pushAtHead);
}

void HookedRakClientInterface::SetRouterInterface(void *routerInterface)
{
	client->GetInterface()->SetRouterInterface(routerInterface);
}
void HookedRakClientInterface::RemoveRouterInterface(void *routerInterface)
{
	client->GetInterface()->RemoveRouterInterface(routerInterface);
}

void HookedRakClientInterface::SetTimeoutTime(RakNetTime timeMS)
{
	client->GetInterface()->SetTimeoutTime(timeMS);
}

bool HookedRakClientInterface::SetMTUSize(int size)
{
	return client->GetInterface()->SetMTUSize(size);
}

int HookedRakClientInterface::GetMTUSize(void) const
{
	return client->GetInterface()->GetMTUSize();
}

void HookedRakClientInterface::AllowConnectionResponseIPMigration(bool allow)
{
	client->GetInterface()->AllowConnectionResponseIPMigration(allow);
}

void HookedRakClientInterface::AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength)
{
	client->GetInterface()->AdvertiseSystem(host, remotePort, data, dataLength);
}

RakNetStatisticsStruct* const HookedRakClientInterface::GetStatistics(void)
{
	return client->GetInterface()->GetStatistics();
}

void HookedRakClientInterface::ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)
{
	client->GetInterface()->ApplyNetworkSimulator(maxSendBPS, minExtraPing, extraPingVariance);
}

bool HookedRakClientInterface::IsNetworkSimulatorActive(void)
{
	return client->GetInterface()->IsNetworkSimulatorActive();
}

PlayerIndex HookedRakClientInterface::GetPlayerIndex(void)
{
	return client->GetInterface()->GetPlayerIndex();
}

bool HookedRakClientInterface::RPC_(int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID)
{
	return client->GetInterface()->RPC_(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
}