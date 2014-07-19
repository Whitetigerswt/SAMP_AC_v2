#pragma once

#include <string>
#include "Network/CClientSocketInfo.h"

class CPlayer
{
public:
	CPlayer(CClientSocketInfo* socketInfo);
	~CPlayer();
	void OnProcessStarted(char* processpath, char* md5);
	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;

};