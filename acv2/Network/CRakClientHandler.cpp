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

void CRakClientHandler::Load()
{
	// Get the main SAMP structure.
	samp = stGetSampInfo();

	// Get the raknet interfaces.
	client = new RakClient(samp->pRakClientInterface);
	samp->pRakClientInterface = new HookedRakClientInterface(client);

	// Even though we have the raknet interfaces, doesn't mean it's connected yet.
	// Make sure it's connected before we continue.
	while (samp->pRakClientInterface == NULL)
	{
		Sleep(5);
	}

	CRPCCallback::Initialize();
}

void CRakClientHandler::CustomSend(RakNet::BitStream *bs)
{
	if (client != NULL && client->GetRakClientInterface()->IsConnected())
	{
		client->Send(bs);
	}
}

bool CRakClientHandler::IsConnected()
{
	if (!client)
		return false;

	if (!client->GetRakClientInterface())
		return false;

	return client->GetRakClientInterface()->IsConnected();
}