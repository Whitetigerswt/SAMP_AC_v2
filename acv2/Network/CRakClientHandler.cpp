#include "../../Shared/Network/BitStream.h"
#include <boost\thread.hpp>
#include <Windows.h>

#include "../s0beit/stdtypes.h"
#include "RakClient.h"
#include "HookedRakClient.h"
#include "../s0beit/samp.h"
#include "../../Shared/Network/Network.h"
#include "../CLoader.h"
#include "../CLog.h"
#include "../CPacketIntegrity.h"

#include "CRPCCallback.h"
#include "CRakClientHandler.h"
#include "CPacketQueue.h"

static RakClient* client = NULL;
static stSAMP* samp = NULL;

bool CRakClientHandler::isACServer = false;
bool CRakClientHandler::everConnected = false;

void CRakClientHandler::Load()
{
	// get samp struct
	samp = stGetSampInfo();

	// Make sure rak client interface is initialized
	while (samp->pRakClientInterface == NULL)
	{
		Sleep(5);
	}

	// Get the raknet interfaces.
	client = new RakClient(samp->pRakClientInterface);
	samp->pRakClientInterface = new HookedRakClientInterface(client);
}

void CRakClientHandler::CustomSend(RakNet::BitStream *bs, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	if (IsConnected())
	{
		// save packet for comparing before sending
		new CPacketIntegrity(bs);

		client->Send(bs, priority, reliability, orderingChannel);
	}
	else
	{
		// save the packet and resend it after we connect to the server
		CPacketQueue::Add(bs, priority, reliability, orderingChannel);
	}
}

bool CRakClientHandler::IsConnected()
{
	if (!CLoader::IsProcessElevated())
		return false;

	if (!isACServer)
		return false;

	if (!client)
		return false;

	if (!client->GetInterface())
		return false;

	return client->GetInterface()->IsConnected();
}

void CRakClientHandler::Disconnect()
{
	if (!client)
		return;

	if (!client->GetInterface())
		return;

	return client->GetInterface()->Disconnect(500);
}

void CRakClientHandler::SetConnected(bool toggle)
{
	isACServer = toggle;
	everConnected = true;
}

bool CRakClientHandler::HasEverConnected()
{
	return everConnected;
}