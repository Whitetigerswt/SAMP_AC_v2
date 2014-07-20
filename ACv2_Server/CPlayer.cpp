#include "CPlayer.h"
#include "Utility.h"

CPlayer::CPlayer(CClientSocketInfo* socketInfo) : m_pSockInfo(socketInfo)
{

}

CPlayer::~CPlayer()
{
	delete m_pSockInfo;
}

CClientSocketInfo* CPlayer::GetConnectionInfo()
{
	return m_pSockInfo;
}

void CPlayer::OnFileExecuted(char* processpath, char* md5)
{
	Utility::Printf("OnProcessStarted - %s - %s", processpath, md5);
}