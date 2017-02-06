#include <a_samp>
#include <sampac>
#include <zcmd>
#include <sscanf2>

// Make a DB handle for the sqlite database to hold hardware ID bans
new DB:bans_reference;

// query macro for when we don't store the result.
#define query(%0) (db_free_result(db_query(bans_reference, %0)))

public OnFilterScriptInit()
{
	// First, check if the AC plugin is even loaded before we bother dong other stuff
	if(!IsACPluginLoaded())
	{
		return printf("Error: The ACv2 plugin is not loaded, Hardware ID ban filterscript will not work.");
	}

	// Get a reference to the hardware ID bans database
	bans_reference = db_open("ac_bans.db");
	if(bans_reference == DB:0)
	{
		return printf("Error: Failed to load hardware ID bans.");
	}

	// Create the table structure
	query("CREATE TABLE IF NOT EXISTS [bans] ([hwid] VARCHAR(256), [name] VARCHAR(30), [Reason] VARCHAR(256), [Admin] VARCHAR(30), [IP] VARCHAR(30));");

	// Clean the database
	query("VACUUM");

	printf("** Hardware ID Ban filterscript loaded successfully.");

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
	// Create a variable to handle a ban if the AC plugin is not loaded (and hardware ID can't be retrived...)
	new bool:ip_only = !IsACPluginLoaded() || !IsPlayerUsingSampAC(playerid);

	// Variable to hold hardware ID.
	new hwid[HARDWAREID_LEN];

	// set an invalid hwid - can be anything as long as it's not a valid hwid.
	hwid = "lolz";

	if(!ip_only)
	{
		// Only change the hardware id from invalid if the AC is loaded properly.
		GetPlayerHardwareID(playerid, hwid, sizeof(hwid));
	}

	new escaped_hwid[160];
	new IP[MAX_PLAYER_NAME];

	escaped_hwid = DB_Escape(hwid);
	GetPlayerIp(playerid, IP, sizeof(IP));

	new str[256];
	format(str, sizeof(str), "SELECT `Reason`, `Admin` FROM `bans` WHERE `hwid` = '%s' OR `IP` = '%s' LIMIT 1", escaped_hwid, IP);
	new DBResult:query_result = db_query(bans_reference, str);

	// If we got results...
	if(db_num_rows(query_result))
	{
		// Ban evading
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, sizeof(name));
		format(str, sizeof(str), "{FF0000}'%s'{FFFFFF} has been caught ban evading.", name);
		SendClientMessageToAll(-1, str);

		Ban(playerid);
	}

	db_free_result(query_result);

	return 1;
}

CMD:hardwareban(playerid, params[])
{
	if(!IsACPluginLoaded())
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}The ACv2 plugin is not loaded, Cannot ban players by hardware ID.");
	}

	if(!IsPlayerAdmin(playerid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not an admin on this server.");
	}

	new targetid, reason[90];
	if(sscanf(params, "?<MATCH_NAME_PARTIAL=1>uS(No Reason Given)[90]", targetid, reason))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Usage: {FFFFFF}/hardwareban [playerid] [reason]");
	}

	if(!IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}This player is not connected to the server.");
	}

	if(!IsPlayerUsingSampAC(targetid))
	{
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}This player is not running the ACv2 client.");
	}

	new hwid[HARDWAREID_LEN];
	GetPlayerHardwareID(targetid, hwid, sizeof(hwid));

	new escaped_hwid[160];
	escaped_hwid = DB_Escape(hwid);

	new escaped_reason[160]; 
	escaped_reason = DB_Escape(reason);

	new adminName[MAX_PLAYER_NAME], targetName[MAX_PLAYER_NAME], IP[MAX_PLAYER_NAME];

	GetPlayerName(playerid, adminName, sizeof(adminName));
	GetPlayerName(targetid, targetName, sizeof(targetName));
	GetPlayerIp(targetid, IP, sizeof(IP));

	new str[256];
	format(str, sizeof(str), "INSERT INTO `bans` (`hwid`, `name`, `Reason`, `Admin`, `IP`) VALUES ('%s', '%s', '%s', '%s', '%s');", escaped_hwid, targetName, escaped_reason, adminName, IP);
	query(str);

	format(str, sizeof(str), "'{FF0000}%s{FFFFFF}' has banned '{FF0000}%s{FFFFFF}' from the server for \"{FF0000}%s{FFFFFF}\"", adminName, targetName, reason);
	SendClientMessage(playerid, -1, str);

	BanEx(targetid, reason);

	return 1;
}

stock DB_Escape(text[])
{
	new
		ret[160],
		ch,
		i,
		j;
	while ((ch = text[i++]) && j < sizeof (ret))
	{
		if (ch == '\'')
		{
			if (j < sizeof (ret) - 2)
			{
				ret[j++] = '\'';
				ret[j++] = '\'';
			}
		}
		else if (j < sizeof (ret))
		{
			ret[j++] = ch;
		}
		else
		{
			j++;
		}
	}
	ret[sizeof (ret) - 1] = '\0';
	return ret;
}