#pragma once

#include <map>
#include "BitStream.h"
#include <cstddef>

#define RPC_CALLBACK void
#define RPC_ARGS RakNet::BitStream& bsData, int iExtra

typedef RPC_CALLBACK(*RPCFunct_t)(RPC_ARGS);

enum eRPC : unsigned short
{
	// -- CLIENT RPCs --

	// MAKE SURE VERSION_NOT_COMPATIBLE IS THE FIRST RPC AT ALL TIMES
	// VERY IMPORTANT.
	// ------------------------------------------------------------------------------
	// Called when the server determines the client's AC version is not compatible.
	VERSION_NOT_COMPATIBLE,

	// Called when the player has sent it's initial info.
	ON_INITIAL_INFO,

	// Called when the server asks the client to send verified packet.
	VERIFY_CLIENT,

	// Called when the server requests a region of memory checksum to be calculated.
	MD5_MEMORY_REGION,

	// Called when the server requests a file to be md5'd 
	MD5_FILE,

	// Called when the server wants to turn off switch reload for a player.
	TOGGLE_SWITCH_RELOAD,

	// Called when the server wants to set the frame limit.
	SET_FRAME_LIMIT,

	// Called when the server requests the client to exit process
	EXIT_PROCESS,

	// Called when the server wants to turn on or off crouch bug.
	TOGGLE_CROUCH_BUG,

	// Called when the server wants to turn on or off unlimited sprint.
	TOGGLE_UNLIMITED_SPRINT,

	// Called when the server wants to turn on or off macro limitations.
	TOGGLE_MACRO_LIMITS,

	// Called when the server wants to set the sprint speed limit.
	SET_SPRINT_LIMIT,

	// Called when the server wants to turn on or off sprinting on all surfaces
	TOGGLE_SPRINT_ALL_SURFACES,

	// Called when the server wants to turn on or off vehicle blips.
	TOGGLE_VEHICLE_BLIPS,

	// -- SERVER RPCs --

	// Called when the client sends verified packet to the server
	ON_CLIENT_VERIFIED,

	// Called when a new module (in the current process) or process has been run
	ON_FILE_EXECUTED,
	
	// Called when an md5 checksum on a region of memory has been calculated
	ON_MD5_CALCULATED,

	// Called when a file's md5 has been calculated, most likely in the GTA directory.
	ON_FILE_CALCULATED,

	// Called with an old version of SAMP_AC. Should never be called normally.
	VERSION_INCOMPAT2,

	// Called when a file inside gta3.img's md5 has been calculated.
	ON_IMG_FILE_MODIFIED,

	// Called when a macro is detected on the client machine.
	ON_MACRO_DETECTED,

	// Called when the player is using a virtual machine, debugger, or the CRC image doesn't match
	ON_TAMPER_ATTEMPT,

	// Called when a player pauses or unpauses, or alt tabs from the game.
	TOGGLE_PAUSE,

	// Called when a player takes a screenshot
	TAKE_SCREENSHOT,

	// Called when CPacketIntegrity::Check finds suspicious activity
	ON_UNKNOWN_SENDPACKET_CALLER_FOUND
};

class CRPC
{
public:
	static void Add(unsigned short usRPCId, RPCFunct_t func);
	static void Process(unsigned short usRPCId, RakNet::BitStream& bsData, int iExtra = NULL);

private:
	static std::map<unsigned short, RPCFunct_t> m_functions;

};
