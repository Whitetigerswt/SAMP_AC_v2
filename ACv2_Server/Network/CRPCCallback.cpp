#include "CRPCCallback.h"
#include "../Utility.h"
#include "../Callback.h"
#include "Network.h"
#include "../CAntiCheatHandler.h"

#include <string>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

void CRPCCallback::Initialize()
{
	// Add RPC Callback functions.
	CRPC::Add(ON_FILE_EXECUTED, OnFileExecuted);
	CRPC::Add(ON_MD5_CALCULATED, OnMD5Calculated);
	CRPC::Add(ON_FILE_CALCULATED, OnFileCalculated);
	CRPC::Add(ON_IMG_FILE_MODIFIED, OnImgFileModified);
	CRPC::Add(ON_MACRO_DETECTED, OnMacroDetected);
	CRPC::Add(ON_INITIAL_INFO, OnIntialInfoGotten);
	CRPC::Add(ON_TAMPER_ATTEMPT, OnTamperAttempt);
	CRPC::Add(TOGGLE_PAUSE, OnPauseToggled);
	CRPC::Add(TAKE_SCREENSHOT, OnTakeScreenshot);
}


RPC_CALLBACK CRPCCallback::OnFileExecuted(RakNet::BitStream& bsData, int iExtra)
{
	// Allocate space for the process path and md5 parameters.
	unsigned char processpath[256];
	unsigned char md5[256];

	// Reset those variables values.
	memset(processpath, 0, sizeof(processpath));
	memset(md5, 0, sizeof(md5));

	// Read new values for those variables.
	if (bsData.Read((char*)processpath) && bsData.Read((char*)md5))
	{
		// Call the main OnFileExecuted function.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnFileExecuted((char*)processpath, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnMD5Calculated(RakNet::BitStream &bsData, int iExtra)
{
	// Create variables
	int address, size;
	unsigned char md5[128];

	// Reset string value
	memset(md5, 0, sizeof(md5));

	// Read values sent from client.
	if (bsData.Read(address) && bsData.Read(size) && bsData.Read((char*)md5))
	{
		// Call the main function with the info we got.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnMD5Calculated(address, size, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnFileCalculated(RakNet::BitStream &bsData, int iExtra)
{
	// Create variables to hold the file path and md5
	unsigned char path[MAX_PATH + 1];
	unsigned char md5[33];

	// Reset memory.
	memset(path, 0, sizeof(path));
	memset(md5, 0, sizeof(md5));

	// Read the data the client sent us.
	if(bsData.Read((char*)path) && bsData.Read((char*)md5)) 
	{
		// Call the main function with the info we got.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnFileCalculated((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnImgFileModified(RakNet::BitStream &bsData, int iExtra)
{
	// Create variables to hold the file path and md5
	unsigned char path[MAX_PATH + 1];
	unsigned char md5[33];

	// Reset memory
	memset(path, 0, sizeof(path));
	memset(md5, 0, sizeof(md5));

	// Read the data sent to us by the server.
	if (bsData.Read((char*)path) && bsData.Read((char*)md5))
	{
		// Use our helper class's function.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnImgFileModified((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnMacroDetected(RakNet::BitStream &bsData, int iExtra)
{
	// Create an int var to hold the virtual key the macro was detected on.
	int vKey;

	// Read the data the client said was a key used by a macro
	if (bsData.Read(vKey))
	{
		// Let our helper class know.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnMacroDetected(vKey);
	}
}

RPC_CALLBACK CRPCCallback::OnIntialInfoGotten(RakNet::BitStream &bsData, int iExtra)
{
	CAntiCheatHandler::Init(iExtra);

	// Create a big variable to hold hardware ID.
	unsigned char hwid[2048];
	float version;

	// Reset it's memory.
	memset(hwid, 0, sizeof(hwid));

	// Read the hardware ID from the client.
	if (bsData.Read((char*)hwid) && bsData.Read(version))
	{
		// Send to our helper class so it can store it.
		CAntiCheatHandler::GetAntiCheat(iExtra)->OnHardwareCalculated((char*)hwid);

		// Check the version compatiblity.
		CAntiCheatHandler::GetAntiCheat(iExtra)->CheckVersionCompatible(version);
	}
}

RPC_CALLBACK CRPCCallback::OnTamperAttempt(RakNet::BitStream &bsData, int iExtra)
{
	CAntiCheatHandler::GetAntiCheat(iExtra)->OnTamperAttempt();

	// Create packet
	RakNet::BitStream bitStream;

	// Write header to packet
	bitStream.Write((unsigned char)PACKET_RPC);
	bitStream.Write(EXIT_PROCESS);

	Network::PlayerSend(iExtra, &bitStream);
}

RPC_CALLBACK CRPCCallback::OnPauseToggled(RakNet::BitStream &bsData, int iExtra)
{
	// Record the data the client sent us.
	int iType;
	bool bPause;

	// Read the data.
	if (bsData.Read(iType) && bsData.Read(bPause))
	{
		CAntiCheatHandler::GetAntiCheat(iExtra)->TogglePause(iType, bPause);
	}
}

RPC_CALLBACK CRPCCallback::OnTakeScreenshot(RakNet::BitStream &bsData, int iExtra)
{
	CAntiCheatHandler::GetAntiCheat(iExtra)->OnScreenshotTaken();
}