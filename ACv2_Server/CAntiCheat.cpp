#include "CAntiCheat.h"
#include "Utility.h"
#include "GDK/sampgdk.h"
#include "Network/Network.h"
#include "Callback.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "../Shared/Network/CRPC.h"
#include "CServerUpdater.h"
#include "PacketPriority.h"
#include "BanHandler.h"

std::vector<int> CAntiCheat::m_Admins;
std::vector<std::string> CAntiCheat::m_FileNames;
std::vector<std::string> CAntiCheat::m_MD5s;
std::vector<std::string> CAntiCheat::m_ProcessMD5s;

#ifdef WIN32
#define snprintf sprintf_s
#endif

CAntiCheat::CAntiCheat(unsigned int playerid) : ID(playerid)
{
	UpdateCheatList();

	// Set the default values for the variables.
	m_LiteFoot = true;
	m_CBug = 9999;
	m_SwitchReload = true;
	m_FrameLimit = 9999;
	m_UnlimitedSprint = false;
	m_MacroLimits = true;
	m_SprintOnAllSurfaces = false;
	m_VehicleBlips = true;
	m_LastCheatUpdate = 0;
	m_IsConnected = false;
}

CAntiCheat::~CAntiCheat()
{
	// Loop through the list of admins
	for (std::vector<int>::iterator it = m_Admins.begin(); it != m_Admins.end(); )
	{
		// If that iteration is the playerid. If this player is able to toggle AC. If he's an admin.
		if ((*it) == ID)
		{
			/*
				This is important as it fixes a bug that if this player leaves the server while having
				admin power and another player joins afterwards and takes the same ID, they will get
				admin power too without rcon login.

				Remove him from the admin list. Get the next element and store it into 'it'.
			*/
			it = m_Admins.erase(it);
		}
		else
		{
			// If it is not the player we're looking for, iterate!
			++it;
		}
	}
}

void CAntiCheat::UpdateCheatList()
{
	// If the list hasn't been updated in 24 hours...
	if (time(NULL) > m_LastCheatUpdate + 86400)
	{
		// Update our cheat lists!
		m_ProcessMD5s = Cmd5Info::GetBadExecutableFiles();
		m_MD5s = Cmd5Info::GetGtaDirectoryFilesMd5();
		m_FileNames = Cmd5Info::GetGtaDirectoryFilesNames();

		// Set the last update to the current time.
		m_LastCheatUpdate = (int)time(NULL);
	}
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
			// Create 2 variables, one holding the player name, and one holding a formatted string telling why we're going to kick this player.
			char name[MAX_PLAYER_NAME], msg[144];

			// Get the player name and store it in the name variable.
			GetPlayerName(ID, name, sizeof(name));

			// Format the string telling all the players on the server why we kicked this one.
			snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for using an illegal file: \"{FF0000}%s{FFFFFF}\"", name, Utility::GetSafeFilePath(processpath));

			// Send the message to all the players on the server.
			SendClientMessageToAll(-1, msg);

			// Format a proper reason and add cheater to AC global ban list
			snprintf(msg, sizeof msg, "illegal file: %s", processpath);
			BanHandler::AddCheater(ID, msg);

			// Print the result to the console so it can be logged.
			Utility::Printf("%s has been kicked from the server for using illegal file: \"%s\"", name, processpath);

			// Kick the player from the server.
			SetTimer(3000, 0, Callback::KickPlayer, (void*)ID);
		}
	}

	// Execute the PAWN callback.
	Callback::Execute("AC_OnFileExecuted", "issi", found, md5, processpath, ID);
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
				GetPlayerName(ID, name, sizeof(name));

				// Format a new message that tells what happened.
				snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has modified weapon.dat info", name);

				// Send the result to everyone on the server.
				SendClientMessageToAll(-1, msg);

				// And kick the player.
				SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
			}
		}
	}
	// Execute a PAWN callback.
	Callback::Execute("AC_OnMD5Calculated", "siii", md5, size, address, ID);
}

