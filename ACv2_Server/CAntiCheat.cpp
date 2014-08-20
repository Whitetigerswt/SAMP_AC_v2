#include "CAntiCheat.h"
#include "Utility.h"
#include "GDK/a_players.h"
#include "Network/Network.h"
#include "Callback.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "../Shared/Network/CRPC.h"
#include "CServerUpdater.h"

std::vector<int> CAntiCheat::m_Admins;
std::vector<std::string> CAntiCheat::m_FileNames;
std::vector<std::string> CAntiCheat::m_MD5s;
std::vector<std::string> CAntiCheat::m_ProcessMD5s;

#ifdef WIN32
#define snprintf sprintf_s
#endif

CAntiCheat::CAntiCheat(CClientSocketInfo* socketInfo, unsigned int playerid) : m_pSockInfo(socketInfo), ID(playerid)
{

	// If m_FileNames is empty, that's a problem.
	if (m_FileNames.empty())
	{
		// m_FileNames should never be empty.
		m_FileNames = Cmd5Info::GetGtaDirectoryFilesNames();
	}

	// Now, check the md5's list for the same thing.
	if (m_MD5s.empty())
	{
		// Again, it should never be empty.
		m_MD5s = Cmd5Info::GetGtaDirectoryFilesMd5();
	}

	// We need to do the same for the list of bad processes too.
	if (m_ProcessMD5s.empty())
	{
		// Get the list of bad processes from the internet.
		m_ProcessMD5s = Cmd5Info::GetBadExecutableFiles();
	}

	// Set the default values for the variables.
	m_LiteFoot = false;
	m_CBug = 9999;
	m_SwitchReload = true;
	m_FrameLimit = 9999;
	m_UnlimitedSprint = false;
}

CAntiCheat::~CAntiCheat()
{
	delete m_pSockInfo;
}

CClientSocketInfo* CAntiCheat::GetConnectionInfo()
{
	return m_pSockInfo;
}


void CAntiCheat::OnFileExecuted(char* processpath, char* md5)
{
	// Loop through the list of bad processes to see if we can find a match to the one just sent to us by the client.
	for (std::vector<std::string>::iterator it = m_ProcessMD5s.begin(); it != m_ProcessMD5s.end(); ++it)
	{
		// If the md5 matches one of the md5's from our list of bad md5's
		if (it->compare(md5) == 0)
		{
			// Create 2 variables, one holding the player name, and one holding a formatted string telling why we're going to kick this player.
			char name[MAX_PLAYER_NAME], msg[144];

			// Get the player name and store it in the name variable.
			GetPlayerName(ID, name, sizeof(name));

			// Format the string telling all the players on the server why we kicked this one.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for using an illegal file: \"{FF0000}%s{FFFFFF}\"", name, processpath);

			// Send the message to all the players on the server.
			SendClientMessageToAll(-1, msg);

			// Print the result to the console so it can be logged.
			Utility::Printf("%s has been kicked from the server for using illegal file: \"%s\"", name, processpath);

			// Kick the player from the server.
			SetTimer(3000, 0, Callback::KickPlayer, (void*)ID);
		}
	}
	// Execute the PAWN callback.
	Callback::Execute("OnFileExecuted", "ssi", md5, processpath, ID);
}

void CAntiCheat::OnMD5Calculated(int address, int size, char* md5)
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
			GetPlayerName(ID, name, sizeof(name));

			// Format a new message that tells what happened.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has modified weapon.dat info", name);

			// Send the result to everyone on the server.
			SendClientMessageToAll(-1, msg);

			// And kick the player.
			SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
		}
	}

	// This address is some handling.cfg address, though it seems to include code to and not just data.
	/*else if (address == 0xC2B9DC)
	{
		// Compare the returned md5 to our pre-calculated checksum.
		if (strcmp(md5, "12c0520d9b5442fbecccfa81ebbf2603") != 0)
		{
			// Create 2 new variables, one to hold the player name, and one to send a formatted message to everyone on the server.
			char msg[144], name[MAX_PLAYER_NAME];

			// Get the player name and store it in the name variable.
			GetPlayerName(ID, name, sizeof(name));

			// Format the message that we're going to send to everyone.
			snprintf(msg, sizeof(msg), "[TEST] %s has modified handling.cfg info", name);

			// Send the mssage to everyone connected to the server.
			SendClientMessageToAll(-1, msg);
		}
	}*/

	// Execute a PAWN callback.
	Callback::Execute("OnMD5Calculated", "siii", md5, size, address, ID);
}

