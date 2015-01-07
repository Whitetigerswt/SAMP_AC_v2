#include <RakNet\BitStream.h>
#include <Windows.h>

#include "RakClient.h"
#include "HookedRakClient.h"
#include "../s0beit/samp.h"
#include "../CLog.h"
#include "../../Shared/Network/Network.h"

#include "CRakClientHandler.h"

RakClient* client = NULL;
stSAMP* samp = NULL;

CRakClientHandler::CRakClientHandler()
{
	// Get the main SAMP structure.
	samp = stGetSampInfo();

	// Get the raknet interfaces.
	client = new RakClient(samp->pRakClientInterface);
	samp->pRakClientInterface = new HookedRakClientInterface(client);

	// Even though we have the raknet interfaces, doesn't mean it's connected yet.
	// Make sure it's connected before we continue.
	while (samp->pRakClientInterface == NULL || !client->GetRakClientInterface()->IsConnected())
	{
		Sleep(5);
	}

	RakNet::BitStream bsData;
	bsData.Write((unsigned char)Network::PACKET_RPC); // packetid
	bsData.Write((unsigned short)20U); // TAKE_SCREENSHOT

	CustomRPC(&bsData);
}

void CRakClientHandler::CustomRPC(RakNet::BitStream *bs)
{
	if (client->GetRakClientInterface()->IsConnected())
	{
		client->Send(bs);
	}
}