#pragma once

#include "Network/CClientSocketInfo.h"

class CPlayer
{
public:
	CPlayer(CClientSocketInfo* socketInfo);
	~CPlayer();

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;

};