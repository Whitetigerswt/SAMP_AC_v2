#if !defined GetGravity
	native Float:GetGravity();
#endif

native ac_gateway_param0(func[]) = GetGravity;
native ac_gateway_param1(func[], param1) = GetGravity;
native ac_gateway_param2(func[], param1, param2) = GetGravity;
native ac_gateway_param3(func[], param1, param2, param3) = GetGravity;
native ac_gateway_param4(func[], param1, param2, param3, param4) = GetGravity;

// For GetPlayerHardwareID
native ac_gateway_param3_str(func[], param1, param2[], param3) = GetGravity;


// PURPOSE: Check if the player is using samp AC and return the result
// REQUIRES: NULL
// NOTES: This function might return incorrect values when called at OnPlayerConnect in filterscripts. 
// NOTES: For a workaround, see OnPlayerConnect in sample scripts.
//
// Params:
// playerid - Player to check
//
// Returns: int - 1 if player is using AC, else false.
#define IsPlayerUsingSampAC(%0) ac_gateway_param1("IsPlayerUsingSampAC", %0)

// PURPOSE: MD5's a region of memory in the player's address space and then calls OnMD5Calculated
// REQUIRES: The player is using AC.
//
// Params:
// playerid - Player to check
// address  - Memory address of gta_sa.exe (or loaded modules) to check
// size     - size of memory to check
//
// Returns: int - 1 if the action was sent to the client
#define MD5_Memory(%0) ac_gateway_param3("MD5_Memory", %0)

// PURPOSE: Enable or disable the command to require AC usage on the server for the player
// REQUIRES: The player is connected to the server.
//
// Params:
// playerid 		- Player to check
// bool:toggle 		- set so they can(not) enable/disable ac with /actoggle
//
// Returns: irrelevant
#define SetPlayerCanEnableAC(%0) ac_gateway_param2("SetPlayerCanEnableAC", %0)

// PURPOSE: Get if the player has access to turn on or off AC.
// REQUIRES: The player is connected to the server.
//
// Params:
// playerid         - Player to check
//
// Returns: int 	- 1 if the player can enable AC or not via /actoggle
#define CanPlayerEnableAC(%0) ac_gateway_param1("CanPlayerEnableAC", %0)

// PURPOSE: Get the player's unique HardwareID number.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to check
// buf[]            - String to store the result in
// size (optional)  - size of buf string
//
// Returns: irrelevant
#define GetPlayerHardwareID(%0) ac_gateway_param3_str("GetPlayerHardwareID", %0)

// PURPOSE: Enable or disable switch reload
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to check
// bool:toggle      - enable or disable switch reload for the player.
//
// Returns: int - 1 if the action was sent to the client.
#define TogglePlayerSwitchReload(%0) ac_gateway_param2("TogglePlayerSwitchReload", %0)

// PURPOSE: Set a players frame limit.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to set new FPS limit
// framelimit       - the new frame limit for the player (values below 1 are ignored, 0 sets FPS limiter off)
//
// Returns: int - 1 if action was sent to the client.
#define SetPlayerFPSLimit(%0) ac_gateway_param2("SetPlayerFPSLimit", %0)

// PURPOSE: Get a players frame limit.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to check
//
// Returns: int - the player's FPS limit.
#define GetPlayerFPSLimit(%0) ac_gateway_param1("GetPlayerFPSLimit", %0)

// PURPOSE: Toggle crouch bug
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - playerid to set
// offset           - integer between 1-9999, the lower the number is, the slower you can crouch after shooting.
//
// Returns: int - 1 if the action was sent to the client.
#define SetPlayerCrouchBug(%0) ac_gateway_param2("SetPlayerCrouchBug", %0)

// PURPOSE: Get the players crouch bug status
// REQUIRES: Nothing
//
// Params:
// playerid         - Player to check
//
// Returns: int - value last set by SetPlayerCrouchBug on this player. (default 9999)
#define GetPlayerCrouchBug(%0) ac_gateway_param1("GetPlayerCrouchBug", %0)

// PURPOSE: Get the players switch reload status
// REQUIRES: Nothing
//
// Params:
// playerid         - Player to check
//
// Returns: bool - True if switch reload is enabled (default)
#define GetPlayerSwitchReload(%0) ac_gateway_param1("GetPlayerSwitchReload", %0)

