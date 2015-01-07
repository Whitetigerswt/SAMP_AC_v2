#include <RakNet\BitStream.h>

#include "RakClient.h"
#include "HookedRakClient.h"
#include "../s0beit/samp.h"
#include "../CLog.h"

#include "Test.h"

void Load_RakClient()
{
	CLog log = CLog("addresses_test.txt");
	log.Write("Welcome");
	stSAMP *samp = stGetSampInfo();
	log.Write("samp: 0x%x", samp);
	log.Write("pRakClientInterface: 0x%x", samp->pRakClientInterface);
	RakClient *client = new RakClient(samp->pRakClientInterface);
	log.Write("Client: 0x%x", client);
	samp->pRakClientInterface = new HookedRakClientInterface(client);
	log.Write("Hooked rakclient: 0x%x", samp->pRakClientInterface);
}