void CAntiCheat::OnFileCalculated(char* path, char* md5)
{	
	// Create a new variable that can contain a true/false value so we know if a file matches an MD5 from our list
	bool found = false;

	// Loop through a list of our md5's that we stored previously...
	for (std::vector<std::string>::iterator it = m_MD5s.begin(); it != m_MD5s.end(); ++it)
	{
		// Compare the md5 sent to us by the client to our list of MD5's
		if (strcmp(it->c_str(), md5) == 0)
		{
			// If they match, set found=true and break
			found = true;
			break;
		}
	}

	// Check if an md5 matches, and if it doesn't kick the player.
	if (!found)
	{
		// Create a new variable holding a string that will be formatted to let the player know he's been kicked.
		char msg[160];
		snprintf(msg, sizeof(msg), "{FF0000}Error: {FFFFFF}You've been kicked from this server for having ({FF0000}%s{FFFFFF}) modified.", path);

		// Send the formatted message to the player.
		SendClientMessage(ID, -1, msg);

		// Now, we need to send a message to the whole server saying someone was kicked, and we need to include their name
		// So create a variable that can hold their name.
		char name[MAX_PLAYER_NAME];

		// Find their name.
		GetPlayerName(ID, name, sizeof(name));

		// Format the string telling all the users this player has been kicked.
		snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for having ({FF0000}%s{FFFFFF}) modified.", name, path);
		
		// Send it to everyone
		SendClientMessageToAll(-1, msg);

		// Finally, print our a message to the console so we can log the result.
		Utility::Printf("%s has been kicked for modifying %s", name, path);

		// And kick the player.
		SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
	}

	// Execute PAWN callback.
	Callback::Execute("OnFileCalculated", "ssi", md5, path, ID);
}

void CAntiCheat::OnImgFileModified(char* filename, char* md5)
{
	// We already know the file is modified, so we don't need to check for that.

	// Create 2 variables, one to hold the player name and one to send a formatted message to the whole server telling them what happened.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name and store it in the variable we just created.
	GetPlayerName(ID, name, sizeof(name));

	// Format the message to send to all players.
	snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for having ({FF0000}%s{FFFFFF}) modified.", name, filename);

	// Send the message to all players connected to the server.
	SendClientMessageToAll(-1, msg);

	// Kick the player who has the modified file.
	SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);

	// Execute the PAWN callback.
	Callback::Execute("OnImgFileModifed", "ssi", md5, filename, ID);
}

bool CAntiCheat::CanEnableAC(int playerid)
{
	// Loop through the list of admins that we added with ToggleCanEnableAC.
	for (std::vector<int>::iterator it = m_Admins.begin(); it != m_Admins.end(); ++it)
	{
		// If it matches the current playerid passed in the parameter, return true.
		if ((*it) == playerid) return true;
	}

	// Else, return false.
	return false;
}

void CAntiCheat::ToggleCanEnableAC(int playerid, bool toggle)
{
	// If toggle is true
	if (toggle)
	{
		// Add that user to the admin list.
		m_Admins.push_back(playerid);

		return;
	}
	else
	{
		// If toggle is false
		// Loop through the list of admins
		for (std::vector<int>::iterator it = m_Admins.begin(); it != m_Admins.end(); ++it)
		{
			// if that iteration is the playerid.
			if ((*it) == playerid)
			{
				// Remove him from the admin list
				m_Admins.erase(it);
			}
		}
		return;
	}
}

void CAntiCheat::CheckGTAFiles(int playerid)
{
	// Loop through the files we need to send to the client for him/her to check.
	for (std::vector<std::string>::iterator it = m_FileNames.begin(); it != m_FileNames.end(); ++it)
	{
		// Create a new string that will hold the final file value, preceeded by the macro $(GtaDirectory)
		std::string szFile("$(GtaDirectory)/");

		// Add the file name from the m_FileNames vector (containing all the file names to check in the gta directory).
		szFile.append(it->c_str());

		// Prepare to send the data to the client.
		RakNet::BitStream bsData;
		bsData.WriteCasted<unsigned char*, const char*>(szFile.c_str());

		// Send the data to the client and have them calculate the md5 of that file.
		Network::PlayerSendRPC(MD5_FILE, playerid, &bsData);
	}
}

