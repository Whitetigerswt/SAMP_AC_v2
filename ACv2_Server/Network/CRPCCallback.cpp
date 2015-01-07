#include "CRPCCallback.h"
#include "../Utility.h"
#include "../Callback.h"
#include "Network.h"

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
		Network::GetPlayerFromPlayerid(iExtra)->OnFileExecuted((char*)processpath, (char*)md5);
		Utility::Printf("process: %s, md5: %s", processpath, md5);
	}
}

RPC_CALLBACK CRPCCallback::OnMD5Calculated(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("md5 calc called: iExtra: %d", iExtra);
	// Create variables
	int address, size;
	unsigned char md5[128];

	// Reset string value
	memset(md5, 0, sizeof(md5));

	// Read values sent from client.
	if (bsData.Read(address) && bsData.Read(size) && bsData.Read((char*)md5))
	{
		// Call the main function with the info we got.
		Network::GetPlayerFromPlayerid(iExtra)->OnMD5Calculated(address, size, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnFileCalculated(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("filecalc called: iExtra: %d", iExtra);
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
		Network::GetPlayerFromPlayerid(iExtra)->OnFileCalculated((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnImgFileModified(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("imgmod called: iExtra: %d", iExtra);
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
		Network::GetPlayerFromPlayerid(iExtra)->OnImgFileModified((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnMacroDetected(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("macro called: iExtra: %d", iExtra);
	// Create an int var to hold the virtual key the macro was detected on.
	int vKey;

	// Read the data the client said was a key used by a macro
	if (bsData.Read(vKey))
	{
		// Let our helper class know.
		Network::GetPlayerFromPlayerid(iExtra)->OnMacroDetected(vKey);
	}
}

RPC_CALLBACK CRPCCallback::OnIntialInfoGotten(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("initial info called: iExtra: %d", iExtra);
	// Create a big variable to hold hardware ID.
	unsigned char hwid[2048];
	float version;

	// Reset it's memory.
	memset(hwid, 0, sizeof(hwid));

	// Read the hardware ID from the client.
	if (bsData.Read((char*)hwid) && bsData.Read(version))
	{
		// Send to our helper class so it can store it.
		Network::GetPlayerFromPlayerid(iExtra)->OnHardwareCalculated((char*)hwid);

		// Check the version compatiblity.
		Network::GetPlayerFromPlayerid(iExtra)->CheckVersionCompatible(version);
	}
}

RPC_CALLBACK CRPCCallback::OnTamperAttempt(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("tamper called: iExtra: %d", iExtra);
	Network::GetPlayerFromPlayerid(iExtra)->OnTamperAttempt();
	Network::PlayerSendRPC(EXIT_PROCESS, iExtra);
}

RPC_CALLBACK CRPCCallback::OnPauseToggled(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("pause called: iExtra: %d", iExtra);
	// Record the data the client sent us.
	int iType;
	bool bPause;

	// Read the data.
	if (bsData.Read(iType) && bsData.Read(bPause))
	{
		Network::GetPlayerFromPlayerid(iExtra)->TogglePause(iType, bPause);
	}
}

RPC_CALLBACK CRPCCallback::OnTakeScreenshot(RakNet::BitStream &bsData, int iExtra)
{
	Utility::Printf("TakeScreenshot called: iExtra: %d", iExtra);
	Network::GetPlayerFromPlayerid(iExtra)->OnScreenshotTaken();
}