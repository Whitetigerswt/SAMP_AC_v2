#pragma once

#include "../../Shared/Network/BitStream.h"
#include "RakClient.h"

class CRakClientHandler
{
public:
	static void Load();
	static void CustomSend(RakNet::BitStream *bs, PacketPriority priority = (PacketPriority)1, PacketReliability reliability = (PacketReliability)8, char orderingChannel = 0);
	static bool IsConnected();
	static void Disconnect();
	static void SetConnected(bool toggle);
	static bool HasEverConnected();

private:
	static bool everConnected;
	static bool isACServer;
};