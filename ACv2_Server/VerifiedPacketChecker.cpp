#include "VerifiedPacketChecker.h"
#include "GDK/sampgdk.h"
#include "Callback.h"
#include "Utility.h"
#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"
#include "Network/Network.h"
#include "../Shared/Network/CRPC.h"
#include "PacketPriority.h"

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
		if (ClientVerification[playerid].UnverifiedClientWarnings == MAX_UNVERIFIED_CLIENT_WARNINGS)
		{
			char msg[144], name[MAX_PLAYER_NAME];
			GetPlayerName(playerid, name, sizeof name);
			snprintf(msg, sizeof msg, "%s has been kicked for not verifying anti-cheat client in time.", name);
			SendClientMessageToAll(0xFF0000FF, msg);
			Utility::Printf(msg);

			// Kick the player from the server
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
			return;
		}
		ClientVerification[playerid].LastTimeVerifiedClient = sampgdk_GetTickCount();
		ClientVerification[playerid].UnverifiedClientWarnings ++;
	}

	bool IsVerified(unsigned int playerid, unsigned char byt, unsigned int index)
	{
		return (byt == ClientVerification[playerid].ToVerify[index]);
	}

	// This asks connected players to verify their AC clients from time to time
	// to fight against unloading AC module after initial connection.
	void SAMPGDK_CALL VerifyClients(int timerid, void *params)
	{
		// Loop through all players.
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			// Make sure the player is connected to the AC and the server.
			if (IsPlayerConnected(i) && CAntiCheatHandler::IsConnected(i))
			{
				// See if they haven't verified their client in a while
				if (GetTickCount() - ClientVerification[i].LastTimeVerifiedClient > VERIFY_CLIENTS_INTERVAL)
				{
					WarnUnverifiedClient(i);
				}
				else
				{
					// Request verified packet from client

					RakNet::BitStream bsData;

					// Write header
					bsData.Write((unsigned char)PACKET_RPC);
					bsData.Write(VERIFY_CLIENT);

					// Generate random array and send it to client
					for (int j = 0; j != ACVerifiedPacket::MAX_ARRAY_SIZE; ++j)
					{
						// Generate randomly
						ClientVerification[i].ToVerify[j] = rand() % 255;
						
						// Write raw byte to bitstream
						bsData.Write(ClientVerification[i].ToVerify[j]);

						// Apply verified packet algorithm and store it so we can compare when client replies
						ACVerifiedPacket::Verify(&ClientVerification[i].ToVerify[j]);
					}

					// Send RPC.
					Network::PlayerSend(i, &bsData, HIGH_PRIORITY, RELIABLE);
				}
			}
		}
	}

	void StartVerifiedPacketChecker()
	{
		SetTimer(VERIFY_CLIENTS_INTERVAL, true, VerifyClients, NULL);
	}
}