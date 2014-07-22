#include "CRPCCallback.h"
#include "../CLog.h"
#include "../CLoader.h"
#include "../Misc.h"
#include "Network.h"

#include <Boost\thread.hpp>


void CRPCCallback::Initialize()
{
	CRPC::Add(MD5_MEMORY_REGION, MD5_Memory_Region);
	boost::thread ResendFilesThread(&ResendFileInformation);
}

void CRPCCallback::ResendFileInformation()
{
	CLoader::Processes.ResendFiles();
	CLoader::Modules.ResendFiles();
}

void CRPCCallback::MD5_Memory_Region(RakNet::BitStream &bsData, int iExtra)
{
	// Create variables to store the info the server sent.
	int address, size;

	if (bsData.Read(address) && bsData.Read(size))
	{
		// Calculate the result of the MD5 hash at the address and put it in an std::string
		std::string md5 = Misc::MD5_Memory(address, size);

		// Send the result of the hash back to the server.
		RakNet::BitStream bitStream;
		bitStream.Write(address);
		bitStream.Write(size);
		bitStream.Write(md5.c_str());

		Network::SendRPC(ON_MD5_CALCULATED, &bitStream);
	}
}