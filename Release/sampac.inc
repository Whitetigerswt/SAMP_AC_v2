
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
native ToggleSwitchReload(playerid, bool:toggle);

// PURPOSE: Set a players frame limit.
// REQUIRES: The player is using AC.
native SetPlayerFPSLimit(playerid, framelimit);

// PURPOSE: Get a players frame limit.
// REQUIRES: The player is using AC.
native GetPlayerFPSLimit(playerid);

// PURPOSE: Toggle crouch bug
// REQUIRES: The player is using AC.
native ToggleCrouchBug(playerid, bool:toggle);

// PURPOSE: Callback for when a new AC connection has been accepted.
// REQUIRES: The client is using AC.
forward OnACOpened(ip[]);

// PURPOSE: Callback for when a player has disconnected from the AC server.
// REQUIRES: The client is using AC.
forward OnACClosed(ip[]);

// PURPOSE: Callback for MD5_Memory
// REQUIRES: MD5_Memorys return value will be sent to this function sometime after MD5_Memory is called.
forward OnMD5Calculated(playerid, address, size, md5[]);

// PURPOSE: Callback for when a new PE file is run on the clients machine.
// REQUIRES: The player is using AC.
forward OnFileExecuted(playerid, module[], md5[]);

// PURPOSE: Callback for when the AC has detected there is a file inside gta3.img that is modifed that is also on the list of bad gta3.img files. (Files that can't be modified in gta3.img)
// REQUIRES: The player is using AC.
forward OnImgFileModifed(playerid, filename[], md5[]);

// PURPOSE: Callback for when a file (usually) inside the players GTA directory md5 checksum has been calculated.
// REQUIRES: The player is using AC.
forward OnFileCalculated(playerid, filename[], md5[]);