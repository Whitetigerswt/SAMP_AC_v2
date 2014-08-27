#include "Callback.h"
#include "Network/Network.h"
#include "Utility.h"
#include "GDK/a_players.h"
#include "CAntiCheat.h"
#include "CPlayer.h"
#include "../Shared/Network/CRPC.h"
#include "CServerUpdater.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define snprintf sprintf_s
#endif

namespace Callback
{
	static AMX* amx_allowed = NULL;
	static std::list<AMX*> amxPointers;

	static bool ACToggle = true;

	std::list<AMX*>& GetAMXList()
	{
		return amxPointers;
	}

	cell Execute(const char* szFunction, const char* szFormat, ...)
	{
		cell iReturnValue = 1;
		cell* pReturnValues = new cell[amxPointers.size()];
		std::fill_n(pReturnValues, amxPointers.size(), 1);
		size_t idx = 0;
		va_list argPtr;

		for (std::list<AMX*>::iterator it = amxPointers.begin(); it != amxPointers.end(); ++it, ++idx)
		{
			AMX* pAmx = *it;
			int iFuncIdx;

			if (amx_FindPublic(pAmx, szFunction, &iFuncIdx) != 0)
				continue;

			cell addresses[16];
			unsigned int addr_idx = 0;

			if (szFormat)
			{
				va_start(argPtr, szFormat);

				for (unsigned int i = 0; i < strlen(szFormat); ++i)
				{
					switch (szFormat[i])
					{
					case 'i':
					{
						amx_Push(pAmx, va_arg(argPtr, int));

						break;
					}
					case 's':
					{
						amx_PushString(pAmx, &addresses[addr_idx++], NULL, va_arg(argPtr, char*), false, false);

						break;
					}
					case 'a':
					{
						cell iAmxAddr, *pPhysAddr;

						PAWNArray array = va_arg(argPtr, PAWNArray);
						amx_Allot(pAmx, array.length, &iAmxAddr, &pPhysAddr);
						memcpy(pPhysAddr, array.address, array.length*sizeof(cell));
						amx_Push(pAmx, iAmxAddr);
						break;
					}
					}
				}

				va_end(argPtr);
			}

			amx_Exec(pAmx, &pReturnValues[idx], iFuncIdx);

			for (unsigned int i = 0; i < addr_idx; ++i)
				amx_Release(pAmx, addresses[i]);

		}

		for (size_t i = 0; i < idx; ++i)
			if (!pReturnValues[i])
				iReturnValue = 0;

		delete[] pReturnValues;

		return iReturnValue;
	}

	void SAMPGDK_CALL KickPlayer(int timerid, void *params)
	{
		// Make sure the player is connected.
		if (IsPlayerConnected((int)params))
		{
			// Kick the player from the server.
			Kick((int)params);
		}
	}

	void SAMPGDK_CALL CheckPacketResponse(int timerid, void *params)
	{
		// Convert the params into a playerid.
		int playerid = (int)params;

		// Make sure the player is still connected to the server.
		if (!IsPlayerConnected(playerid))
		{
			// Return
			return;
		}

		// Make sure the player is connected to the server and AC.
		if (!Network::IsPlayerConnectedToAC(playerid))
		{
			// Create new variables to hold strings we'll send to the server.
			char msg[144], name[MAX_PLAYER_NAME];

			// Get the player's name
			GetPlayerName(playerid, name, sizeof(name));

			// Format the main string
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server ({FF0000}AC Lost Connection{FFFFFF})");

			// Send the message to the rest of the players on the server.
			SendClientMessageToAll(-1, msg);

			// Kick the player from the server.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);

			// Don't continue in the function.
			return;
		}

		// Get the player's CAntiCheat pointer.
		CAntiCheat* ac = Network::GetPlayerFromPlayerid(playerid);
		
		// Get the player's Hardware ID.
		std::string hwid = ac->GetPlayerHardwareID();

		// Check if it's an empty string
		if (hwid.empty())
		{
			// If it's empty, the client didn't respond to our PACKET_PLAYER_REGISTERED from OnPlayerConnect
			// (When the player first connects it sends the players HardwareID immediately after receiving that packet)

			// Create new variables for strings we'll send to the rest of the players telling them what happened.
			char msg[144], name[MAX_PLAYER_NAME];

			// Get the player's name
			GetPlayerName(playerid, name, sizeof(name));

			// Format the main string we'll send to the players on the server.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF}'s AC did not respond in time.");

			// Send the message to the server
			SendClientMessageToAll(-1, msg);

			// Kick the player from the server.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
		}
	}
	
