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

	// PURPOSE: Callback for when a player has sent their hardwareID to the server.
	// REQUIRES: The client is using AC.
	void OnHardwareCalculated(char* hwid);

	// PURPOSE: Callback for when the player attempts to tamper with the mod.
	// REQUIRES: The client is using AC.
	void OnTamperAttempt();

	// PURPOSE: Toggles whether the player can turn AC on or off
	// REQUIRES: NULL
	static void ToggleCanEnableAC(int playerid, bool toggle);

	// PURPOSE: Return whether the player can enable AC or not.
	// REQUIRES: NULL
	static bool CanEnableAC(int playerid);

	// PURPOSE: Return the player's hardwareID.
	// REQUIRES: The client is using AC.
	std::string GetPlayerHardwareID() { return HardwareID; }

	// PURPOSE: Set a players frame limit.
	// REQUIRES: The client is using AC.
	void SetFPSLimit(int newlimit);

	// PURPOSE: Get the players frame limit.
	// REQUIRES: NULL
	int GetFPSLimit() { return FrameLimit; }

	CClientSocketInfo* GetConnectionInfo();

private:
	CClientSocketInfo* m_pSockInfo;

	// PURPOSE: Remember the player's playerID.
	int ID;

	// PURPOSE: Keep track of the players set frame rate.
	int FrameLimit = 9999;

	// PURPOSE: Store the player's hardware ID.
	std::string HardwareID;

	// PURPOSE: a static list of all the players allowed to use the /actoggle command, and other AC options
	static std::vector<int> m_Admins;

	// PURPOSE: Hold all the file names that we scan in the gta directory everytime a player connects.
	static std::vector<std::string> m_FileNames;

	// PURPOSE: A list of md5's of files that should be found in the player's GTA directory
	static std::vector<std::string> m_MD5s;

	// PURPOSE: A list of processes that are not allowed to be used while the AC is enabled.
	static std::vector<std::string> m_ProcessMD5s;
};