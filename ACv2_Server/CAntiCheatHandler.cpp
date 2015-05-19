#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"

std::map<unsigned int, CAntiCheat*> CAntiCheatHandler::players;

CAntiCheat* CAntiCheatHandler::GetAntiCheat(unsigned int playerid)
{
	return players[playerid];
}

void CAntiCheatHandler::Init(unsigned int playerid)
{
	players[playerid] = new CAntiCheat(playerid);
}

void CAntiCheatHandler::Remove(unsigned int playerid)
{
	delete players[playerid];
	players.erase(playerid);
}

bool CAntiCheatHandler::IsConnected(unsigned int playerid)
{
	return players[playerid] != NULL;
}
