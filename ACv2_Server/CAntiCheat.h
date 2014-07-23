#pragma once

#include <string>
#include <vector>
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

	// PURPOSE: Toggles whether the player can turn AC on or off
	// REQUIRES: NULL
	static void ToggleCanEnableAC(int playerid, bool toggle);

	// PURPOSE: Return whether the player can enable AC or not.
	// REQUIRES: NULL
	static bool CanEnableAC(int playerid);

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;
	int ID;
	static std::vector<int> Admins;
};