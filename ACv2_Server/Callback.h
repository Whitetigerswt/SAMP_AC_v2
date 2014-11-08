#pragma once

#include <list>
#include "SDK/samp-sdk/amx/amx.h"
#include <string>

struct PAWNArray
{
	cell* address;
	size_t length;
};

namespace Callback
{
	// PURPOSE: Execute a PAWN callback.
	// REQUIRES: A public function name, and whatever arguments.
	// PROMISE: That public function will be called in all PAWN scripts.
	// NOTES: Arguments pushed in reverse order.
	cell Execute(const char* szFunction, const char* szFormat, ...);

	// PURPOSE: Get a list of loaded AMX scripts (all gamemodes and filterscripts)
	// REQUIRES: an std::list to hold it all.
	std::list<AMX*>& GetAMXList();

	// PURPOSE: Callback for when a player closes their AC by IP.
	// REQUIRES: Nothing, this function should not be called manually.
	// REQUIRES: The disconnection type.
	void OnACClosed(std::string ip, int type);

	// PURPOSE: Callback for when a player closes their AC by playerid.
	// REQUIRES: Nothing, this function should not be called manually.
	// REQUIRES: The disconnection type.
	void OnACClosed(unsigned int playerid, int type);

	// PURPOSE: Callback to kick a player on a delay (So they will see the SendClientMessage's before they get kicked)
	// REQUIRES: Nothing.
	void KickPlayer(int timerid, void* params);

	static bool ACToggle;
}