void CAntiCheat::OnFileCalculated(char* path, char* md5)
{	
	/*
			Create a boolean variable to indicate whether a match has been found for this file in our list
		of the MD5 of original/unmodified game files.

			In other words, we will set this variable to TRUE once we find a match (md5 comparison) for it
		in our list of the MD5 of original/unmodified game files. So, setting its value to TRUE means 
		everything is FINE. 
		
			But, if we do not find a match for this file in our trusted list, this means it is a modified 
		file and the value of 'found' will remain FALSE. Hence, FALSE means this is a bad file or a cheat.
	*/
	bool found = false;

	// Loop through a list of our trusted md5's that we stored previously...
	for (std::vector<std::string>::iterator it = m_MD5s.begin(); it != m_MD5s.end(); ++it)
	{
		// Compare the md5 sent to us by the client to our list of trusted MD5's
		if (strcmp(it->c_str(), md5) == 0)
		{
			/*
					Okay, we found a match for our client's file in our trusted list of files. This means
				our client is not cheating and is using original/unmodified game files. So we set the value
				of our variable to TRUE which indicates that everything is alright.

			*/
			found = true;
			break;
		}
	}

	// If AC Main checks are enabled
	if (Callback::GetACEnabled() == true)
	{
		// See if we have not found a match for this file in our trusted list of files. This means it's a bad file.
		if (!found)
		{
			// Create a new variable holding a string that will be formatted to let the player know he's been kicked.
			char msg[160];
			snprintf(msg, sizeof(msg), "{FF0000}Error: {FFFFFF}You've been kicked from this server for having ({FF0000}%s{FFFFFF}) modified.", Utility::GetSafeFilePath(path));

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
	}
	/* 
		Execute PAWN callback.

		# Sidenote
		There's a reason why we send the value of the 'found' variable reversed (i.e  !found).
		If you read the documentation of AC_OnFileCalculated callback, you will realize that the
		value of 'isCheat' boolean variable is TRUE when there is dangerous while 'found' variable
		is completely contrary which is why we reverse values. 
		(Scroll up! Documentation is written where the local variable, 'found', is declared).

	*/
	Callback::Execute("AC_OnFileCalculated", "issi", !found, md5, path, ID);
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
		GetPlayerName(ID, name, sizeof(name));

		// Format the message to send to all players.
		snprintf(msg, sizeof(msg), "{FF0000}%s{FFFFFF} has been kicked from the server for having ({FF0000}%s{FFFFFF}) modified.", name, Utility::GetSafeFilePath(filename));

		// Send the message to all players connected to the server.
		SendClientMessageToAll(-1, msg);

		// Kick the player who has the modified file.
		SetTimer(1000, 0, Callback::KickPlayer, (void*)ID);
	}

	// Execute the PAWN callback.
	Callback::Execute("AC_OnImgFileModifed", "ssi", md5, filename, ID);
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
		for (std::vector<int>::iterator it = m_Admins.begin(); it != m_Admins.end(); )
		{
			// if that iteration is the playerid.
			if ((*it) == playerid)
			{
				// Remove him from the admin list. Get the next element and store it into 'it'.
				it = m_Admins.erase(it);
			}
			else
			{
				// If it is not the player we're looking for, iterate!
				++it;
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

		// Write header
		bsData.Write((unsigned char)PACKET_RPC);
		bsData.Write(MD5_FILE);

		// Write file to packet
		bsData.Write((unsigned short)szFile.length());
		bsData.Write((const char*)szFile.c_str(), szFile.length());

		// Send the data to the client and have them calculate the md5 of that file.
		Network::PlayerSend(playerid, &bsData, LOW_PRIORITY, RELIABLE);
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

	if (Callback::Default_FrameLimit != 9999) SetFPSLimit(Callback::Default_FrameLimit);

	// Execute a PAWN callback telling the server we've just calculated the user's Hardware ID.
	Callback::Execute("AC_OnHardwareIDCalculated", "isi", m_HardwareID.length(), hwid, ID);
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

void CAntiCheat::OnBanChecked(bool status)
{
	// Set our instance variable to the player's ban status so we can store it for later use.
	m_IsBanned = status;
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

		RakNet::BitStream bsData;
		bsData.Write((unsigned char)PACKET_RPC);
		bsData.Write(VERSION_NOT_COMPATIBLE);

		// And send an RPC telling the client's AC not to continue to monitor things.
		Network::PlayerSend(ID, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED);

		// Close the connection.
		Kick(ID);
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

	// Write header
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_LITE_FOOT);

	// meat n potatoes
	bsData.Write(toggle);

	// Send RPC to player.
	Network::PlayerSend(ID, &bsData, LOW_PRIORITY, RELIABLE_ORDERED);

	// Set the lite foot variable to true.
	m_LiteFoot = toggle;
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