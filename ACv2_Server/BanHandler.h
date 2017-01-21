#pragma once

#include <string>

#define AC_BAN_HANDLER_ADD			"https://apples.sampcup.com/addcheater.php"
#define AC_BAN_HANDLER_CHECK		"https://apples.sampcup.com/checkcheater.php"

namespace BanHandler
{
	// PURPOSE: Add a player to the global banlist.
	// NOTES: "reason" must contain the name of cheat used and any extra info (e.g. reason = "modified FILE_NAME")
	void AddCheater(unsigned int playerid, char *reason);

	// PURPOSE: POST data (in another thread)
	void Thread_AddCheater(unsigned int playerid, std::string reason, std::string hwid, std::string name, std::string ip, std::string server_name, int server_port);

	// PURPOSE: Check whether a player is in the global banlist or not.
	// NOTES: The result is given in CThreadSync::OnCheaterCheckResponse
	void CheckCheater(unsigned int playerid);

	// PURPOSE: Request information regarding playerid cheater status from webserver (in another thread)
	// NOTES: The result is given in CThreadSync::OnCheaterCheckResponse
	void CheckCheater_Thread(unsigned int playerid, std::string hwid, std::string ip);
}
