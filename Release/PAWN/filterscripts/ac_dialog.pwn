#include <a_samp>
#include <zcmd>
#include <sampac>

#define DIALOGID 2064

CMD:ac_dialog(playerid, params[])
{

	if(!IsPlayerAdmin(playerid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not an admin on this server.");
	}

	if(!IsACPluginLoaded())
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}The ACv2 plugin is not loaded.");
	}

	new long_dlg_string[470];

	strcat(long_dlg_string, "Check if a player has AC on\nGet a player's Hardware ID\nGet a list of player's processes and files injected into their game\nGet a player's FPS limit\n");
	strcat(long_dlg_string, "Check the integrity of a player's game\nChange a player's ability to switch reload\nChange a player's ability to crouch bug\nChange a player's FPS limit\n");
	strcat(long_dlg_string, "Change a player's ability to sprint infinitely\nChange a player's ability to sprint on all surfaces\nChange a player's ability to see vehicles on the map.");

	ShowPlayerDialog(playerid, DIALOGID, DIALOG_STYLE_LIST, "What would you like to do?", long_dlg_string, "OK", "Cancel");

	return 1;
}