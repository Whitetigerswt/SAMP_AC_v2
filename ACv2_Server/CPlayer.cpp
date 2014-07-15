#include "CPlayer.h"

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