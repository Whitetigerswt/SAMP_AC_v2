#pragma once

#include <string>
#include <vector>
#include "GDK/sampgdk.h"
#include "VersionHelper.h"


class CAntiCheat
{
public:
	CAntiCheat(unsigned int playerid);
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

	// PURPOSE: Callback for when an unknown module is detected sending a packet to server.
	// REQUIRES: The client is using AC.
	void OnUnknownSendPacketCallerFound(unsigned int base, unsigned int addr, unsigned char frame, char* path, char* md5);

	// PURPOSE: Callback for when a file in gta3.img is modified.
	// REQUIRES: The client is using AC.
	void OnImgFileModified(char* filename, char* md5);

	// PURPOSE: Callback for when a player has been detected as using a macro
	// REQUIRES: The client is using AC.
	void OnMacroDetected(int vKey);

	// PURPOSE: Send an RPC to client to md5 files.
	// REQUIRES: The client is using AC.
	static void SAMPGDK_CALL Timer_CheckGTAFiles(int timerid, void *params);
	void CheckGTAFiles();

	// PURPOSE: Do cleanup after all files names have been sent to server or player disconnected while doing so.
	// REQUIRES: The client is using AC.
	void Cleanup_CheckGTAFiles();

	// PURPOSE: Callback for when a player has sent their hardwareID to the server.
	// REQUIRES: The client is using AC.
	void OnHardwareCalculated(char* hwid);

	// PURPOSE: Callback for when the player attempts to tamper with the mod.
	// REQUIRES: The client is using AC.
	void OnTamperAttempt();

	// PURPOSE: Callback for when the player's AC ban status is checked.
	// REQUIRES: The client is using AC.
	void OnBanChecked(bool status);

	// PURPOSE: Check if the player's client version of AC is compatible with the server's version.
	// REQUIRES: The client is using AC.
	void CheckVersionCompatible(CSelfUpdater::stVersion);

	// PURPOSE: Toggles whether the player can turn AC on or off
	// REQUIRES: NULL
	static void ToggleCanEnableAC(int playerid, bool toggle);

	// PURPOSE: Return whether the player can enable AC or not.
	// REQUIRES: NULL
	static bool CanEnableAC(int playerid);

	// PURPOSE: Return the player's hardwareID.
	// REQUIRES: The client is using AC.
	std::string GetPlayerHardwareID() { return m_HardwareID; }

	// PURPOSE: Set a players frame limit.
	// REQUIRES: The client is using AC.
	void SetFPSLimit(int newlimit);

	// PURPOSE: Get the players frame limit.
	// REQUIRES: NULL
	int GetFPSLimit() { return m_FrameLimit; }

	// PURPOSE: Set the players crouch bug status.
	// REQUIRES: The client is using AC.
	void ToggleCrouchBug(unsigned short toggle);

	// PURPOSE: Get the players crouch bug status.
	// REQUIRES: NULL
	int GetCrouchBug() { return m_CBug; }

	// PURPOSE: Set the players switch reload status.
	// REQUIRES: The client is using AC.
	void ToggleSwitchReload(bool toggle);

	// PURPOSE: Get the players switch reload status
	// REQUIRES: NULL
	bool GetSwitchReload() { return m_SwitchReload; }

	// PURPOSE: Set the players unlimited sprint.
	// REQUIRES: The client is using AC.
	void ToggleUnlimitedSprint(bool toggle);

	// PURPOSE: Get the players unlimited sprint
	// REQUIRES: NULL
	bool GetUnlimitedSprint() { return m_UnlimitedSprint; }

	// PURPOSE: Set the players macro limitations.
	// REQUIRES: The client is using AC.
	void ToggleMacroLimitations(bool toggle);

	// PURPOSE: Get the players Macro limitation.
	// REQUIRES: NULL
	bool GetMacroLimitations() { return m_MacroLimits; }

	// PURPOSE: Set the player's sprint speed limit.
	// NOTES: Speed hook is disabled if speed = 0.
	// REQUIRES: The client is using AC.
	void SetSprintLimit(float speed);

	// PURPOSE: Get the player's sprint speed limit.
	// REQUIRES: The client is using AC.
	float GetSprintLimit() { return m_SprintLimit; }

	// PURPOSE: Set the players macro limitations.
	// REQUIRES: The client is using AC.
	void ToggleSprintOnAllSurfaces(bool toggle);

