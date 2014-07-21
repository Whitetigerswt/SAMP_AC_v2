#include "CAntiCheat.h"
#include "Utility.h"
#include "GDK\a_players.h"
#include "Network\Network.h"

CAntiCheat::CAntiCheat(CClientSocketInfo* socketInfo, unsigned int playerid) : m_pSockInfo(socketInfo), ID(playerid)
{

}

CAntiCheat::~CAntiCheat()
{
	delete m_pSockInfo;
}

CClientSocketInfo* CAntiCheat::GetConnectionInfo()
{
	return m_pSockInfo;
}

/*void CAntiCheat::SetID()
{
	ID = Network::GetPlayeridFromSystemAddress(m_pSockInfo->GetSystemAddress());
}*/

void CAntiCheat::OnFileExecuted(char* processpath, char* md5)
{
	Utility::Printf("OnProcessStarted - %s - %s", processpath, md5);
}
/*
std::string CAntiCheat::GetName()
{
	char name[MAX_PLAYER_NAME];
	GetPlayerName(GetID(), name, sizeof(name));

	std::string thename(name);
	return thename;
}

void CAntiCheat::KickPlayer()
{
	Kick(GetID());
}*/