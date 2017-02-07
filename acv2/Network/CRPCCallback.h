#pragma once

#include "../../Shared/Network/CRPC.h"

class CRPCCallback
{
public:
	// PURPOSE: Register the RPCs.
	// REQUIRES: The client is connected to the server.
	static void Initialize();

private:
	// PURPOSE: Sends the initial info to the server after the connection has been accepted and OnPlayerConnect has been called on the server.
	// REQUIRES: NULL
	static void ResendFileInformation();

	// PURPOSE: do all the things the client should do when we connect to the server.
	// REQUIRES: The player to have just been connected to the server.
	static void OnConnect();

	// PURPOSE: An RPC callback to make client send verification packet to server.
	// REQUIRES: The server sends the VERIFY_CLIENT RPC to the client.
	static RPC_CALLBACK VerifyClient(RPC_ARGS);

	// PURPOSE: An RPC callback to get an md5 checksum of a specific memory region.
	// REQUIRES: The server sends the MD5_MEMORY_REGION RPC to the client.
	static RPC_CALLBACK MD5_Memory_Region(RPC_ARGS);

	// PURPOSE: An RPC callback to get an MD5 of a specific file on the client's machine.
	// REQUIRES: The server sends the MD5_FILE RPC to the client.
	static RPC_CALLBACK MD5_File(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle the players ability to use switch reload.
	// REQUIRES: The server sends the TOGGLE_SWITCH_RELOAD RPC to the client.
	static RPC_CALLBACK ToggleSwitchReload(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle the players frame limiter.
	// REQUIRES: The server sends the TOGGLE_FRAME_LIMITER RPC to the client.
	static RPC_CALLBACK ToggleFrameLimiter(RPC_ARGS);

	// PURPOSE: An RPC callback to set the players frame limit.
	// REQUIRES: The server sends the SET_FRAME_LIMIT RPC to the client.
	static RPC_CALLBACK SetFrameLimit(RPC_ARGS);

	// PURPOSE: An RPC callback to end the current process
	// REQUIRES: The server sends the EXIT_PROCESS RPC to the client.
	static RPC_CALLBACK ExitThisProcess(RPC_ARGS);

	// PURPOSE: An RPC callback to tell the AC not to continue to monitor anything, and unload itself.
	// REQUIRES: The server sends the VERSION_NOT_COMPATIBLE RPC to the client.
	static RPC_CALLBACK VersionNotCompatible(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off cbug
	// REQUIRES: The server sends the TOGGLE_CROUCH_BUG RPC to the client.
	static RPC_CALLBACK ToggleCrouchBug(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off Lite foot.
	// REQUIRES: The server sends the TOGGLE_LITE_FOOT RPC to the client.
	static RPC_CALLBACK ToggleLiteFoot(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off unlimited sprint.
	// REQUIRES: The server sends the TOGGLE_UNLIMMITED_SPRINT RPC to the client.
	static RPC_CALLBACK ToggleUnlimitedSprint(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off macro limitations.
	// REQUIRES: The server sends the TOGGLE_MACRO_LIMITS RPC to the client.
	static RPC_CALLBACK ToggleMacroLimits(RPC_ARGS);

	// PURPOSE: An RPC callback to set sprint speed limit
	// REQUIRES: The server sends the SET_SPRINT_LIMIT RPC to the client.
	static RPC_CALLBACK SetSprintLimit(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off sprinting on all surfaces.
	// REQUIRES: The server sends the TOGGLE_SPRINT_ALL_SURFACES RPC to the client.
	static RPC_CALLBACK ToggleSprintOnAllSurfaces(RPC_ARGS);

	// PURPOSE: An RPC callback to toggle on or off vehicle blips.
	// REQUIRES: The server sends the TOGGLE_VEHICLE_BLIPS RPC to the client.
	static RPC_CALLBACK ToggleVehicleBlips(RPC_ARGS);
};