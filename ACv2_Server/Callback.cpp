#include "Callback.h"
#include "Network/Network.h"
#include "Utility.h"
#include "GDK/a_players.h"
#include "CAntiCheat.h"
#include "CPlayer.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define snprintf sprintf_s
#endif

namespace Callback
{
	static AMX* amx_allowed = NULL;
	static std::list<AMX*> amxPointers;

	static bool ACToggle = false;

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

		delete pReturnValues;

		return iReturnValue;
	}

	void OnACClosed(unsigned int playerid)
	{
		if (IsPlayerConnected(playerid) && ACToggle)
		{
			char str[144], name[MAX_PLAYER_NAME];
			GetPlayerName(playerid, name, sizeof(name));
			snprintf(str, sizeof(str), "{FF0000}%s{FFFFFF} has been kicked from the server (AC Lost Connection)", name);
			Kick(playerid);
		}
	}

	void OnACClosed(std::string ip)
	{
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				char IP[MAX_PLAYER_NAME];
				GetPlayerIp(i, IP, sizeof(IP));

				std::string userip(IP);

				if (ip.compare(userip) == 0)
				{
					OnACClosed(i);
				}
			}
		}
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
	{
		if (Network::HandleConnection(playerid))
		{
			Network::PlayerSend(Network::PACKET_PLAYER_REGISTERED, playerid);
		}

		if (!Network::IsPlayerConnectedToAC(playerid) && ACToggle)
		{
			SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You've been kicked from this server for not running Whitetiger's Anti-Cheat (v2)");

			char msg[160], name[MAX_PLAYER_NAME];
			GetPlayerName(playerid, name, sizeof(name));
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for not running Whitetiger's Anti-Cheat (v2)", name);
			SendClientMessageToAll(-1, msg);

			Utility::Printf("%s has been kicked from the server for not connecting with AC while AC is on.", name);

			Kick(playerid);
		}

		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
	{
		if (Network::IsPlayerConnectedToAC(playerid))
		{
			if (reason)
				Network::PlayerSend(Network::PACKET_PLAYER_PROPER_DISCONNECT, playerid);

			Network::CloseConnection(playerid);
		}
		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit()
	{
		Utility::CheckForUpdate();

		return true;
	}
	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char* params)
	{
		if (!strcmp(params, "/actoggle") && CAntiCheat::CanEnableAC(playerid))
		{
			ACToggle = !ACToggle;

			char str[144], name[MAX_PLAYER_NAME];
			GetPlayerName(playerid, name, sizeof(name));

			snprintf(str, sizeof(str), "{FF0000}%s{0000FF} has {FFFFFF}%s{0000FF} SAMP_AC_v2.", name, ACToggle == true ? "Enabled" : "Disabled");

			SendClientMessageToAll(-1, str);

			for (int i = 0; i < MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i) && !Network::IsPlayerConnectedToAC(i))
				{
					Kick(i);
				}
			}
			return true;
		}
		return false;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnRconLoginAttempt(const char* ip, const char* port, bool success)
	{
		if (!success) return true;

		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			char IP[MAX_PLAYER_NAME];
			GetPlayerIp(i, IP, sizeof(IP));
			if (!strcmp(ip, IP))
			{
				CAntiCheat::ToggleCanEnableAC(i, true);
			}
		}
	}
}