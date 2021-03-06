#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"
#include "Utility.h"
#include "GDK/sampgdk.h"
#include "Network/Network.h"
#include "Callback.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "../Shared/Network/CRPC.h"
#include "PacketPriority.h"
#include "BanHandler.h"
#include "VerifiedPacketChecker.h"
#include <ctime>
#include <cstring>
#include <stdio.h>
#include <boost/thread.hpp>

std::vector<int> CAntiCheat::m_Admins;
std::vector<std::string> CAntiCheat::m_FileNames;
std::vector<std::string> CAntiCheat::m_MD5s;
std::vector<std::string> CAntiCheat::m_ProcessMD5s;
unsigned int CAntiCheat::m_MaxCreationTickDifference = 2000;

#ifdef WIN32
#define snprintf sprintf_s
#endif

CAntiCheat::CAntiCheat(unsigned int playerid) : ID(playerid)
{
	// Set the default values for the variables.
	m_CBug = 9999;
	m_SwitchReload = true;
	m_FrameLimit = 9999;
	m_UnlimitedSprint = false;
	m_MacroLimits = true;
	m_SprintOnAllSurfaces = false;
	m_VehicleBlips = true;
	m_SprintLimit = 0.0f;
	m_BanStatus = -1;
	m_CreationTick = Utility::getTickCount();
	m_CheckGTAFilesTimerId = 0;
}

CAntiCheat::~CAntiCheat()
{
	// Kill check timer (if exists)
	Cleanup_CheckGTAFiles();
}

void CAntiCheat::OnFileExecuted(char* processpath, char* md5)
{
	// Create new variable that will hold if this .exe is a bad exe.
	bool found = false;

	// Loop through the list of bad processes to see if we can find a match to the one just sent to us by the client.
	for (std::vector<std::string>::iterator it = m_ProcessMD5s.begin(); it != m_ProcessMD5s.end(); ++it)
	{
		// If the md5 matches one of the md5's from our list of bad md5's
		if (it->compare(md5) == 0)
		{
			found = true;
			break;
		}
	}

	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		// Loop through the list of bad processes to see if we can find a match to the one just sent to us by the client.
		if(found)
		{
			// Add cheater to AC global ban list
			BanHandler::AddCheater(ID, std::string(processpath), std::string(md5));
		}
	}

	// Execute the PAWN callback.
	Callback::Execute("AC_OnFileExecuted", "ssi", md5, processpath, ID);
}

void CAntiCheat::OnMD5Calculated(int address, int size, char* md5)
{
	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		// The start of the weapon.dat block of HITMAN skills only.
		if (address == 0xC8C418)
		{
			// Compare the md5 returned to a pre-calculated checksum of that memory block.
			if (strcmp(md5, "af82edadc0d8d2f6488e8dc615c34627") != 0)
			{
				// Create 2 new variables, one to hold the name and one to send a test message to all the players on the server.
				char msg[144], name[MAX_PLAYER_NAME];

				// Get the player name and store it in the name variable.
				sampgdk::GetPlayerName(ID, name, sizeof(name));

				// Format a new message that tells what happened.
				snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has modified weapon.dat info", name);

				// Send the result to everyone on the server.
				sampgdk::SendClientMessageToAll(-1, msg);

				// And kick the player.
				sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
			}
		}
	}
	// Execute a PAWN callback.
	Callback::Execute("AC_OnMD5Calculated", "siii", md5, size, address, ID);
}

