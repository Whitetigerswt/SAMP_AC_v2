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

	// PURPOSE: Callback for when an MD5 hash has been returned after an MD5_Memory function was called.
	// REQUIRES: The client is using AC.
	void OnMD5Calculated(int address, int size, char* md5);

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;
	int ID;
};