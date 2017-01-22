/*
	
	This script keeps the server clean
	as it kicks anyone who is banned -
	on Anti-Cheat. Requires AC plugin.
	

*/

#include <a_samp>
#include <sampac>

forward DelayedKick(playerid);
public DelayedKick(playerid)
{
    Kick(playerid);
    return 1;
}

public AC_OnBanStatusRetrieved(playerid, status)
{
	// Check if AC plugin is loaded or not.
	if(IsACPluginLoaded())
	{
		/*
			Let's also see whether the AC is enabled or not.
			If it's enabled, then this filterscript is of no
			use because AC has a built-in ban checker that -
			does work automatically when the AC is enabled.
		*/
		if(!IsACEnabled())
		{
			// If he's banned
			if(AC_IsPlayerBanned(playerid))
			{
				new str[144], name[MAX_PLAYER_NAME];
				
				// Get the player's name
				GetPlayerName(playerid, name, MAX_PLAYER_NAME);
				
				// Format string with info
				format(str, sizeof str, "{FFFFFF}%s {FF0000}has been kicked for being banned on Anti-Cheat v2.", name);
				
				// Tell everyone
				SendClientMessageToAll(-1, str);
				
				// Actually kick them a second later on a timer.
				SetTimerEx("DelayedKick", 1000, false, "i", playerid);
			}
		}
	}
	return 1;
}