// PURPOSE: Toggle the players unlimited sprint.
// REQUIRES: The player is using AC.
// NOTES: Calling this function to true, and then back to false does NOT replenish sprint. Only SpawnPlayer will replenish sprint.
//
// Params:
// playerid         - Player to change
// bool:toggle      - True to enable unlimited sprint, false to disable
//
// Returns: int - 1 if action was sent to the client.
#define TogglePlayerUnlimitedSprint(%0) ac_gateway_param2("TogglePlayerUnlimitedSprint", %0)

// PURPOSE: Get the players unlimited sprint status.
// REQUIRES: Nothing
//
// Params:
// playerid         - Player to check
//
// Returns: bool - True if unlimited sprint is enabled, else false.
#define GetPlayerUnlimitedSprint(%0) ac_gateway_param1("GetPlayerUnlimitedSprint", %0)

// PURPOSE: Toggle the macro limits for players (limiting sprint, chat macros, etc)
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to change
// bool:toggle      - true to enable macro limitations, else false
//
// Returns: int - 1 if action was sent to the client.
#define TogglePlayerMacroLimits(%0) ac_gateway_param2("TogglePlayerMacroLimits", %0)

// PURPOSE: Get the player's macro limits status
// REQUIRES: Nothing
//
// Params:
// playerid         - Player to check
//
// Returns: bool - True if macro limits are enabled, else false.
#define GetPlayerMacroLimits(%0) ac_gateway_param1("GetPlayerMacroLimits", %0)

// PURPOSE: Set a player's sprint speed limit.
// REQUIRES: The player is using AC.
// NOTES: This will have effect only if macro limits option is enabled (See TogglePlayerMacroLimits and GetPlayerMacroLimits)
// NOTES: (speed = 85) is a good sprint speed limit.
// NOTES: If (speed = 0), default GTA speed limit will be used.
//
// Params:
// playerid	- Player to set new sprint speed limit
// speed	- the new sprint speed limit
//
// Returns: int - 1 if action was sent to the client.
#define SetPlayerSprintLimit(%0) ac_gateway_param2("SetPlayerSprintLimit", %0)

// PURPOSE: Get a player's sprint speed limit.
// REQUIRES: The player is using AC.
//
// Params:
// playerid	- Player to get sprint speed limit
//
// Returns: int - Player's sprint speed limit.
#define GetPlayerSprintLimit(%0) ac_gateway_param1("GetPlayerSprintLimit", %0)

// PURPOSE: Toggle the player's ability to sprint on all surfaces.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to allow/disallow sprinting on all surfaces.
// bool:toggle      - true to enable sprint on all surfaces, false to disallow sprinting on SOME surfaces (equivalent to how it is without AC).
//
// Returns: int - 1 if action was sent to client
#define TogglePlayerSprintOnAllSurfaces(%0) ac_gateway_param2("TogglePlayerSprintOnAllSurfaces", %0)

// PURPOSE: Get the player's ability to sprint on all surfaces.
// REQUIRES: Nothing.
//
// Params:
// playerid         - Player to check
//
// Returns: bool - true if sprinting on all surfaces is enabled, else false.
#define GetPlayerSprintOnAllSurfaces(%0) ac_gateway_param1("GetPlayerSprintOnAllSurfaces", %0)

// PURPOSE: Toggle the player's ability to to see vehicle blips on their radar.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to change
// bool:toggle      - True to enable vehicle blips, false to disable.
//
// Returns: int - 1 if action was sent to the client.
#define TogglePlayerVehicleBlips(%0) ac_gateway_param2("TogglePlayerVehicleBlips", %0)

// PURPOSE: Get the player's ability to see vehicle radar blips.
// REQUIRES: Nothing.
//
// Params:
// playerid         - Player to check
//
// Returns: bool - True if vehicle blips are enabled.
#define GetPlayerVehicleBlips(%0) ac_gateway_param1("GetPlayerVehicleBlips", %0)

// PURPOSE: Check if the player is banned on Anti-Cheat or not.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to check
//
// Returns: bool - True if player is banned.
#define AC_IsPlayerBanned(%0) ac_gateway_param1("AC_IsPlayerBanned", %0)

// PURPOSE: Get player's ban status on Anti-Cheat.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player to check
//
// Returns: (-1) = unknown ban status, 0 = not banned, 1 = banned
#define AC_GetPlayerBanStatus(%0) ac_gateway_param1("AC_GetPlayerBanStatus", %0)

// PURPOSE: Check if the AC is enabled via /actoggle
// REQUIRES: Nothing
//
// Params: Nothing
//
// returns: bool - true if ac is on or not.
#define IsACEnabled() ac_gateway_param0("IsACEnabled")

