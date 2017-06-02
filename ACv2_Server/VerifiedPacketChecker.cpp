#include "VerifiedPacketChecker.h"
#include "GDK/sampgdk.h"
#include "Callback.h"
#include "Utility.h"
#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"
#include "Network/Network.h"
#include "../Shared/Network/CRPC.h"
#include "PacketPriority.h"
#include <cstdio>
#include <cstring>

ClientVerificationStruct ClientVerification[MAX_PLAYERS];

namespace VerifiedPacketChecker
{
	void SetLastTimeVerifiedClient(unsigned int playerid, unsigned int time)
	{
		ClientVerification[playerid].LastTimeVerifiedClient = time;
		ClientVerification[playerid].UnverifiedClientWarnings = 0;
	}

	void WarnUnverifiedClient(unsigned int playerid)
	{
		ClientVerification[playerid].LastTimeVerifiedClient = Utility::getTickCount();
		ClientVerification[playerid].UnverifiedClientWarnings++;

		if (ClientVerification[playerid].UnverifiedClientWarnings == MAX_UNVERIFIED_CLIENT_WARNINGS)
		{
			Callback::Execute("AC_OnVerificationFail", "i", playerid);

			if (Callback::GetACEnabled() == true)
			{
				char msg[144], name[MAX_PLAYER_NAME];
				sampgdk::GetPlayerName(playerid, name, sizeof name);
				snprintf(msg, sizeof msg, "%s has been kicked for not verifying anti-cheat client in time.", name);
				sampgdk::SendClientMessageToAll(0xFF0000FF, msg);
				Utility::Printf(msg);

				// Kick the player from the server
				sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
			}
			ClientVerification[playerid].UnverifiedClientWarnings = 0;
		}
	}

	bool IsVerified(unsigned int playerid, RakNet::BitStream &bs)
	{
		unsigned char readData[ACVerifiedPacket::MAX_ARRAY_SIZE];
		unsigned short readDataSize = bs.GetNumberOfBytesUsed() - sizeof(eRPC); // we subtract size of RPC_* header from actual size

		if (readDataSize != ClientVerification[playerid].ToVerifySize)
			return 0;
		
		bs.Read((char*)&readData, readDataSize);
		return (0 == memcmp(readData, ClientVerification[playerid].ToVerify, ClientVerification[playerid].ToVerifySize));
	}

	// This asks connected players to verify their AC clients from time to time
	// to fight against unloading AC module after initial connection.
	void SAMPGDK_CALL VerifyClients(int timerid, void *params)
	{
		// Loop through all players.
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			// Make sure the player is connected to the AC and the server.
			if (sampgdk::IsPlayerConnected(i) && CAntiCheatHandler::IsConnected(i))
			{
				// See if they haven't verified their client in a while
				if (Utility::getTickCount() - ClientVerification[i].LastTimeVerifiedClient > VERIFY_CLIENTS_INTERVAL+30)
				{
					WarnUnverifiedClient(i);
				}
				else
				{
					// Request verified packet from client
					VerifyClient(i);
				}
			}
		}
	}

	void VerifyClient(unsigned int playerid)
	{
		RakNet::BitStream bsData;

		unsigned char nBytes = ACVerifiedPacket::MIN_ARRAY_SIZE + rand() % (ACVerifiedPacket::MAX_ARRAY_SIZE - ACVerifiedPacket::MIN_ARRAY_SIZE + 1); // [8 .. 16]
		unsigned char keyByte = rand() % nBytes; // [0 .. nBytes]
		unsigned char sendArray[ACVerifiedPacket::MAX_ARRAY_SIZE + 2]; // this array size must be at least (nBytes+2)
		for (int j = 0; j < nBytes; ++j)
			sendArray[j] = rand() % 256;

		sendArray[nBytes] = sendArray[ACVerifiedPacket::MIN_ARRAY_SIZE];
		sendArray[nBytes + 1] = sendArray[ACVerifiedPacket::MIN_ARRAY_SIZE + 1];

		sendArray[ACVerifiedPacket::MIN_ARRAY_SIZE] = keyByte;
		sendArray[ACVerifiedPacket::MIN_ARRAY_SIZE + 1] = nBytes;

		bsData.Write((unsigned char)PACKET_RPC);
		bsData.Write(VERIFY_CLIENT);
		bsData.Write((char*)&sendArray, nBytes + 2);
		Network::PlayerSend(playerid, &bsData, MEDIUM_PRIORITY, RELIABLE);

		bsData.SetReadOffset(BYTES_TO_BITS((sizeof(unsigned char) + sizeof(eRPC)))); // skip header
		ACVerifiedPacket::Verify(bsData, ClientVerification[playerid].ToVerify, ClientVerification[playerid].ToVerifySize);
	}

	void StartVerifiedPacketChecker()
	{
		sampgdk::SetTimer(VERIFY_CLIENTS_INTERVAL, true, VerifyClients, NULL);
	}
}