#include <RakNet\BitStream.h>
#include <Windows.h>

#include "RakClient.h"
#include "HookedRakClient.h"
#include "../s0beit/samp.h"
#include "../CLog.h"
#include "../../Shared/Network/Network.h"

#include "CRakClientHandler.h"

static RakClient* client = NULL;
static stSAMP* samp = NULL;

void CRakClientHandler::Load()
{
	// Get the main SAMP structure.
	samp = stGetSampInfo();

	// Get the raknet interfaces.
	client = new RakClient(samp->pRakClientInterface);
	samp->pRakClientInterface = new HookedRakClientInterface(client);

	//MAKE LOG HERE	- see if null

	// Even though we have the raknet interfaces, doesn't mean it's connected yet.
	// Make sure it's connected before we continue.
	while (samp->pRakClientInterface == NULL || !client->GetRakClientInterface()->IsConnected())
	{
		Sleep(5);
	}
}

void CRakClientHandler::CustomSend(RakNet::BitStream *bs)
{
	if (client != NULL && client->GetRakClientInterface()->IsConnected())
	{
		client->Send(bs);
	}
}