void CAntiCheat::OnFileCalculated(char* path, char* md5)
{	
	// Indicate whether this file is modified or not.
	bool isModified = true;

	// Loop through a list of our trusted md5's that we stored previously...
	for (std::vector<std::string>::iterator it = m_MD5s.begin(); it != m_MD5s.end(); ++it)
	{
		// Compare the md5 sent to us by the client to our list of trusted MD5's
		if (strcmp(it->c_str(), md5) == 0)
		{
			// File exists in our trusted list, so it is not modified
			isModified = false;
			break;
		}
	}

	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		// See if we have not found a match for this file in our trusted list of files. This means it's a bad file.
		if (isModified)
		{
			// Create a new variable holding a string that will be formatted to let the player know he's been kicked.
			char msg[160];
			snprintf(msg, sizeof(msg), "{FF0000}Error: {FFFFFF}You've been kicked from this server for having ({FF0000}%s{FFFFFF}) modified.", Utility::GetSafeFilePath(path));

			// Send the formatted message to the player.
			sampgdk::SendClientMessage(ID, -1, msg);

			// Now, we need to send a message to the whole server saying someone was kicked, and we need to include their name
			// So create a variable that can hold their name.
			char name[MAX_PLAYER_NAME];

			// Find their name.
			sampgdk::GetPlayerName(ID, name, sizeof(name));

			// Format the string telling all the users this player has been kicked.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for having ({FF0000}%s{FFFFFF}) modified.", name, path);

			// Send it to everyone
			sampgdk::SendClientMessageToAll(-1, msg);

			// Finally, print our a message to the console so we can log the result.
			Utility::Printf("%s has been kicked for modifying %s", name, path);

			// And kick the player.
			sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
		}
	}
	
	// Execute PAWN callback.
	Callback::Execute("AC_OnFileCalculated", "issi", isModified, md5, path, ID);
}

void CAntiCheat::OnUnknownSendPacketCallerFound(unsigned int base, unsigned int addr, unsigned char frame, char* path, char* md5)
{
	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		char name[MAX_PLAYER_NAME];
		sampgdk::GetPlayerName(ID, name, sizeof(name));

		Utility::Printf("%s - packet tampering, frame: %d, base: %x, addr: %x, path: %s, md5: %s.", name, frame, base, addr, path, md5);

		if (Callback::Default_KickPacketTampering)
		{
			// Create a new variable holding a string that will be formatted to let the player know he's been kicked.
			char msg[160];

			// Send the formatted message to the player.
			sampgdk::SendClientMessage(ID, -1, "{FF0000}Error: {FFFFFF}You've been kicked from this server for packet tampering.");

			// Format the string telling all the users this player has been kicked.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for packet tampering.", name);

			// Send it to everyone
			sampgdk::SendClientMessageToAll(-1, msg);

			// And kick the player.
			sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
		}
	}

	// Execute PAWN callback.
	Callback::Execute("AC_OnPacketTampering", "i", ID);
}

void CAntiCheat::OnImgFileModified(char* filename, char* md5)
{
	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		// We already know the file is modified, so we don't need to check for that.
		// Create 2 variables, one to hold the player name and one to send a formatted message to the whole server telling them what happened.
		char name[MAX_PLAYER_NAME], msg[144];

		// Get the player name and store it in the variable we just created.
		sampgdk::GetPlayerName(ID, name, sizeof(name));

		// Format the message to send to all players.
		snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for having ({FF0000}%s{FFFFFF}) modified.", name, Utility::GetSafeFilePath(filename));

		// Send the message to all players connected to the server.
		sampgdk::SendClientMessageToAll(-1, msg);

		// Kick the player who has the modified file.
		sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
	}

	// Execute the PAWN callback.
	Callback::Execute("AC_OnImgFileModifed", "ssi", md5, filename, ID);
}

void CAntiCheat::ToggleCanEnableAC(int playerid, bool toggle)
{
	if (toggle)
	{
		m_Admins.push_back(playerid);
	}
	else
	{
		m_Admins.erase(std::remove(m_Admins.begin(), m_Admins.end(), playerid), m_Admins.end());
	}
}


bool CAntiCheat::CanEnableAC(int playerid)
{
	return (sampgdk::IsPlayerAdmin(playerid) || m_Admins.end() != std::find(m_Admins.begin(), m_Admins.end(), playerid));
}

