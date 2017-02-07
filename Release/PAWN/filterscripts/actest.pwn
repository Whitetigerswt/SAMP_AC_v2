#include <a_samp>
#include <zcmd>
#include <sampac>

public OnFilterScriptInit()
{
	printf("IsUsingACPlugin: %d", IsACPluginLoaded());
}

public OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, -1, "Use some of the AC test commands: /cbug /switchreload /vehblips /infsprint /sprintsurface /md5");
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

			format(str, sizeof(str), "GetPlayerCBug: %d, GetPlayerSwitchReload: %d", GetPlayerCrouchBug(targetid), GetPlayerSwitchReload(targetid));
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

CMD:sprintlimit(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");
	
	SetPlayerSprintLimit(playerid, strval(params));

	new str[128];
	format(str, sizeof(str), "Set sprint speed limit to: %d", strval(params));
	SendClientMessage(playerid, -1, str);
	
	if(!GetPlayerMacroLimits(playerid))
	    SendClientMessage(playerid, -1, "WARNING: You won't be able to see the effect since macro limitation is currently disabled.");

	return 1;
}

CMD:vehblips(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	if(GetPlayerVehicleBlips(playerid) == 1)
		TogglePlayerVehicleBlips(playerid, false);
	else
	    TogglePlayerVehicleBlips(playerid, true);

	new str[128];
	format(str, sizeof(str), "Set vehicle blips to: %d", GetPlayerVehicleBlips(playerid));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:sprintsurface(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	if(GetPlayerSprintOnAllSurfaces(playerid) == 1)
		TogglePlayerSprintOnAllSurfaces(playerid, 0);
	else
		TogglePlayerSprintOnAllSurfaces(playerid, 1);
	
	new str[128];
	format(str, sizeof(str), "Set sprint on all surfaces to: %d", GetPlayerSprintOnAllSurfaces(playerid));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:infsprint(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	if(GetPlayerUnlimitedSprint(playerid) == 1)
		TogglePlayerUnlimitedSprint(playerid, 0);
	else
		TogglePlayerUnlimitedSprint(playerid, 1);

	new str[128];
	format(str, sizeof(str), "Set sprint on all surfaces to: %d", GetPlayerUnlimitedSprint(playerid));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:cbug(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	if(GetPlayerCrouchBug(playerid) >= 1)
		TogglePlayerCrouchBug(playerid, false);
	else
		TogglePlayerCrouchBug(playerid, true);

	new str[128];
	format(str, sizeof(str), "Set cbug to: %d", GetPlayerCrouchBug(playerid));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:switchreload(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");

	if(GetPlayerSwitchReload(playerid) == 1)
		TogglePlayerSwitchReload(playerid, 0);
	else
		TogglePlayerSwitchReload(playerid, 1);

	new str[128];
	format(str, sizeof(str), "Set switch reload to: %d", GetPlayerSwitchReload(playerid));
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
	TogglePlayerSwitchReload(playerid, !!strval(params));

	return 1;
}

CMD:md5(playerid, params[])
{
	if(!IsACPluginLoaded())
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}AC v2 plugin is not loaded.");

	if(!IsPlayerUsingSampAC(playerid))
		return SendClientMessage(playerid, -1, "{FF0000}Error: {FFFFFF}You're not running SA-MP AC.");
		
	if(strlen(params) == 0)
	{
	    SendClientMessage(playerid, -1, "[Usage] /md5 [memory-address]");
	    return 1;
	}
	
	SendClientMessage(playerid, -1, "All md5's are 256 bytes of memory.");
		
	MD5_Memory(playerid, strval(params), 256);
	
	return 1;
}

public AC_OnFileExecuted(playerid, module[], md5[])
{
	printf("PAWN - OnFileExecuted(%d, %s, %s)", playerid, module, md5);
}

public AC_OnMD5Calculated(playerid, address, size, md5[])
{
	if(address != 0xC8C418)
	{
	    new s[128];
	    format(s, sizeof(s), "OnMD5Calculated(%d, %d, %d, %s)", playerid, address, size, md5);
	    SendClientMessage(playerid, -1, s);
	}
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
