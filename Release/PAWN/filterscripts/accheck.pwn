/*
	This script introduces a new command: /accheck which enables you to
	force a specific player to have anti-cheat running, even if it is
	not globally enabled in the server (through /actoggle). The target
	player must have AC running while they're connected to your server
 	or they are kicked. They're also reported for any bad behavior even
	when ac main checks are disabled.
 	
 	Commands:
 	/accheck
 	/noaccheck
 	/acchecklist
*/

#include <a_samp>
#include <sampac>
#include <zcmd>

// A variable to indicate whether a player has ac-check enabled on them or not
new bool:PlayerACCheck[MAX_PLAYERS];

// AC-check database handle
new DB:accheck_db;

// Some macro for quick SQL queries which leave no important result (e.g UPDATE, INSERT...)
#define query(%0) (db_free_result(db_query(accheck_db, %0)))

ACCheck_InitDatabase()
{
	// Initialize the accheck database and open it
	accheck_db = db_open("accheck_db.db");
	if(accheck_db == DB:0) // Check if there's an error or something
	{
		// Output error info
		printf("Error: Failed to load accheck database.");
		return 0; // and stop the process
	}
	// Create a table to hold ac-checked players's name and IP, only if it does not exist.
	query("CREATE TABLE IF NOT EXISTS [accheck_list] ([Name] VARCHAR(30), [IP] VARCHAR(30));");
	// Cleaning
	query("VACUUM");
	return 1;
}

forward DelayedKick(playerid);
public DelayedKick(playerid)
{
    Kick(playerid);
    return 1;
}

ReturnPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof name);
	return name;
}

public OnFilterScriptInit()
{
	// See if the AC plugin is loaded
	if(IsACPluginLoaded())
	{
	    // Check if database is successfully initialized
		if(ACCheck_InitDatabase())
		{
		    // Loop through players
			for(new i = 0; i != MAX_PLAYERS; ++ i)
			{
				// Check whether they're connected or not
				if(!IsPlayerConnected(i))
					continue; // Skip if they are not connected

				// Initialize them
				OnPlayerConnect(i);
			}
		}
		else
		{
		    // Database failure
			SendRconCommand("unloadfs accheck");
		}
	}
	else
	{
	    // The AC plugin is not loaded
		print("Error: Can't load accheck script since sampac plugin is not loaded.");
		SendClientMessageToAll(-1, "{FF0000}Error: Can't load accheck script since sampac plugin is not loaded. Filterscript unloading!");
		SendRconCommand("unloadfs accheck");
	}
	return 1;
}

public OnFilterScriptExit()
{
	// Close sql-lite connection
	db_close(accheck_db);
	return 1;
}

public OnPlayerConnect(playerid)
{
    // IsPlayerUsingSampAC might return incorrect values when called at OnPlayerConnect in filterscripts. 
    // So, we'll use SetTimerEx for a dirty fix.
    // See "Running code just after a function finishes" paragraph: http://forum.sa-mp.com/showpost.php?p=1037822
    SetTimerEx("FIX_OnPlayerConnect", 0, false, "d", playerid);
    return 1;
}

forward FIX_OnPlayerConnect(playerid);
public FIX_OnPlayerConnect(playerid)
{
	// Get the player's IP
	new IP[16];
	GetPlayerIp(playerid, IP, sizeof(IP));

	// See if this player's name or IP matches any of what is in our database of AC-checked players
	new str[144];
	format(str, sizeof(str), "SELECT `Name` FROM `accheck_list` WHERE `Name`='%q' OR `IP`='%q'", ReturnPlayerName(playerid), IP);
	new DBResult:query_result = db_query(accheck_db, str);

	// See if there are any matches
	if(db_num_rows(query_result))
	{
	    // Check if anti cheat is not enabled
	    if(!IsACEnabled())
	    {
	        // Check if the player is not using AC
			if(!IsPlayerUsingSampAC(playerid))
			{
			    // Punish them!
				format(str, sizeof str, "{FF0000}%s {FFFFFF}has been kicked for not running Anti-cheat while having AC-check enabled on them.", ReturnPlayerName(playerid));
				SendClientMessageToAll(-1, str);

				new name[MAX_PLAYER_NAME];
				db_get_field_assoc(query_result, "Name", name, sizeof name);
				format(str, sizeof str, "{FF0000}To disable AC-check on this player, type {FFFFFF}/noaccheck %s.", name);
				SendClientMessageToAll(-1, str);

				SetTimerEx("DelayedKick", 1000, false, "i", playerid);
   			}
   			else
   			{
   			    // They are using anti-cheat. We will warn our script that they have ac-checked enabled on them just for now.
   			    PlayerACCheck[playerid] = true;
   			}
		}
		else
		{
		    // Anti-cheat is enabled. We still need to warn our script that they have ac-checked enabled on them.
		    PlayerACCheck[playerid] = true;
		}
	}
	else
	{
		// We could not find them in our database, so they're clean and were not ac-checked.
	    PlayerACCheck[playerid] = false;
	}
	// Always free result
	db_free_result(query_result);
	return 1;
}

