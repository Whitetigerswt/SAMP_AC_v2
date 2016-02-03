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

new bool:PlayerACCheck[MAX_PLAYERS];

new DB:accheck_db;
#define query(%0) (db_free_result(db_query(accheck_db, %0)))

ACCheck_InitDatabase()
{
	accheck_db = db_open("accheck_db.db");
	if(accheck_db == DB:0)
	{
		printf("Error: Failed to load accheck database.");
		return 0;
	}
	query("CREATE TABLE IF NOT EXISTS [accheck_list] ([Name] VARCHAR(30), [IP] VARCHAR(30));");
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
	if(IsACPluginLoaded())
	{
		if(ACCheck_InitDatabase())
		{
			for(new i = 0; i != MAX_PLAYERS; ++ i)
			{
				if(!IsPlayerConnected(i))
					continue;

				OnPlayerConnect(i);
			}
		}
		else
		{
			SendRconCommand("unloadfs accheck");
		}
	}
	else
	{
		print("Error: Can't load accheck script since sampac plugin is not loaded.");
		SendClientMessageToAll(-1, "{FF0000}Error: Can't load accheck script since sampac plugin is not loaded. Filterscript unloading!");
		SendRconCommand("unloadfs accheck");
	}
	return 1;
}

public OnFilterScriptExit()
{
	db_close(accheck_db);
	return 1;
}

public OnPlayerConnect(playerid)
{
	new IP[16];
	GetPlayerIp(playerid, IP, sizeof(IP));

	new str[144];
	format(str, sizeof(str), "SELECT `Name` FROM `accheck_list` WHERE `Name`='%s' OR `IP`='%s'", ReturnPlayerName(playerid), IP);
	new DBResult:query_result = db_query(accheck_db, str);

	if(db_num_rows(query_result))
	{
	    if(!IsACEnabled())
	    {
			if(!IsPlayerUsingSampAC(playerid))
			{
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
   			    PlayerACCheck[playerid] = true;
   			}
		}
		else
		{
		    PlayerACCheck[playerid] = true;
		}
	}
	else
	{
	    PlayerACCheck[playerid] = false;
	}
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

	SendClientMessage(targetid, -1, "{FF0000}Warning: {FFFFFF}Download AC from samp-ac.com if you don't have it and make sure it's running next time you connect!");

	new IP[16];
	GetPlayerIp(targetid, IP, sizeof(IP));

	format(str, sizeof str, "INSERT INTO `accheck_list` (`Name`, `IP`) VALUES ('%s', '%s')", ReturnPlayerName(targetid), IP);
	query(str);

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
	format(str, sizeof(str), "SELECT * FROM `accheck_list` WHERE `Name`='%s'", params);
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

	format(str, sizeof str, "DELETE FROM `accheck_list` WHERE `Name`='%s'", params);
	query(str);
	return 1;
}

COMMAND:acchecklist(playerid, params[])
{
    if(!IsACPluginLoaded())
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC plugin is not loaded.");
 	}
	new DBResult:query_result = db_query(accheck_db, "SELECT `Name`, `IP` FROM `accheck_list`");
	if(db_num_rows(query_result))
	{
	    new name[MAX_PLAYER_NAME], IP[16];
	    new str[256] = "Name\tIP";
		do
		{
			db_get_field_assoc(query_result, "Name", name, sizeof name);
			db_get_field_assoc(query_result, "IP", IP, sizeof IP);
			format(str, sizeof str, "%s\n%s\t%s", str, name, IP);
		}
		while(db_next_row(query_result));
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "AC-check list", str, "Close", "");
	}
	db_free_result(query_result);
	return 1;
}

public AC_OnFileExecuted(playerid, module[], md5[], bool:isCheat)
{
	if(isCheat && !IsACEnabled() && PlayerACCheck[playerid] == true)
	{
		new str[144];
		format(str, sizeof str, "{FF0000}[AC] {FFFFFF}File execution report from {FF0000}%s {FFFFFF}module: %s / blacklisted: Yes", ReturnPlayerName(playerid), module);
		SendClientMessageToAll(-1, str);
	}
	return 1;
}

public AC_OnImgFileModifed(playerid, filename[], md5[])
{
	if(!IsACEnabled() && PlayerACCheck[playerid] == true)
	{
		new str[144];
		format(str, sizeof str, "{FF0000}[AC] %s {FFFFFF}has got a modified IMG file {FF0000}(%s)", ReturnPlayerName(playerid), filename);
		SendClientMessageToAll(-1, str);
	}
	return 1;
}

public AC_OnFileCalculated(playerid, filename[], md5[], bool:isCheat)
{
    if(isCheat && !IsACEnabled() && PlayerACCheck[playerid] == true)
	{
		new str[144];
		format(str, sizeof str, "{FF0000}[AC] {FFFFFF}File calculation report from {FF0000}%s {FFFFFF}file: %s / blacklisted: Yes", ReturnPlayerName(playerid), filename);
		SendClientMessageToAll(-1, str);
	}
	return 1;
}
