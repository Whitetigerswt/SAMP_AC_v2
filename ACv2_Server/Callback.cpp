#include "Callback.h"
#include "Network/Network.h"
#include "Utility.h"
#include "GDK/sampgdk.h"
#include "CAntiCheat.h"
#include "../Shared/Network/CRPC.h"
#include "VersionHelper.h"
#include "CAntiCheatHandler.h"
#include "PacketPriority.h"
#include "BanHandler.h"
#include "VerifiedPacketChecker.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace Callback
{
	static AMX* amx_allowed = NULL;
	static std::list<AMX*> amxPointers;

	std::list<AMX*>& GetAMXList()
	{
		return amxPointers;
	}

	// AC Config property tree, must be global, used for writing to or modifying ac_config.ini
	static boost::property_tree::ptree ac_config_ptree;

	// Initialization, AC is enabled
	static bool ACToggle = true;

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
		if (sampgdk::IsPlayerConnected((int)params))
		{
			// Kick the player from the server.
			sampgdk::Kick((int)params);
		}
	}

	void SAMPGDK_CALL CheckAC(int timerid, void *params)
	{
		// Convert the params into a playerid.
		int playerid = (int)params;

		// Make sure the player is still connected to the server.
		if (!sampgdk::IsPlayerConnected(playerid))
		{
			// Return
			return;
		}
	}
	
	void SAMPGDK_CALL CheckPlayersMemory(int timerid, void *params) 
	{
		// Loop through all players.
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			// Make sure the player is connected to the AC and the server.
			if (sampgdk::IsPlayerConnected(i) && CAntiCheatHandler::IsConnected(i))
			{
				// Verify the players weapon.dat values.
				RakNet::BitStream bsData;

				// Write header
				bsData.Write((unsigned char)PACKET_RPC);
				bsData.Write(MD5_MEMORY_REGION);

				bsData.Write(0xC8C418);
				bsData.Write(0x460);

				// Send RPC.
				Network::PlayerSend(i, &bsData, LOW_PRIORITY, RELIABLE);

				/*// Verify the players handling.cfg values
				RakNet::BitStream bsData2;
				bsData2.Write(0xC2B9DC);
				bsData2.Write(0xAF00);

				Network::PlayerSendRPC(MD5_MEMORY_REGION, playerid, &bsData2);*/
			}
		}
	}

	bool GetACEnabled()
	{
		return ACToggle;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
	{
		// Reset this variable at connect
		CAntiCheat::ToggleCanEnableAC(playerid, false);

		// Make sure the new connected user isn't an NPC.
		if (sampgdk::IsPlayerNPC(playerid))
		{
			// Return
			return true;
		}
		
		CAntiCheat *ac = CAntiCheatHandler::GetAntiCheat(playerid);
		if (ac && !ac->IsCreationTickValid())
		{
			CAntiCheatHandler::Remove(playerid);
			ac = NULL;
		}

		if (ac != NULL)
		{
			std::string hwid = "";
			// Request player's ban status
			BanHandler::CheckCheater(playerid);

			// Get the player's Hardware ID.
			hwid = ac->GetPlayerHardwareID();

			if (ACToggle)
			{
				// Tell the player we're using the AC on this server.
				sampgdk::SendClientMessage(playerid, -1, "{FF0000}Warning: {FFFFFF}This server has Anti-Cheat (v2) enabled.");
			}

			// Send the client the files we need them to return md5's to.
			ac->CheckGTAFiles();

			// Set defaults
			ac->ToggleUnlimitedSprint(Callback::Default_InfSprint);
			ac->ToggleSprintOnAllSurfaces(Callback::Default_SprintOnAllSurfaces);
			ac->ToggleMacroLimitations(Callback::Default_MacroLimits);
			ac->ToggleSwitchReload(Callback::Default_SwitchReload);
			ac->ToggleCrouchBug(Callback::Default_CrouchBug);
			ac->SetSprintLimit(Callback::Default_SprintLimit);
			ac->ToggleVehicleBlips(Callback::Default_VehicleBlips);

			// Check if it's an empty string
			if (hwid.empty())
			{
				if (ACToggle)
				{
					// Create new variables for strings we'll send to the rest of the players telling them what happened.
					char msg[144], name[MAX_PLAYER_NAME];

					// Get the player's name
					sampgdk::GetPlayerName(playerid, name, sizeof(name));

					// Format the main string we'll send to the players on the server.
					snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF}'s AC did not respond in time.", name);

					// Send the message to the server
					sampgdk::SendClientMessageToAll(-1, msg);

					// Kick the player from the server.
					sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);
				}

			}
			// hwid.empty()
		} // CAntiCheatHandler::IsConnected(playerid)
		else if (ACToggle)
		{
			// Notify them that this isn't allowed.
			sampgdk::SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You've been kicked from this server for not running Whitetiger's Anti-Cheat (v2)");

			char msg[160], name[MAX_PLAYER_NAME];

			// Get the player name and store it in the name variable.
			sampgdk::GetPlayerName(playerid, name, sizeof(name));
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for not running Whitetiger's Anti-Cheat (v2)", name);

			// Send them the formatted message.
			sampgdk::SendClientMessageToAll(-1, msg);

			// Tell them where to get the AC and install it.
			snprintf(msg, sizeof msg, "{FFFFFF}You can download the latest version of Whitetiger's Anti-Cheat at: {FF0000}%s", AC_WEBSITE);
			sampgdk::SendClientMessage(playerid, -1, msg);

			Utility::Printf("%s has been kicked from the server for not connecting with AC while AC is on.", name);

			// Finally, kick the player.
			sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)playerid);

			return true;
		}

		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
	{
		// If the player has just disconnected from the server, we need to handle AC disconnect as well.

		// If the player was connected to the AC before disconnecting.
		if (CAntiCheatHandler::IsConnected(playerid))
		{
			CAntiCheatHandler::Remove(playerid);
		}
		return true;
	}

	void WriteAndLoadDefaultConfig()
	{
		ACToggle = true;
		Default_InfSprint = true;
		Default_SprintOnAllSurfaces = true;
		Default_MacroLimits = true;
		Default_VehicleBlips = true;
		Default_SwitchReload = true;
		Default_KickPacketTampering = false;

		Default_CrouchBug = 9999;
		Default_FrameLimit = 9999;
		Default_SprintLimit = 0.0f;

		ac_config_ptree.put("defaults.main_ac_checks", (ACToggle == true) ? 1 : 0);
		ac_config_ptree.put("defaults.inf_sprint", (Default_InfSprint == true) ? 1 : 0);
		ac_config_ptree.put("defaults.sprint_all_surfaces", (Default_SprintOnAllSurfaces == true) ? 1 : 0);
		ac_config_ptree.put("defaults.macro_limits", (Default_MacroLimits == true) ? 1 : 0);
		ac_config_ptree.put("defaults.vehicle_blips", (Default_VehicleBlips == true) ? 1 : 0);
		ac_config_ptree.put("defaults.switch_reload", (Default_SwitchReload == true) ? 1 : 0);
		ac_config_ptree.put("defaults.crouch_bug", Default_CrouchBug);
		ac_config_ptree.put("defaults.frame_limit", Default_FrameLimit);
		ac_config_ptree.put("defaults.sprint_speed_limit", static_cast<int>(Default_SprintLimit * 10.0f));
		ac_config_ptree.put("defaults.kick_for_packet_tampering", (Default_KickPacketTampering == true) ? 1 : 0);

		boost::property_tree::ini_parser::write_ini("ac_config.ini", ac_config_ptree);
	}

	bool onGameModeInitCalled = false; // if OnGameModeInit has been called at least once
	PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit()
	{
		if (onGameModeInitCalled)
			return true;

		// Check memory pretty frequently in a new timer.
		sampgdk::SetTimer(60000, true, CheckPlayersMemory, NULL);

		// Request client verification in a repeated timer
		VerifiedPacketChecker::StartVerifiedPacketChecker();

		// Run a timer that keeps trying to re-apply failed bans
		BanHandler::StartQueuedBansChecker();

		// Seed for rand()
		srand((unsigned int)time(NULL));

		if (!boost::filesystem::exists("ac_config.ini"))
		{
			WriteAndLoadDefaultConfig();
			Utility::Printf("Notice: ac_config.ini was missing, we've created one with default AC values.");
		}
		else
		{
			try 
			{
				// Load config.
				boost::property_tree::ini_parser::read_ini("ac_config.ini", ac_config_ptree);

				// Load default values from file.
				ACToggle = ac_config_ptree.get<bool>("defaults.main_ac_checks");
				Default_InfSprint = ac_config_ptree.get<bool>("defaults.inf_sprint");
				Default_SprintOnAllSurfaces = ac_config_ptree.get<bool>("defaults.sprint_all_surfaces");
				Default_MacroLimits = ac_config_ptree.get<bool>("defaults.macro_limits");
				Default_SprintLimit = static_cast<float>(ac_config_ptree.get<int>("defaults.sprint_speed_limit")) / 10.0f;
				Default_SwitchReload = ac_config_ptree.get<bool>("defaults.switch_reload");
				Default_CrouchBug = ac_config_ptree.get<int>("defaults.crouch_bug");
				Default_FrameLimit = ac_config_ptree.get<int>("defaults.frame_limit");
				Default_VehicleBlips = ac_config_ptree.get<bool>("defaults.vehicle_blips");
				Default_KickPacketTampering = ac_config_ptree.get<bool>("defaults.kick_for_packet_tampering");
			}
			catch (const boost::property_tree::ptree_error &e)
			{
				Utility::Printf("Notice: An error occured while loading configuration file: \"%s\"", e.what());
				Utility::Printf("Notice: ac_config.ini was reinitialized with default AC values.");
				boost::filesystem::remove("ac_config.ini");
				WriteAndLoadDefaultConfig();
			}
		}

		onGameModeInitCalled = true;
		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char* params)
	{
		// If the user typed /actoggle and they're allowed to run that command.
		if (!strcmp(params, "/actoggle") && CAntiCheat::CanEnableAC(playerid))
		{ 
			// Set ACToggle to whatever it wasn't previously.
			ACToggle = !ACToggle;

			// Update property tree with the new value
			ac_config_ptree.put("defaults.main_ac_checks", (ACToggle == true) ? 1 : 0);
			
			// Write to ini file
			boost::property_tree::ini_parser::write_ini("ac_config.ini", ac_config_ptree);

			// Create 2 variables, one to hold the player name who just enabled/disabled AC, and one to send a formatted message
			// telling all the users on the server that the AC is now on and they will be kicked if they're not using it.
			char str[144], name[MAX_PLAYER_NAME];

			// Get the player name
			sampgdk::GetPlayerName(playerid, name, sizeof(name));

			// Format the message telling all the users that AC is now on.
			snprintf(str, sizeof(str), "{FFFFFF}%s has %s{FFFFFF} SAMP_AC_v2.", name, ACToggle == true ? "{35B863}enabled" : "{DE4545}disabled");

			// Send the message to everyone on the server.
			sampgdk::SendClientMessageToAll(-1, str);

			// If the AC was turned on
			if (ACToggle)
			{
				// Delcare the pointer "ac" which will be used to check players connected to the AC in the following FOR loop
				CAntiCheat* ac;

				// Loop through the maximum amount of players that can be connected to the server.
				for (int i = 0; i < MAX_PLAYERS; ++i)
				{
					// If they're not connected to the server, we skip this index.
					if (!sampgdk::IsPlayerConnected(i) || sampgdk::IsPlayerNPC(i))
						continue;
					
					// If they are not connected to the AC
					if (!CAntiCheatHandler::IsConnected(i))
					{
						// Kick them from the server!
						sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)i);
					}
					else // they are connected to the AC
					{
						// Get AC pointer
						ac = CAntiCheatHandler::GetAntiCheat(i);
							
						// Check if it is valid
						if (ac != NULL)
						{
							// Check if the player is banned on AC
							if (ac->AC_IsBanned())
							{
								// Kick the banned player from the server
								sampgdk::Kick(i);
							}
						}
					}
				}
			}
			// We know what command they typed, so return true;
			return true;
		}
		return false;
	}
}