void SAMPGDK_CALL CAntiCheat::Timer_CheckGTAFiles(int timerid, void *params)
{
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(reinterpret_cast<int>(params));
	if (ac == NULL)
	{
		sampgdk::KillTimer(timerid);
		return;
	}
	size_t curFileIdx = ac->GetCheckGTAFilesCurrentFileId();

	if (curFileIdx >= m_FileNames.size())
	{
		ac->Cleanup_CheckGTAFiles();
		return;
	}

	// Create a new string that will hold the final file value, preceeded by the macro $(GtaDirectory)
	std::string szFile("$(GtaDirectory)/");

	// Add the file name from the m_FileNames vector (containing all the file names to check in the gta directory).
	szFile.append(m_FileNames[curFileIdx].c_str());

	// Prepare to send the data to the client.
	RakNet::BitStream bsData;

	// Write header
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(MD5_FILE);

	// Write file to packet
	bsData.Write((unsigned short)szFile.length());
	bsData.Write((const char*)szFile.c_str(), szFile.length());

	// Send the data to the client and have them calculate the md5 of that file.
	Network::PlayerSend(ac->ID, &bsData, LOW_PRIORITY, RELIABLE);

	ac->SetCheckGTAFilesCurrentFileId(curFileIdx + 1);
}

void CAntiCheat::CheckGTAFiles()
{
	// make sure we won't have 2 checks running in parallel
	Cleanup_CheckGTAFiles();

	SetCheckGTAFilesCurrentFileId(0);
	m_CheckGTAFilesTimerId = sampgdk::SetTimer(150, true, Timer_CheckGTAFiles, (void*)ID);
}

void CAntiCheat::Cleanup_CheckGTAFiles()
{
	sampgdk::KillTimer(m_CheckGTAFilesTimerId);
	m_CheckGTAFilesTimerId = 0;
}

void CAntiCheat::OnMacroDetected(int vKey)
{
	// Create 2 variables, one to hold the player name and one to hold a string formatted to send all players on the server.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name and store it in the name variable
	sampgdk::GetPlayerName(ID, name, sizeof(name));

	// Format the string telling all the users what happened.
	snprintf(msg, sizeof(msg), "[TEST] %s is using a macro? (vKey=0x%x)", name, vKey);

	// Send the message to all connected players
	sampgdk::SendClientMessageToAll(-1, msg);
}

void CAntiCheat::OnHardwareCalculated(char* hwid)
{
	// Set our instance variable to the player's hardware ID so we can store it for later use.
	m_HardwareID = hwid;

	if (Callback::Default_FrameLimit != 9999) SetFPSLimit(Callback::Default_FrameLimit);

	// Execute a PAWN callback telling the server we've just calculated the user's Hardware ID.
	Callback::Execute("AC_OnHardwareIDCalculated", "isi", m_HardwareID.length(), hwid, ID);
}

void CAntiCheat::OnTamperAttempt()
{
	// Create 2 new variables, one to hold the players name and one to send a formatted message to the rest of the players on the server.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name and store it in the name variable.
	sampgdk::GetPlayerName(ID, name, sizeof(name));

	// Format the message that will be sent to everyone on the server.
	snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} is trying to tamper with the AC mod.", name);

	// Send the message to everyone on the server.
	sampgdk::SendClientMessageToAll(-1, msg);

	// Print the message to the console as well
	Utility::Printf(msg);

	// Kick the player.
	sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
}

void CAntiCheat::OnBanChecked(bool status)
{
	// Set our instance variable to the player's ban status so we can store it for later use.
	m_BanStatus = status;

	// Notify scripts about player's ban status
	Callback::Execute("AC_OnBanStatusRetrieved", "ii", status, ID);

	if (m_BanStatus)
	{
		// This player is a cheater and has been banned before. 
		if (Callback::GetACEnabled())
		{
			// AC is enabled. Kick the banned player.
			char msg[144];

			// Tell the player
			snprintf(msg, sizeof msg, "{FF0000}Anti-Cheat (v2): {FFFFFF}You're banned. Know more: %s", AC_WEBSITE);
			sampgdk::SendClientMessage(this->GetID(), -1, msg);
			char name[MAX_PLAYER_NAME];
			sampgdk::GetPlayerName(this->GetID(), name, sizeof name);

			// Tell other players connected
			snprintf(msg, sizeof msg, "{FFFFFF}%s {FF0000}has been kicked for being banned from AC servers.", name);
			sampgdk::SendClientMessageToAll(-1, msg);

			// Kick the player from the server
			sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)this->GetID());
		}
	}
}

