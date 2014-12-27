
// PURPOSE: Check if the player is using samp AC and return the result
// REQUIRES: NULL
native IsPlayerUsingSampAC(playerid);

// PURPOSE: MD5's a region of memory in the player's address space and then calls OnMD5Calculated
// REQUIRES: The player is using AC.
native MD5_Memory(playerid, address, size);

// PURPOSE: Enable or disable the command to require AC usage on the server for the player
// REQUIRES: The player is connected to the server.
native SetPlayerCanEnableAC(playerid, bool:toggle);

// PURPOSE: Get if the player has access to turn on or off AC.
// REQUIRES: The player is connected to the server.
native CanPlayerEnableAC(playerid);

// PURPOSE: Get the player's unique HardwareID number.
// REQUIRES: The player is using AC.
native GetPlayerHardwareID(playerid, buf[], size = sizeof(buf));

// PURPOSE: Enable or disable switch reload
// REQUIRES: The player is using AC.
native TogglePlayerSwitchReload(playerid, bool:toggle);

// PURPOSE: Set a players frame limit.
// REQUIRES: The player is using AC.
native SetPlayerFPSLimit(playerid, framelimit);

// PURPOSE: Get a players frame limit.
// REQUIRES: The player is using AC.
native GetPlayerFPSLimit(playerid);

// PURPOSE: Toggle crouch bug
// REQUIRES: The player is using AC.
native SetPlayerCrouchBug(playerid, offset);

// PURPOSE: Get the players crouch bug status
// REQUIRES: Nothing
native bool:GetPlayerCrouchBug(playerid);

// PURPOSE: Get the players switch reload status
// REQUIRES: Nothing
native bool:GetPlayerSwitchReload(playerid);

// PURPOSE: Toggle the players ability to use lite foot.
// REQUIRES: The player is using AC.
native TogglePlayerLiteFoot(playerid, bool:toggle);

// PURPOSE: Get the players ability to use lite foot.
// REQUIRES: Nothing
native GetPlayerLiteFoot(playerid);

// PURPOSE: Toggle the players unlimited sprint.
// REQUIRES: The player is using AC.
native TogglePlayerUnlimitedSprint(playerid, bool:toggle);

// PURPOSE: Get the players unlimited sprint status.
// REQUIRES: Nothing
native GetPlayerUnlimitedSprint(playerid);

// PURPOSE: Toggle the macro limits for players (limiting sprint, chat macros, etc)
// REQUIRES: The player is using AC.
native TogglePlayerMacroLimits(playerid, bool:toggle);

// PURPOSE: Get the player's macro limits status
// REQUIRES: Nothing
native GetPlayerMacroLimits(playerid);

// PURPOSE: Toggle the player's ability to sprint on all surfaces.
// REQUIRES: The player is using AC.
native TogglePlayerSprintOnAllSurfaces(playerid, bool:toggle);

// PURPOSE: Get the player's ability to sprint on all surfaces.
// REQUIRES: Nothing.
native GetPlayerSprintOnAllSurfaces(playerid);

// PURPOSE: Toggle the player's ability to to see vehicle blips on their radar.
// REQUIRES: The player is using AC.
native TogglePlayerVehicleBlips(playerid, bool:toggle);

// PURPOSE: Get the player's ability to see vehicle radar blips.
// REQUIRES: Nothing.
native GetPlayerVehicleBlips(playerid);

// PURPOSE: Callback for when a new AC connection has been accepted.
// REQUIRES: The client is using AC.
forward OnACOpened(ip[]);

// PURPOSE: Callback for when a player has disconnected from the AC server.
// REQUIRES: The client is using AC.
// types: 0 = proper disconnection, 1 = timeout
forward OnACClosed(ip[], type);

// PURPOSE: Callback for MD5_Memory
// REQUIRES: MD5_Memorys return value will be sent to this function sometime after MD5_Memory is called.
forward AC_OnMD5Calculated(playerid, address, size, md5[]);

// PURPOSE: Callback for when a new PE file is run on the clients machine.
// REQUIRES: The player is using AC.
forward AC_OnFileExecuted(playerid, module[], md5[], bool:isAllowed);

// PURPOSE: Callback for when the AC has detected there is a file inside gta3.img that is modifed that is also on the list of bad gta3.img files. (Files that can't be modified in gta3.img)
// REQUIRES: The player is using AC.
forward AC_OnImgFileModifed(playerid, filename[], md5[]);

// PURPOSE: Callback for when a file (usually) inside the players GTA directory md5 checksum has been calculated.
// REQUIRES: The player is using AC.
forward AC_OnFileCalculated(playerid, filename[], md5[], bool:isAllowed);

// PURPOSE: Callback for when a player pauses, unpauses, or alt tabs from the game
// REQUIRES: The player is using AC.
// Type = 1 (alt tab)
// Type = 0 (Regular pause)
// Pause = 1 (pause or alt tabbed)
// Pause = 0 (Unpause or came back into the game from an alt tab)
forward AC_OnPlayerPause(playerid, type, bool:pause);

// PURPOSE: Callback for when a player takes a screenshot of their game using F8 only.
// REQUIRES: The player is using AC.
forward AC_OnScreenshotTaken(playerid); 

stock TogglePlayerCrouchBug(playerid, bool:toggle)
{
	SetPlayerCrouchBug(playerid, toggle ? 9999 : 1);
}