	void SAMPGDK_CALL CheckPlayersMemory(int timerid, void *params) 
	{
		// Get the player ID from the params.
		int playerid = (int)params;

		// Make sure the player is connected to the AC and the server.
		if (IsPlayerConnected(playerid) && Network::IsPlayerConnectedToAC(playerid))
		{
			// Verify the players weapon.dat values.
			RakNet::BitStream bsData;
			bsData.Write(0xC8C418); 
			bsData.Write(0x460);

			// Send RPC.
			Network::PlayerSendRPC(MD5_MEMORY_REGION, playerid, &bsData);

			/*// Verify the players handling.cfg values
			RakNet::BitStream bsData2;
			bsData2.Write(0xC2B9DC);
			bsData2.Write(0xAF00);

			Network::PlayerSendRPC(MD5_MEMORY_REGION, playerid, &bsData2);*/

			// Repeat on a 1 minute interval.
			SetTimer(60000, 0, CheckPlayersMemory, (void*)playerid);
		}
	}

	void OnACClosed(unsigned int playerid, int type)
	{
		// If the player is connected, and /actoggle has been turned on (aka, AC is on)
		if (IsPlayerConnected(playerid) && ACToggle && type == 1)
		{
			// Create 2 variables, one to hold the file name, and one to format a string to send to all users on the server
			// letting them know why this player was kicked.
			char str[144], name[MAX_PLAYER_NAME];

			// Get the player name and store it in the name variable.
			GetPlayerName(playerid, name, sizeof(name));

			// Format the string
			snprintf(str, sizeof(str), "{FF0000}%s{FFFFFF} has been kicked from the server ({FF0000}AC Lost Connection{FFFFFF})", name);

			// Send the string to everyone
			SendClientMessageToAll(-1, str);

			// Kick the user from the server.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
		} else {
			// Just make sure the user is kicked from the server.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
		}
	}