void CAntiCheat::OnMacroDetected(int vKey)
{
	// Create 2 variables, one to hold the player name and one to hold a string formatted to send all players on the server.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name and store it in the name variable
	GetPlayerName(ID, name, sizeof(name));

	// Format the string telling all the users what happened.
	snprintf(msg, sizeof(msg), "[TEST] %s is using a macro? (vKey=0x%x)", name, vKey);

	// Send the message to all connected players
	SendClientMessageToAll(-1, msg);
}

void CAntiCheat::OnHardwareCalculated(char* hwid)
{
	// Set our instance variable to the player's hardware ID so we can store it for later use.
	m_HardwareID = hwid;

	// Create 2 variables, one for the player's name and one for a formatted message sent to everyone on the server.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name in our name variable.
	GetPlayerName(ID, name, sizeof(name));

	// Format the message to let everyone know his hardware ID.
	snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF}'s HardwareID: {FF0000}%s", name, m_HardwareID.c_str());

	// Send the message to everyone on the server.
	SendClientMessageToAll(-1, msg);
}

void CAntiCheat::OnTamperAttempt()
{
	// Create 2 new variables, one to hold the players name and one to send a formatted message to the rest of the players on the server.
	char name[MAX_PLAYER_NAME], msg[144];

	// Get the player name and store it in the name variable.
	GetPlayerName(ID, name, sizeof(name));

	// Format the message that will be sent to everyone on the server.
	snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} is trying to tamper with the AC mod.", name);

	// Send the message to everyone on the server.
	SendClientMessageToAll(-1, msg);

	// Print the message to the console as well
	Utility::Printf(msg);

	// Kick the player.
	SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
}

void CAntiCheat::CheckVersionCompatible(float version)
{
	// Check if the version is incompatible with the server version.
	if (version > CURRENT_MAJOR_VERSION)
	{
		// Inform the player there version of AC is not compatible with the server.
		char msg[150];

		// Format the message letting the user know their AC version will not work on this server.
		snprintf(msg, sizeof(msg), "{FF0000}Fatal Error:{FFFFFF} The servers Anti-Cheat plugin is not compatible with your version. IT WILL NOT WORK ON THIS SERVER.");

		// Send the message to the user.
		SendClientMessage(ID, -1, msg);

		// And send an RPC telling the client's AC not to continue to monitor things.
		Network::PlayerSendRPC(VERSION_NOT_COMPATIBLE, ID);

		// Close the connection.
		Network::CloseConnection(ID);
	}
}

void CAntiCheat::SetFPSLimit(int newlimit)
{
	// Update the frame limit the server keeps.
	m_FrameLimit = newlimit;
}

void CAntiCheat::ToggleLiteFoot(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSendRPC(TOGGLE_LITE_FOOT, ID, &bsData);

	// Set the lite foot variable to true.
	m_LiteFoot = toggle;
}

void CAntiCheat::ToggleCrouchBug(int toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSendRPC(TOGGLE_CROUCH_BUG, ID, &bsData);

	// Set the crouch bug variable to true.
	m_CBug = toggle;
}

void CAntiCheat::ToggleSwitchReload(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSendRPC(TOGGLE_SWITCH_RELOAD, ID, &bsData);

	// Set the crouch bug variable to true.
	m_SwitchReload = toggle;
}

void CAntiCheat::ToggleUnlimitedSprint(bool toggle)
{
	// Prepare to send RPC to client.
	RakNet::BitStream bsData;
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSendRPC(TOGGLE_UNLIMITED_SPRINT, ID, &bsData);

	// Set the crouch bug variable to true.
	m_UnlimitedSprint = toggle;
}

void CAntiCheat::TogglePause(int iType, bool bPause)
{
	// Let PAWN scripts know.
	Callback::Execute("OnPlayerPause", "iii", bPause, iType, ID);
}

void CAntiCheat::OnScreenshotTaken()
{
	// Let PAWN scripts know.
	Callback::Execute("OnScreenshotTaken", "i", ID);
}