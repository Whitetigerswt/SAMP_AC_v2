
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

forward OnMD5Calculated(playerid, address, size);
forward OnFileExecuted(playerid, module[], md5[]);