	// PURPOSE: Get the players Macro limitation.
	// REQUIRES: NULL
	bool GetSprintOnAllSurfaces() { return m_SprintOnAllSurfaces; }

	// PURPOSE: Set the player's vehicle blips.
	// REQUIRES: The client is using AC.
	void ToggleVehicleBlips(bool toggle);

	// PURPOSE: Get the player's vehicle blip status.
	// REQUIRES: NULL
	bool GetVehicleBlips() { return m_VehicleBlips; }

	// PURPOSE: Get the player's AC ban status.
	// REQUIRES: NULL
	short AC_GetBanStatus() { return m_BanStatus; }

	// PURPOSE: Check whether player is banned (unretrieved ban status is taken as NOT BANNED).
	// REQUIRES: NULL
	bool AC_IsBanned() { return (m_BanStatus == 1); }

	// PURPOSE: Called when the player pauses or unpauses, or alt tabs from the game.
	// REQUIRES: NULL
	void TogglePause(int iType, bool bPause);

	// PURPOSE: Called when a player takes a screenshot of their game with F8.
	// REQUIRES: NULL
	void OnScreenshotTaken();

	// PURPOSE: Get player's ID.
	// REQUIRES: NULL
	int GetID() { return ID; }

	// PURPOSE: Verify if client is authentic.
	// REQUIRES: NULL
	void SendVerificationPacket();

	// PURPOSE: Check whether connection tick is valid or not.
	// REQUIRES: NULL
	bool IsCreationTickValid();

	// PURPOSE: Fetch information about cheats on server startup.
	// REQUIRES: NULL
	static void UpdateCheatDatabase();

	void SetCheckGTAFilesCurrentFileId(size_t id) { m_CheckGTAFilesCurFileId = id; }
	size_t GetCheckGTAFilesCurrentFileId() { return m_CheckGTAFilesCurFileId; }

	static std::vector<std::string> GetScannedFilenames() { return m_FileNames; }
	static void SetScannedFilenames(std::vector<std::string> filenames) { m_FileNames = filenames; }

	static std::vector<std::string> GetScannedFilesMD5s() { return m_MD5s; }
	static void SetScannedFilesMD5s(std::vector<std::string> md5s) { m_MD5s = md5s; }

	static std::vector<std::string> GetScannedProcessesMD5s() { return m_ProcessMD5s; }
	static void SetScannedProcessesMD5s(std::vector<std::string> processesmd5s) { m_ProcessMD5s = processesmd5s; }

private:

	// PURPOSE: Remember the player's playerID.
	int ID;

	// PURPOSE: Keep track of the player's cbug status.
	unsigned short m_CBug;

	// PURPOSE: Keep track of the player's switch reload status.
	bool m_SwitchReload;

	// PURPOSE: Keep track of the player's set frame rate.
	int m_FrameLimit;

	// PURPOSE: Keep track of the player's unlimited sprint.
	bool m_UnlimitedSprint;
	
	// PURPOSE: Keep track of the player's macro limits.
	bool m_MacroLimits;

	// PURPOSE: Keep track of the player's sprint speed limit.
	float m_SprintLimit;

	// PURPOSE: Keep track of the player's status sprinting on all surfaces.
	bool m_SprintOnAllSurfaces;

	// PURPOSE: Keep track of player's status of vehicle blips.
	bool m_VehicleBlips;

	// PURPOSE: Keep track of player's ban status on Anti-Cheat.
	short m_BanStatus;

	// PURPOSE: Used to validate connection
	unsigned int m_CreationTick;
	static unsigned int m_MaxCreationTickDifference;

	// PURPOSE: Store timer id for GTA Files check.
	int m_CheckGTAFilesTimerId;

	// PURPOSE: Store index of current file (m_FileNames)
	size_t m_CheckGTAFilesCurFileId;

	// PURPOSE: Store the player's hardware ID.
	std::string m_HardwareID;

	// PURPOSE: a static list of all the players allowed to use the /actoggle command, and other AC options
	static std::vector<int> m_Admins;

	// PURPOSE: Hold all the file names that we scan in the gta directory everytime a player connects.
	static std::vector<std::string> m_FileNames;

	// PURPOSE: A list of md5's of files that should be found in the player's GTA directory
	static std::vector<std::string> m_MD5s;

	// PURPOSE: A list of processes that are not allowed to be used while the AC is enabled.
	static std::vector<std::string> m_ProcessMD5s;
};