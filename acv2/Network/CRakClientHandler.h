#pragma once

#include "../../Shared/Network/BitStream.h"

class CRakClientHandler
{
public:
	static void Load();
	static void CustomSend(RakNet::BitStream *bs);
	static bool IsConnected();

private:
};