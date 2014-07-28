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

	// PURPOSE: Callback for when a file in the gta_sa path's md5 is calculated.
	// REQUIRES: The client is using AC.
	void OnFileCalculated(char* path, char* md5);

	// PURPOSE: Callback for when a file in gta3.img is modified.
	// REQUIRES: The client is using AC.
	void OnImgFileModified(char* filename, char* md5);

	// PURPOSE: Callback for when a player has been detected as using a macro
	// REQUIRES: The client is using AC.
	void OnMacroDetected(int vKey);

	// PURPOSE: Send an RPC to client to md5 files.
	// REQUIRES: The client is using AC.
	void CheckGTAFiles(int playerid);

	// PURPOSE: Toggles whether the player can turn AC on or off
	// REQUIRES: NULL
	static void ToggleCanEnableAC(int playerid, bool toggle);

	// PURPOSE: Return whether the player can enable AC or not.
	// REQUIRES: NULL
	static bool CanEnableAC(int playerid);

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;

	// PURPOSE: Remember the player's playerID.
	int ID;

	// PURPOSE: a static list of all the players allowed to use the /actoggle command, and other AC options
	static std::vector<int> m_Admins;

	// PURPOSE: Hold all the file names that we scan in the gta directory everytime a player connects.
	static std::vector<std::string> m_FileNames;

	// PURPOSE: A list of md5's of files that should be found in the player's GTA directory
	static std::vector<std::string> m_MD5s;

	// PURPOSE: A list of processes that are not allowed to be used while the AC is enabled.
	static std::vector<std::string> m_ProcessMD5s;
};