COMMAND:accheck(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not admin. Log into rcon admin maybe!");
	}
	if(IsACEnabled())
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC main checks are already enabled on all players.");
	}
	if(isnull(params))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Syntax error: {FFFFFF}Correct syntax is /accheck [playerid].");
	}
	new targetid = strval(params);
	if(!IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}Invalid player id.");
	}
	if(PlayerACCheck[targetid] == true)
	{
	    return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}This player has already got ac-check enabled on them.");
	}
	new str[144];
	format(str, sizeof str, "{FFFFFF}%s {FF0000}has enabled AC-check on {FFFFFF}%s {FF0000}(forced them to re-connect with AC running)!", ReturnPlayerName(playerid), ReturnPlayerName(targetid));
	SendClientMessageToAll(-1, str);

	// Tell the player that they must have AC running next time they connect to the server
	SendClientMessage(targetid, -1, "{FF0000}Warning: {FFFFFF}Download AC from samp-ac.com if you don't have it and make sure it's running next time you connect!");

	new IP[16];
	GetPlayerIp(targetid, IP, sizeof(IP));

	// Insert this player's name and IP into our table of ac-checked players
	format(str, sizeof str, "INSERT INTO `accheck_list` (`Name`, `IP`) VALUES ('%q', '%q')", ReturnPlayerName(targetid), IP);
	query(str);

	// Kick the player
	SetTimerEx("DelayedKick", 1000, false, "i", targetid);
	return 1;
}

COMMAND:noaccheck(playerid, params[])
{
	if(!IsPlayerAdmin(playerid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not admin. Log into rcon admin maybe!");
	}
	if(isnull(params))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Syntax error: {FFFFFF}Correct syntax is /noaccheck [player name].");
	}
	new str[144];
	format(str, sizeof(str), "SELECT * FROM `accheck_list` WHERE `Name`='%q'", params);
	new DBResult:query_result = db_query(accheck_db, str);

	if(db_num_rows(query_result))
	{
		SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}That player does not have AC-check enabled on them.");
		db_free_result(query_result);
		return 1;
	}
	db_free_result(query_result);

	format(str, sizeof str, "{FFFFFF}%s {FF0000}has disabled AC-check on {FFFFFF}%s!", ReturnPlayerName(playerid), params);
	SendClientMessageToAll(-1, str);

	// Remove this player from acchecked players table
	format(str, sizeof str, "DELETE FROM `accheck_list` WHERE `Name`='%q'", params);
	query(str);
	return 1;
}

COMMAND:acchecklist(playerid, params[])
{
    if(!IsACPluginLoaded())
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC plugin is not loaded.");
 	}
 	// Get all results from our accheck list table
	new DBResult:query_result = db_query(accheck_db, "SELECT `Name`, `IP` FROM `accheck_list`");
	if(db_num_rows(query_result))
	{
	    new name[MAX_PLAYER_NAME], IP[16];
	    new str[256] = "Name\tIP";
		do
		{
		    // Format name and ip into their appropriate variables
			db_get_field_assoc(query_result, "Name", name, sizeof name);
			db_get_field_assoc(query_result, "IP", IP, sizeof IP);

			// Format main string that will be showed in dialog
			format(str, sizeof str, "%s\n%s\t%s", str, name, IP);
		}
		while(db_next_row(query_result)); // stay as long as next rows are valid

		// Show dialog
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "AC-check list", str, "Close", "");
	}
	// Always free result
	db_free_result(query_result);
	return 1;
}

public AC_OnImgFileModifed(playerid, filename[], md5[])
{
    // If it's a modified file, ac is not enabled and this player has got ac-checked enabled on them
	if(!IsACEnabled() && PlayerACCheck[playerid] == true)
	{
	    // Report it to other players
		new str[144];
		format(str, sizeof str, "{FF0000}[AC] %s {FFFFFF}has got a modified IMG file {FF0000}(%s)", ReturnPlayerName(playerid), filename);
		SendClientMessageToAll(-1, str);
	}
	return 1;
}

public AC_OnFileCalculated(playerid, filename[], md5[], bool:isCheat)
{
    // If it's a modified file, ac is not enabled and this player has got ac-checked enabled on them
    if(isCheat && !IsACEnabled() && PlayerACCheck[playerid] == true)
	{
	    // Report it to other players
		new str[144];
		format(str, sizeof str, "{FF0000}[AC] {FFFFFF}File calculation report from {FF0000}%s {FFFFFF}file: %s / blacklisted: Yes", ReturnPlayerName(playerid), filename);
		SendClientMessageToAll(-1, str);
	}
	return 1;
}
