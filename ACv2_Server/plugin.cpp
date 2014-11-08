#include "plugin.h"
#include "Utility.h"
#include "../Shared/RakNet/BitStream.h"
#include "../Shared/Network/CRPC.h"
#include "GDK/core.h"
#include "GDK/a_players.h"
#include "Callback.h"
#include "SDK/samp-sdk/amx/amx.h"
#include "CServerUpdater.h"

cell AMX_NATIVE_CALL IsPlayerUsingSAMPACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "IsPlayerUsingSampAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Return the result if the player is connected to AC or not.
	return Network::IsPlayerConnectedToAC(pParams[1]);
}

cell AMX_NATIVE_CALL MD5_MemoryProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(3, "MD5_Memory");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// pParams[1] = playerid
	// pParams[2] = address
	// pParams[3] = size

	// Prepare to send the info to the client.
	RakNet::BitStream bitStream;

	// All cell's need to be casted as an int. 
	bitStream.WriteCasted<int, cell>(pParams[2]);
	bitStream.WriteCasted<int, cell>(pParams[3]);

	// Send the RPC to the client and return.
	return Network::PlayerSendRPC(MD5_MEMORY_REGION, pParams[1], &bitStream);
}

cell AMX_NATIVE_CALL SetPlayerCanEnableACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerCanEnableAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Toggle if the player can enable AC or not.
	CAntiCheat::ToggleCanEnableAC(pParams[1], !!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL CanPlayerEnableACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "CanPlayerEnableAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Return if the player can enable AC or not.
	return CAntiCheat::CanEnableAC(pParams[1]);
}

cell AMX_NATIVE_CALL GetPlayerHardwareIDProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(3, "GetPlayerHardwareID");

	// Get the player's CAntiCheat class pointer.
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// Get the address of the variable in the 2nd parameter.
	cell* cVarAddress = NULL;
	amx_GetAddr(pAmx, pParams[2], &cVarAddress);

	// Return the result of setting the 2nd parameter to the hardware ID string.
	return amx_SetString(cVarAddress, ac->GetPlayerHardwareID().c_str(), 0, 0, pParams[3]);
}

cell AMX_NATIVE_CALL TogglePlayerSwitchReloadProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerSwitchReload");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleSwitchReload(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL SetPlayerFPSLimitProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerFPSLimit");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected and the frame limit isn't 0
	if (!IsPlayerConnected(pParams[1]) || ac == NULL || pParams[2] == 0) return 0;

	// Set our internal recorder of the player's FPS limit.
	ac->SetFPSLimit(pParams[2]);

	// Create the RPC data to send to the client.
	RakNet::BitStream bsData;
	bsData.WriteCasted<int, cell>(pParams[2]);

	return Network::PlayerSendRPC(SET_FRAME_LIMIT, pParams[1], &bsData);
}

cell AMX_NATIVE_CALL GetPlayerFPSLimitProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerFPSLimit");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected and connected to AC
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// return the player's frame limit.
	return ac->GetFPSLimit();
}

cell AMX_NATIVE_CALL SetPlayerCrouchBugProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerCrouchBug");

	// Get CAntiCheat pointer.
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleCrouchBug(pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL TogglePlayerLiteFootProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerLiteFoot");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// Toggle lite foot.
	ac->ToggleLiteFoot(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerLiteFootProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerLiteFoot");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetLiteFoot();
}


cell AMX_NATIVE_CALL GetPlayerCrouchBugProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerCrouchBug");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetCrouchBug();
}


cell AMX_NATIVE_CALL GetPlayerSwitchReloadProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerSwitchReload");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetSwitchReload();
}

cell AMX_NATIVE_CALL TogglePlayerUnlimitedSprintProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerUnlimitedSprint");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleUnlimitedSprint(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerUnlimitedSprintProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerUnlimitedSprint");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetUnlimitedSprint();
}

cell AMX_NATIVE_CALL TogglePlayerMacroLimitsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerMacroLimits");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleMacroLimitations(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerMacroLimitsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerMacroLimits");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetMacroLimitations();
}

cell AMX_NATIVE_CALL TogglePlayerSprintOnAllSurfacesProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerSprintOnAllSurfaces");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleSprintOnAllSurfaces(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerSprintOnAllSurfacesProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerSprintOnAllSurfaces");

	// Get CAntiCheat pointer
	CAntiCheat* ac = Network::GetPlayerFromPlayerid(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetSprintOnAllSurfaces();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	// Handle network related processing.
	Network::Process();

	// Handle sampGDK ticking.
	return sampgdk::ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	// Initialize
	Utility::Initialize(ppData);

	// Print out that we've loaded successfully.
	Utility::Printf("SA-MP Anti-Cheat v%0.2f Has loaded successfully.", CURRENT_VERSION);

	// Load sampGDK.
	return sampgdk::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// The plugin has been unloaded, let them know we're leaving :(
	Utility::Printf("Unloaded SA-MP Anti-Cheat v%0.2f", CURRENT_VERSION);
	return sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ "IsPlayerUsingSampAC", IsPlayerUsingSAMPACProc },
	{ "CanPlayerEnableAC", CanPlayerEnableACProc },
	{ "SetPlayerCanEnableAC", SetPlayerCanEnableACProc },
	{ "MD5_Memory", MD5_MemoryProc },
	{ "GetPlayerHardwareID", GetPlayerHardwareIDProc },
	{ "TogglePlayerSwitchReload", TogglePlayerSwitchReloadProc },
	{ "SetPlayerFPSLimit", SetPlayerFPSLimitProc },
	{ "GetPlayerFPSLimit", GetPlayerFPSLimitProc },
	{ "SetPlayerCrouchBug", SetPlayerCrouchBugProc },
	{ "TogglePlayerLiteFoot", TogglePlayerLiteFootProc },
	{ "GetPlayerLiteFoot", GetPlayerLiteFootProc },
	{ "GetPlayerCrouchBug", GetPlayerCrouchBugProc },
	{ "GetPlayerSwitchReload", GetPlayerSwitchReloadProc },
	{ "TogglePlayerUnlimitedSprint", TogglePlayerUnlimitedSprintProc },
	{ "GetPlayerUnlimitedSprint", GetPlayerUnlimitedSprintProc },
	{ "TogglePlayerMacroLimits", TogglePlayerMacroLimitsProc },
	{ "GetPlayerMacroLimits", GetPlayerMacroLimitsProc },
	{ "TogglePlayerSprintOnAllSurfaces", TogglePlayerSprintOnAllSurfacesProc },
	{ "GetPlayerSprintOnAllSurfaces", GetPlayerSprintOnAllSurfacesProc },
	{ 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *pAmx)
{
	// Add a new AMX script to our list of running AMX files.
	Callback::GetAMXList().push_back(pAmx);
	
	// Allow the new loaded script to use our plugin's functions.
	return amx_Register(pAmx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *pAmx)
{
	// Remove the AMX scripts from our list of loaded AMX files.
	Callback::GetAMXList().remove(pAmx);
	return AMX_ERR_NONE;
}