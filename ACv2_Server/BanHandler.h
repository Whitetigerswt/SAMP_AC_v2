#pragma once

#include <string>

#define AC_BAN_HANDLER_ADD				"https://apples.sampcup.com/addcheater.php"
#define AC_BAN_HANDLER_CHECK			"https://apples.sampcup.com/checkcheater.php"

namespace BanHandler
{
	// PURPOSE: Add a player to the global banlist.
	// NOTES: "reason" must contain the name of cheat used and any extra info (e.g. reason = "modified FILE_NAME")
	void AddCheater(unsigned int playerid, char * reason);

	// PURPOSE: Check whether a player is in the global banlist or not.
	// RETURNS: a boolean - true if this player exists in the global ban list, else false.
	bool CheckCheater(unsigned int playerid);
}