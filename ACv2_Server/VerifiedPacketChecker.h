#pragma once

#include "../Shared/Network/ACVerifiedPacket.h"
#include "../Shared/Network/BitStream.h"

// Time needed to ask players to verify their AC clients again
#define VERIFY_CLIENTS_INTERVAL 180000

#define MAX_UNVERIFIED_CLIENT_WARNINGS	1

struct ClientVerificationStruct
{
	unsigned char ToVerify[ACVerifiedPacket::MAX_ARRAY_SIZE];
	unsigned char ToVerifySize;

	// Last time a player verified their AC client
	int LastTimeVerifiedClient;

	// How many times a player has been warned for not verifying
	unsigned short UnverifiedClientWarnings;
};

namespace VerifiedPacketChecker
{
	void SetLastTimeVerifiedClient(unsigned int playerid, unsigned int time);
	void WarnUnverifiedClient(unsigned int playerid);
	void StartVerifiedPacketChecker();
	void VerifyClient(unsigned int playerid);
	bool IsVerified(unsigned int playerid, RakNet::BitStream &bs);
}