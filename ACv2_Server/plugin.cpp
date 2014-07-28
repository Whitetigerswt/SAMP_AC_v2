#include "plugin.h"
#include "Utility.h"
#include "../Shared/RakNet/BitStream.h"
#include "../Shared/Network/CRPC.h"
#include "GDK/core.h"
#include "GDK/a_players.h"
#include "Callback.h"

/*cell AMX_NATIVE_CALL CallbackProc(AMX* pAmx, cell* pParams)
{
	return Callback::Process(pAmx, (Callback::eCallbackType)pParams[1], &pParams[2]);
}*/


cell AMX_NATIVE_CALL IsPlayerUsingSAMPACProc(AMX* pAmx, cell* pParams)
{
	// Return the result if the player is connected to AC or not.
	return Network::IsPlayerConnectedToAC(pParams[1]);
}

cell AMX_NATIVE_CALL MD5_MemoryProc(AMX* pAmx, cell* pParams)
{
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
	// Toggle if the player can enable AC or not.
	CAntiCheat::ToggleCanEnableAC(pParams[1], !!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL CanPlayerEnableACProc(AMX* pAmx, cell* pParams)
{
	// Return if the player can enable AC or not.
	return CAntiCheat::CanEnableAC(pParams[1]);
}


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	// Handle network related processing.
	Network::Process();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	// Initialize
	Utility::Initialize(ppData);

	// Print out that we've loaded successfully.
	Utility::Printf("SAMP Anti-Cheat v2.0 Has loaded successfully.");

	// Initialize raknet server to be connected to by the AC.
	Network::Initialize("", 7776, 500);

	// Load sampGDK.
	return sampgdk::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// The plugin has been unloaded, let them know we're leaving :(
	Utility::Printf("Unloaded SAMP Anti-Cheat v2.0");
	return sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ "IsPlayerUsingSampAC", IsPlayerUsingSAMPACProc },
	{ "CanPlayerEnableAC", CanPlayerEnableACProc },
	{ "SetPlayerCanEnableAC", SetPlayerCanEnableACProc },
	{ "MD5_Memory", MD5_MemoryProc },
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