void CAntiCheat::CheckVersionCompatible(CSelfUpdater::stVersion version)
{
	// Check if the version is incompatible with the server version.
	if (!VersionHelper::IsClientCompatible(version))
	{
		// Inform the player there version of AC is not compatible with the server.
		char msg[144];

		sampgdk::SendClientMessage(ID, 0xFF0000FF, "Fatal Error:{FFFFFF} The server's anti-cheat plugin is not compatible with your version.");
		snprintf(msg, sizeof(msg), "Fatal Error:{FFFFFF} Server version: v%d.%02d, your client version: v%d.%02d. You must update your anti-cheat at https://whitetigerswt.github.io/SAMP_AC_v2", VersionHelper::AC_SERVER_VERSION.major, VersionHelper::AC_SERVER_VERSION.minor, version.major, version.minor);
		sampgdk::SendClientMessage(ID, 0xFF0000FF, msg);

		// Close the connection.
		sampgdk::SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
	}
}

void CAntiCheat::SetFPSLimit(int newlimit)
{
	// Update the frame limit the server keeps.
	m_FrameLimit = newlimit;
}

void CAntiCheat::ToggleCrouchBug(unsigned short toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_CROUCH_BUG);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the crouch bug variable to true.
	m_CBug = toggle;
}

void CAntiCheat::ToggleSwitchReload(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_SWITCH_RELOAD);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the crouch bug variable to true.
	m_SwitchReload = toggle;
}

void CAntiCheat::ToggleUnlimitedSprint(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_UNLIMITED_SPRINT);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the sprint variable to true.
	m_UnlimitedSprint = toggle;
}

void CAntiCheat::ToggleMacroLimitations(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_MACRO_LIMITS);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the crouch bug variable to true.
	m_MacroLimits = toggle;
}

void CAntiCheat::ToggleSprintOnAllSurfaces(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_SPRINT_ALL_SURFACES);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the crouch bug variable to true.
	m_SprintOnAllSurfaces = toggle;
}

void CAntiCheat::SetSprintLimit(float speed)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(SET_SPRINT_LIMIT);

	// Main data
	bsData.Write(speed);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the crouch bug variable to true.
	m_SprintLimit = speed;
}

void CAntiCheat::ToggleVehicleBlips(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_VEHICLE_BLIPS);

	// Main data
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the vehicle blips variable.
	m_VehicleBlips = toggle;
}

void CAntiCheat::TogglePause(int iType, bool bPause)
{
	// Let PAWN scripts know.
	Callback::Execute("AC_OnPlayerPause", "iii", bPause, iType, ID);
}

void CAntiCheat::OnScreenshotTaken()
{
	// Let PAWN scripts know.
	Callback::Execute("AC_OnScreenshotTaken", "i", ID);
}

void CAntiCheat::SendVerificationPacket()
{
	VerifiedPacketChecker::VerifyClient(ID);
}

bool CAntiCheat::IsCreationTickValid()
{
	return Utility::getTickCount() - m_CreationTick < m_MaxCreationTickDifference;
}

void CAntiCheat::UpdateCheatDatabase()
{
	m_FileNames = Cmd5Info::GetGtaDirectoryFilesNames();
	m_MD5s = Cmd5Info::GetGtaDirectoryFilesMd5();
	m_ProcessMD5s = Cmd5Info::GetBadExecutableFiles();

	if (m_FileNames.empty() || m_MD5s.empty() || m_ProcessMD5s.empty())
	{
		Utility::Printf("Failed to retrieve data from website (2)");
	}
}