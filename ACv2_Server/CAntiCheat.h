#pragma once

#include <string>
#include "Network/CClientSocketInfo.h"

class CAntiCheat
{
public:
	CAntiCheat(CClientSocketInfo* socketInfo, unsigned int playerid);
	~CAntiCheat();

	// PURPOSE: Callback for when a new process or module is detected on the clients system.
	// REQUIERS: The client is using AC.
	void OnFileExecuted(char* processpath, char* md5);

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;
	int ID;
};