	void OnACClosed(std::string ip, int type)
	{
		// If someone's AC is closed, and we only have their IP, we need to get their playerid to do anything.

		// Loop through MAX_PLAYERS
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			// Make sure the player is connected to the server.
			if (IsPlayerConnected(i))
			{
				// Create a new variable to store the player's IP address.
				char IP[MAX_PLAYER_NAME];

				// Get the player's IP address and store it in the variable we just created.
				GetPlayerIp(i, IP, sizeof(IP));

				// Convert their IP address to an std::string as it's easier to work with.
				std::string userip(IP);

				// Compare the string to the string passed in as a parameter to this function
				if (ip.compare(userip) == 0)
				{
					// If they match, we just found our playerid, and send it to the other OnACClosed so we can handle this situation properly.
					OnACClosed(i, type);
				}
			}
		}
		// Execute the callback to PAWN.
		Execute("OnACClosed", "is", type, ip.c_str());
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
	{
		// If there is an IP address connected to the AC, try and associate it with this playerid.
		if (Network::HandleConnection(playerid))
		{
			// If it was successful, send this player a greeting packet!
			Network::PlayerSend(Network::PACKET_PLAYER_REGISTERED, playerid);

			// Find a CAntiCheat class associated with this player (this was created in Network::HandleConnection earlier in this function)
			CAntiCheat* ac = Network::GetPlayerFromPlayerid(playerid);

			// Send the client the files we need them to return md5's to.
			ac->CheckGTAFiles(playerid);

			// Disable lite foot by default.
			ac->ToggleLiteFoot(false);

			// Check memory pretty frequently in a new timer.
			SetTimer(1, 0, CheckPlayersMemory, (void*)playerid);
			
			// Check if AC is on
			if (ACToggle)
			{
				// Check for a response packet from PACKET_PLAYER_REGISTERED. (The client should send back the HWID ASAP).
				// If we don't get a response, then directX failed to hook(?) on client side.
				SetTimer(5000, 0, CheckPacketResponse, (void*)playerid);
			}
		}

		// If the player is not running the AC, and /actoggle has been turned on (aka AC is on)
		if (!Network::IsPlayerConnectedToAC(playerid) && ACToggle)
		{
			// Notify them that this isn't allowed.
			SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You've been kicked from this server for not running Whitetiger's Anti-Cheat (v2)");

			// Create 2 variables, one to hold the player name, and one to format a string telling all the player's on the server that this player has been kicked for not running the AC.
			char msg[160], name[MAX_PLAYER_NAME];

			// Get the player name and store it in the name variable.
			GetPlayerName(playerid, name, sizeof(name));

			// Format the string telling all the users that this newly connected player is not running the AC, and we're going to kick him.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for not running Whitetiger's Anti-Cheat (v2)", name);
			
			// Send them the formatted message.
			SendClientMessageToAll(-1, msg);

			// Write in the console what we just did.
			Utility::Printf("%s has been kicked from the server for not connecting with AC while AC is on.", name);

			// Finally, kick the player.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
		} 

		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
	{
		// If the player has just disconnected from the server, we need to handle AC disconnect as well.

		// If the player was connected to the AC before disconnecting.
		if (Network::IsPlayerConnectedToAC(playerid) != NULL)
		{
			// And they weren't kicked/banned or timed out.
			if (reason)
			{
				// Send them a goodbye packet :(
				Network::PlayerSend(Network::PACKET_PLAYER_PROPER_DISCONNECT, playerid);
			}

			// Close off the connection cleanly.
			Network::CloseConnection(playerid);
		}
		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit()
	{
		// Check for an update to this plugin version.
		CServerUpdater::CheckForUpdate();

		// Check if the AC server is already started.
		if (!Network::IsInitialized())
		{
			try
			{
				// Initialize raknet server to be connected to by the AC.
				Network::Initialize("", GetServerVarAsInt("port") - 7, GetServerVarAsInt("maxplayers") + 100);
			}
			catch (std::exception &e)
			{
				// an error occured, possibly due to the port being already in use?
				Utility::Printf("We've run into an exception when trying to initialize the AC server. Here's what came back");
				Utility::Printf(e.what());
				Utility::Printf("To fix this issue, the solution may be to simply port forward port %d, and make sure no other software is using this port.", GetServerVarAsInt("port") - 7);
			}
		}

		return true;
	}
	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char* params)
	{
		// If the user typed /actoggle and they're allowed to run that command.
		if (!strcmp(params, "/actoggle") && CAntiCheat::CanEnableAC(playerid))
		{ 
			// Set ACToggle to whatever it wasn't previously.
			ACToggle = !ACToggle;

			// Create 2 variables, one to hold the player name who just enabled/disabled AC, and one to send a formatted message
			// telling all the users on the server that the AC is now on and they will be kicked if they're not using it.
			char str[144], name[MAX_PLAYER_NAME];

			// Get the player name
			GetPlayerName(playerid, name, sizeof(name));

			// Format the message telling all the users that AC is now on.
			snprintf(str, sizeof(str), "{FF0000}%s{0000FF} has {FFFFFF}%s{0000FF} SAMP_AC_v2.", name, ACToggle == true ? "Enabled" : "Disabled");

			// Send the message to everyone on the server.
			SendClientMessageToAll(-1, str);

			// If the AC was turned on
			if (ACToggle)
			{
				// Loop through the maximum amount of players that can be connected to the server.
				for (int i = 0; i < MAX_PLAYERS; ++i)
				{
					// If they're connected to the server, and not connected to the AC
					if (IsPlayerConnected(i) && !Network::IsPlayerConnectedToAC(i))
					{
						// Kick them from the server!
						SetTimer(1000, 0, Callback::KickPlayer, (void*)i);
					}
				}
			}
			// We know what command they typed, so return true;
			return true;
		}
		return false;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnRconLoginAttempt(const char* ip, const char* port, bool success)
	{
		// If the rcon login attempt wasn't successful, we don't really care...
		if (!success) return true;

		// Loop through the maximum amount of players.
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			// If the player is connected.
			if (IsPlayerConnected(i))
			{
				// Create a variable to store the player's IP.
				char IP[MAX_PLAYER_NAME];

				// Get the player's IP and store it in the variable we just created.
				GetPlayerIp(i, IP, sizeof(IP));

				// Compare the IP to the IP that tried to do that rcon login
				if (!strcmp(ip, IP))
				{
					// If it was them, we just found the playerid who logged into rcon, therfore he's allowed to use the /actoggle command.
					CAntiCheat::ToggleCanEnableAC(i, true);
				}
			}
		}
		return true;
	}
}