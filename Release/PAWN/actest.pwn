#include <a_samp>
#include <zcmd>
#include <sampac>

public OnFilterScriptInit()
{
	printf("IsUsingACPlugin: %d", IsACPluginLoaded());
}

public OnPlayerConnect(playerid)
{
	printf("IsPlayerUsingSampAC: %d", IsPlayerUsingSampAC(playerid));	
	return 1;
}

public OnPlayerDisconnect(playerid)
{
	return 0;
}

CMD:acstatus(playerid, params[]) 
{
	if(IsACPluginLoaded())
	{
		new targetid = strval(params);

		// Also checks if the player is connected.
		if(IsPlayerUsingSampAC(targetid))
		{
			new str[128];
			new version[50];
			GetPlayerVersion(targetid, version, sizeof(version));
			new name[MAX_PLAYER_NAME];
			GetPlayerName(targetid, name, sizeof(name));
			format(str, sizeof(str), "Target ID: %s (%d), SA-MP version: %s", name, targetid, version);
			SendClientMessage(playerid, -1, str);
			format(str, sizeof(str), "IsPlayerUsingSampAC: %d", IsPlayerUsingSampAC(targetid));
			SendClientMessage(playerid, -1, str);


			new hwid[256];
			GetPlayerHardwareID(targetid, hwid, sizeof(hwid));
			format(str, sizeof(str), "HardwareID: %s", hwid);
			SendClientMessage(playerid, -1, str);

			format(str, sizeof(str), "GetPlayerCBug: %d, GetPlayerLiteFoot: %d, GetPlayerSwitchReload: %d", GetPlayerCrouchBug(targetid), GetPlayerLiteFoot(targetid), GetPlayerSwitchReload(targetid));
			SendClientMessage(playerid, -1, str);

			format(str, sizeof(str), "GetPlayerFPSLimit: %d", GetPlayerFPSLimit(targetid));
			SendClientMessage(playerid, -1, str);
		} 
		else
		{
			new str[128];
			format(str, sizeof(str), "{FF0000}Error: {FFFFFF}This player is not running SAMP AC.");
			SendClientMessage(playerid, -1, str);

			return 1;
		}
	} 
	else
	{
		new str[128];
		format(str, sizeof(str), "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");
		SendClientMessage(playerid, -1, str);

		return 1;
	}

	return 1;
}

CMD:setmyfps(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	SetPlayerFPSLimit(playerid, strval(params));

	new str[128];
	format(str, sizeof(str), "Set FPS limit to: %d", strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:litefoot(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	TogglePlayerLiteFoot(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set lite foot to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:cbug(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	TogglePlayerCrouchBug(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set cbug to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:switchreload(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	TogglePlayerSwitchReload(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set switch reload to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:togglebugs(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	TogglePlayerCrouchBug(playerid, !!strval(params));
	TogglePlayerLiteFoot(playerid, !!strval(params));
	TogglePlayerSwitchReload(playerid, !!strval(params));

	return 1;
}

public AC_OnFileExecuted(playerid, module[], md5[])
{
	printf("PAWN - OnFileExecuted(%d, %s, %s)", playerid, module, md5);
}

public AC_OnMD5Calculated(playerid, address, size, md5[])
{
	printf("PAWN - OnMD5Calculated(%d, %d, %d, %s)", playerid, address, size, md5);
}

public AC_OnImgFileModifed(playerid, filename[], md5[])
{
	printf("PAWN - OnImgFileModifed(%d, %s, %s)", playerid, filename, md5);
}

public AC_OnFileCalculated(playerid, filename[], md5[])
{
	printf("PAWN - OnFileCalculated(%d, %s, %s)", playerid, filename, md5);
}