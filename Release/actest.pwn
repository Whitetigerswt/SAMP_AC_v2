#include <a_samp>
#include <zcmd>
#include <sampac>

CMD:acstatus(playerid, params[]) 
{
	new str[128];
	format(str, sizeof(str), "IsPlayerUsingSampAC: %d", IsPlayerUsingSampAC(playerid));
	SendClientMessage(playerid, -1, str);


	new hwid[256];
	GetPlayerHardwareID(playerid, hwid, sizeof(hwid));
	format(str, sizeof(str), "HardwareID: %s", hwid);
	SendClientMessage(playerid, -1, str);

	format(str, sizeof(str), "GetPlayerCBug: %d, GetPlayerLiteFoot: %d, GetPlayerSwitchReload: %d", GetPlayerCrouchBug(playerid), GetPlayerLiteFoot(playerid), GetPlayerSwitchReload(playerid));
	SendClientMessage(playerid, -1, str);

	format(str, sizeof(str), "GetPlayerFPSLimit: %d", GetPlayerFPSLimit(playerid));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:setmyfps(playerid, params[])
{
	SetPlayerFPSLimit(playerid, strval(params));

	new str[128];
	format(str, sizeof(str), "Set FPS limit to: %d", strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:litefoot(playerid, params[])
{
	TogglePlayerLiteFoot(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set lite foot to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:cbug(playerid, params[])
{
	TogglePlayerCrouchBug(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set cbug to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:switchreload(playerid, params[])
{
	TogglePlayerSwitchReload(playerid, !!strval(params));

	new str[128];
	format(str, sizeof(str), "Set switch reload to: %d", !!strval(params));
	SendClientMessage(playerid, -1, str);

	return 1;
}

CMD:togglebugs(playerid, params[])
{
	TogglePlayerCrouchBug(playerid, !!strval(params));
	TogglePlayerLiteFoot(playerid, !!strval(params));
	TogglePlayerSwitchReload(playerid, !!strval(params));

	return 1;
}

public OnACOpened(ip[])
{
	printf("PAWN - OnACOpened(%s)", ip);
}

public OnACClosed(ip[])
{
	printf("PAWN - OnACClosed(%s)", ip);
}

public OnFileExecuted(playerid, module[], md5[])
{
	printf("PAWN - OnFileExecuted(%d, %s, %s", playerid, module, md5);
	new str[180];
	format(str, sizeof(str), "OnFileExecuted(%d, %s, %s)", playerid, module, md5);
	SendClientMessage(playerid, -1, str);
}

public OnMD5Calculated(playerid, address, size, md5[])
{
	printf("PAWN - OnMD5Calculated(%d, %d, %d, %s)", playerid, address, size, md5);
	new str[180];
	format(str, sizeof(str), "OnMD5Calculated(%d, 0x%x, %d, %s)", playerid, address, size, md5);
	//SendClientMessage(playerid, -1, str);
}

public OnImgFileModifed(playerid, filename[], md5[])
{
	printf("PAWN - OnImgFileModifed(%d, %s, %s)", playerid, filename, md5);
	new str[180];
	format(str, sizeof(str), "OnImgFileModifed(%d, %s, %s)", playerid, filename, md5);
	SendClientMessage(playerid, -1, str);
}

public OnFileCalculated(playerid, filename[], md5[])
{
	printf("PAWN - OnFileCalculated(%d, %s, %s)", playerid, filename, md5);
	new str[180];
	format(str, sizeof(str), "OnFileCalculated(%d, %s, %s)", playerid, filename, md5);
	SendClientMessage(playerid, -1, str);
}