// PURPOSE: Get if the AC plugin is loaded or not
// REQUIRES: Nothing.
//
// Returns: bool - true if the plugin is loaded, else false
#define IsACPluginLoaded() ((_:GetGravity()) != (IsPlayerUsingSampAC(9999)))

// PURPOSE: Callback for MD5_Memory
// REQUIRES: MD5_Memorys return value will be sent to this function sometime after MD5_Memory is called.
//
// Params:
// playerid     - The player whom was checked
// address      - The address that was calculated
// size         - The number of bytes calculated
// md5[]        - The md5 checksum of the players game starting at address with size length
//
// Returns: irrelevant
forward AC_OnMD5Calculated(playerid, address, size, md5[]);

// PURPOSE: Callback for when a new executed file is run on the clients machine.
// REQUIRES: The player is using AC.
//
// Params:
// playerid     	- Player whom a new file was executed
// module[]     	- The name of the file (should usually be ignored as people can name it whatever they want)
// md5[]        	- The file's MD5
// bool:isCheat   - true if this file is on the AC blacklist.
//
// Returns: irrelevant
forward AC_OnFileExecuted(playerid, module[], md5[]);

// PURPOSE: Callback for when a client is trying to do packet tampering.
// REQUIRES: The player is using AC.
//
// Params:
// playerid     	- Player whom a new file was executed
//
// Returns: irrelevant
forward AC_OnPacketTampering(playerid);

// PURPOSE: Callback for when the AC has detected there is a file inside gta3.img that is modifed that is also on the list of bad gta3.img files. (Files that can't be modified in gta3.img)
// REQUIRES: The player is using AC.
//
// Params:
// playerid     	- Player who's gta3.img is modified.
// filename[]   	- File inside gta3.img that is modified.
// md5[]        	- The file inside gta3.img's md5.
//
// Returns: irrelevant
forward AC_OnImgFileModifed(playerid, filename[], md5[]);

// PURPOSE: Callback for when a file (usually) inside the players GTA directory md5 checksum has been calculated.
// REQUIRES: The player is using AC.
//
// Params:
// playerid         - Player who's file was calculated
// filename[]       - The file name of the file that was calculated
// md5[]            - MD5 of the file
// bool:isCheat   	- true if the file IS on the AC blacklist
//
// Returns: irrelevant
forward AC_OnFileCalculated(playerid, filename[], md5[], bool:isCheat);

// PURPOSE: Callback for when a player pauses, unpauses, or alt tabs from the game
// REQUIRES: The player is using AC.
// NOTES: With the AC loaded in player's game, a pause or an alt tab still makes the game run in the background, so you can still damage players who are paused or alt tabbed.
// Pausing or alt tabbing has no benefit with the AC on.
//
// Params:
// playerid         - Player whos pause state changed
// type             - 0 = paused, 1 = alt tab
// bool:pause       - 0 = unpause or came back into the game from an alt tab, 1 = pause or alt tab
//
// Returns: irrelevant
forward AC_OnPlayerPause(playerid, type, bool:pause);

// PURPOSE: Callback for when a player takes a screenshot of their game using F8 only.
// REQUIRES: The player is using AC.
// NOTES: This callback does not check if the screenshot was successful at taking
// NOTES: This callback is only trigged if the player presses F8 while in game, so it may not corrospond perfectly to when they take a screenshot.
//
// Params:
// playerid         - Player who just took a screenshot
//
// Returns: irrelevant
forward AC_OnScreenshotTaken(playerid);

// PURPOSE: Callback for when a player's hardware ID has been initially calculated.
// REQUIRES: The player is using AC.
// NOTES: THIS CALLBACK IS CALLED BEFORE ONPLAYERCONNECT!!! - VERY IMPORTANT!
//
// Params:
// playerid         - Player who just had their hardware id calculated.
// hwid[]			- The hardware ID
// len				- The length of the hardware ID.
//
// Returns: irrelevant
forward AC_OnHardwareIDCalculated(playerid, hwid[], len);

// PURPOSE: Callback for when a player's ban status is retrieved.
// REQUIRES: The player is using AC.
// NOTES: This callback might never get called.
//
// Params:
// playerid         - Player who just had their hardware id calculated.
// status           - (-1) = unknown ban status, 0 = not banned, 1 = banned 
//
// Returns: irrelevant
forward AC_OnBanStatusRetrieved(playerid, status);

stock TogglePlayerCrouchBug(playerid, bool:toggle)
{
	SetPlayerCrouchBug(playerid, toggle ? 9999 : 1);
}

#define HARDWAREID_LEN 32
