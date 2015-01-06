#include "../Structs.h"
#include "../CAntiCheat.h"
#include "../../Shared/Network/Network.h"
#include "../Utility.h"
#include "../../Shared/Network/CRPC.h"
#include "../Callback.h"
#include "Network.h"

#ifdef LINUX
#include <pthread.h>
#else
#endif

namespace Network
{
	static CCRakServer* pRakServer;
	static bool bInitialized = false;
	static std::map<unsigned int, CAntiCheat*> players;

	void Initialize(void **ppData)
	{
		int(*pfn_GetRakServer)(void) = (int(*)(void))ppData[PLUGIN_DATA_RAKSERVER];
		pRakServer = (CCRakServer*)pfn_GetRakServer();

		Utility::Printf("pRakServer: 0x%x", pRakServer);
	}

	bool IsInitialized()
	{
		return bInitialized;
	}

	void CloseConnection(int playerid)
	{
		pRakServer->Kick(pRakServer->GetPlayerIDFromIndex(playerid));
	}

	std::map<unsigned int, CAntiCheat*>& GetPlayers()
	{
		return players;
	}

	CAntiCheat* GetPlayerFromPlayerid(unsigned int uiPlayerid)
	{
		if (!IsPlayerConnectedToAC(uiPlayerid)) return NULL;

		return players[uiPlayerid];
	}

	bool IsPlayerConnectedToAC(unsigned int uiPlayerid)
	{
		return players.count(uiPlayerid) == 1;
	}

	void Cleanup(unsigned int uiPlayerid, int type)
	{
		delete players[uiPlayerid];
		players.erase(uiPlayerid);

		Callback::OnACClosed(uiPlayerid, type);
	}

	CCRakServer* GetRakServer()
	{
		return pRakServer;
	}

	unsigned int PlayerSend(ePacketType packetType, unsigned int uiPlayerId, RakNet::BitStream* pBitStream, int priority, int reliability, char cOrderingChannel)
	{
		return pRakServer->Send(pBitStream, priority, reliability, cOrderingChannel, pRakServer->GetPlayerIDFromIndex(uiPlayerId), false);
	}

	unsigned int PlayerSendRPC(int usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream, int priority, int reliability, char cOrderingChannel)
	{
		return pRakServer->RPC(&usRPCId, pBitStream, (PacketPriority)priority, (PacketReliability)reliability, cOrderingChannel, pRakServer->GetPlayerIDFromIndex(uiPlayerId), false, false);
	}


	unsigned int PlayerSendRPC2(int* usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream, int priority, int reliability, char cOrderingChannel)
	{
		return pRakServer->RPC(usRPCId, pBitStream, (PacketPriority)priority, (PacketReliability)reliability, cOrderingChannel, pRakServer->GetPlayerIDFromIndex(uiPlayerId), 0, 0);
	}

	int HandleConnection(int playerid) { return 0; }

	void Process()
	{
		if (!IsInitialized())
			return;

		Packet* pPacket;

		while ((pPacket = pRakServer->Receive()))
		{
			if (!pPacket->length)
				return;

			int iPlayerId;
			RakNet::BitStream bitStream(&pPacket->data[1], pPacket->length - 1, false);

			iPlayerId = pPacket->playerIndex;

			switch (pPacket->data[0])
			{
			case PACKET_RPC:
			{
				if (iPlayerId != -1)
				{
					unsigned short usRpcId;

					if (bitStream.Read<unsigned short>(usRpcId))
						CRPC::Process(usRpcId, bitStream, iPlayerId);

				}

				break;
			}
			default:
				Utility::Printf("Unknown packet received: %u, local: %u", pPacket->data[0]);
				break;
			}

			pRakServer->DeallocatePacket(pPacket);
		}
	}
}