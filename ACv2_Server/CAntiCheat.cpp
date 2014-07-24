#include "CAntiCheat.h"
#include "Utility.h"
#include "GDK/a_players.h"
#include "Network/Network.h"
#include "Callback.h"

std::vector<int> CAntiCheat::Admins;

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
	
}

void CAntiCheat::OnMD5Calculated(int address, int size, char* md5)
{
	Utility::Printf("OnMD5Calculated - 0x%x - %d - %s", address, size, md5);
	Callback::Execute("OnMD5Calculated", "sii", md5, size, address);
}

void CAntiCheat::OnFileCalculated(char* path, char* md5)
{
	Utility::Printf("OnFileCalculated - %s -%s", path, md5);
}

void CAntiCheat::OnImgFileModified(char* filename, char* md5)
{
	Utility::Printf("OnImgFileModifed - %s %s", filename, md5);
}

bool CAntiCheat::CanEnableAC(int playerid)
{
	for (std::vector<int>::iterator it = Admins.begin(); it != Admins.end(); ++it)
	{
		if ((*it) == playerid) return true;
	}
	return false;
}

void CAntiCheat::ToggleCanEnableAC(int playerid, bool toggle)
{
	if (toggle)
	{
		Admins.push_back(playerid);
		return;
	}
	else
	{
		for (std::vector<int>::iterator it = Admins.begin(); it != Admins.end(); ++it)
		{
			if ((*it) == playerid)
			{
				Admins.erase(it);
			}
		}
		return;
	}
}