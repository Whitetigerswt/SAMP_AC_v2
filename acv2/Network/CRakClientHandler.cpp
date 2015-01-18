#include "../../Shared/Network/BitStream.h"
#include <Windows.h>

#include "../s0beit/stdtypes.h"
#include "RakClient.h"
#include "HookedRakClient.h"
#include "../s0beit/samp.h"
#include "../../Shared/Network/Network.h"

#include "CRPCCallback.h"
#include "CRakClientHandler.h"

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
		client->Send(bs, priority, reliability, orderingChannel);
	}
}

bool CRakClientHandler::IsConnected()
{
	if (!client)
		return false;

	if (!client->GetRakClientInterface())
		return false;

	if (!isACServer)
		return false;

	return client->GetRakClientInterface()->IsConnected();
}

void CRakClientHandler::Disconnect()
{
	if (!client)
		return;

	if (!client->GetRakClientInterface())
		return;

	return client->GetRakClientInterface()->Disconnect(500);
}

void CRakClientHandler::SetConnected(bool toggle)
{
	isACServer = toggle;
	if (!everConnected)
		everConnected = true;
}

bool CRakClientHandler::HasEverConnected()
{
	return everConnected;
}