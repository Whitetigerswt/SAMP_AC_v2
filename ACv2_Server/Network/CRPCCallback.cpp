#include "CRPCCallback.h"
#include "../Utility.h"
#include "../Callback.h"
#include "Network.h"
#include "../CPlayer.h"

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
	CRPC::Add(ON_HARDWAREID_SENT, OnHardwareIDGotten);
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
		Network::GetPlayerFromPlayerid(iExtra)->OnMD5Calculated(address, size, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnFileCalculated(RakNet::BitStream &bsData, int iExtra)
{
	unsigned char path[MAX_PATH + 1];
	unsigned char md5[33];

	memset(path, 0, sizeof(path));
	memset(md5, 0, sizeof(md5));

	if(bsData.Read((char*)path) && bsData.Read((char*)md5)) 
	{
		Network::GetPlayerFromPlayerid(iExtra)->OnFileCalculated((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnImgFileModified(RakNet::BitStream &bsData, int iExtra)
{
	unsigned char path[MAX_PATH + 1];
	unsigned char md5[33];

	memset(path, 0, sizeof(path));
	memset(md5, 0, sizeof(md5));

	if (bsData.Read((char*)path) && bsData.Read((char*)md5))
	{
		Network::GetPlayerFromPlayerid(iExtra)->OnImgFileModified((char*)path, (char*)md5);
	}
}

RPC_CALLBACK CRPCCallback::OnMacroDetected(RakNet::BitStream &bsData, int iExtra)
{
	int vKey;

	if (bsData.Read(vKey))
	{
		Network::GetPlayerFromPlayerid(iExtra)->OnMacroDetected(vKey);
	}
}

RPC_CALLBACK CRPCCallback::OnHardwareIDGotten(RakNet::BitStream &bsData, int iExtra)
{
	unsigned char hwid[2048];

	memset(hwid, 0, sizeof(hwid));

	if (bsData.Read((char*)hwid))
	{
		Network::GetPlayerFromPlayerid(iExtra)->OnHardwareCalculated((char*)hwid);
	}
}