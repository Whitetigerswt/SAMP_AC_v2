#include "../Structs.h"
#include "../CAntiCheat.h"
#include "../../Shared/Network/Network.h"
#include "../Utility.h"
#include "../../Shared/Network/CRPC.h"
#include "../Callback.h"
#include "Network.h"

namespace Network
{
	static CCRakServer* pRakServer;

	void Initialize(void **ppData)
	{
		int(*pfn_GetRakServer)(void) = (int(*)(void))ppData[PLUGIN_DATA_RAKSERVER];
		pRakServer = (CCRakServer*)pfn_GetRakServer();

		if (pRakServer == NULL)
		{
			Utility::Printf("This SA-MP server version is not supported! Let us know of this problem at www.samp-ac.com");
		}
	}

	CCRakServer* GetRakServer()
	{
		return pRakServer;
	}

	unsigned int PlayerSend(unsigned int uiPlayerId, RakNet::BitStream* pBitStream, int priority, int reliability, char cOrderingChannel)
	{
		return pRakServer->Send(pBitStream, (PacketPriority)1, (PacketReliability)9, 0, pRakServer->GetPlayerIDFromIndex(uiPlayerId), false);
	}

	unsigned int PlayerSendRPC(int usRPCId, unsigned int uiPlayerId, RakNet::BitStream* pBitStream, int priority, int reliability, char cOrderingChannel)
	{
		return pRakServer->RPC(&usRPCId, pBitStream, (PacketPriority)priority, (PacketReliability)reliability, cOrderingChannel, pRakServer->GetPlayerIDFromIndex(uiPlayerId), false, false);
	}
}