#include "CAntiCheat.h"
#include "Utility.h"
#include "GDK\a_players.h"
#include "Network\Network.h"
#include "Callback.h"

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


void CAntiCheat::OnFileExecuted(char* processpath, char* md5)
{
	//Utility::Printf("OnProcessStarted - %s - %s", processpath, md5);
}

void CAntiCheat::OnMD5Calculated(int address, int size, char* md5)
{
	Utility::Printf("OnMD5Calculated - 0x%x - %d - %s", address, size, md5);
	Callback::Execute("OnMD5Calculated", "sii", md5, size, address);
}