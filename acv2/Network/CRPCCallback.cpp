#include "CRPCCallback.h"
#include "CRakClientHandler.h"
#include "../CLoader.h"
#include "../CLog.h"
#include "../Misc.h"
#include "../Addresses.h"
#include "../CHookManager.h"
#include "../CMem.h"
#include "../../Shared/Network/Network.h"
#include "../../Shared/Network/ACVerifiedPacket.h"

#include <Boost\thread.hpp>
#include <tchar.h>

void CRPCCallback::Initialize()
{
	CRPC::Add(VERIFY_CLIENT, VerifyClient);
	CRPC::Add(MD5_MEMORY_REGION, MD5_Memory_Region);
	CRPC::Add(MD5_FILE, MD5_File);
	CRPC::Add(TOGGLE_SWITCH_RELOAD, ToggleSwitchReload);
	CRPC::Add(SET_FRAME_LIMIT, SetFrameLimit);
	CRPC::Add(EXIT_PROCESS, ExitThisProcess);
	CRPC::Add(VERSION_NOT_COMPATIBLE, VersionNotCompatible);
	CRPC::Add(TOGGLE_CROUCH_BUG, ToggleCrouchBug);
	CRPC::Add(TOGGLE_UNLIMITED_SPRINT, ToggleUnlimitedSprint);
	CRPC::Add(TOGGLE_MACRO_LIMITS, ToggleMacroLimits);
	CRPC::Add(TOGGLE_SPRINT_ALL_SURFACES, ToggleSprintOnAllSurfaces);
	CRPC::Add(TOGGLE_VEHICLE_BLIPS, ToggleVehicleBlips);
	CRPC::Add(SET_SPRINT_LIMIT, SetSprintLimit);

	boost::thread connectThread(&OnConnect);
}

void CRPCCallback::OnConnect()
{
	while (!CRakClientHandler::IsConnected())
	{
		// note: if the server is NOT an AC server, the AC will be stuck in this loop forever.
		Sleep(5);
	}

	CHookManager::SetConnectPatches();
	ResendFileInformation();
}

void CRPCCallback::ResendFileInformation()
{
	// Send the server the processes and modules that were loaded while we weren't connected.
	CLoader::Processes.ResendFiles();
	CLoader::Modules.ResendFiles();
	CLoader::GtaDirectory.Scan(Misc::GetGTADirectory());
}

void CRPCCallback::VerifyClient(RakNet::BitStream &bsData, int iExtra)
{
	RakNet::BitStream bitStream;
	unsigned char preparedData[ACVerifiedPacket::MAX_ARRAY_SIZE];
	unsigned char preparedDataSize = 0;

	ACVerifiedPacket::Verify(bsData, preparedData, preparedDataSize);

	// Add header info
	bitStream.Write((unsigned char)PACKET_RPC);
	bitStream.Write(ON_CLIENT_VERIFIED);
	bitStream.Write((char*)&preparedData, preparedDataSize);

	// Send the RPC to the server.
	CRakClientHandler::CustomSend(&bitStream, MEDIUM_PRIORITY, RELIABLE);
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

		// Add header info to packet
		bitStream.Write((unsigned char)PACKET_RPC);
		bitStream.Write(ON_MD5_CALCULATED);

		bitStream.Write(address);
		bitStream.Write(size);

		// convert md5 string to bytes
		BYTE digest[16];
		std::string md5_string(md5);

		// if string isn't null
		if (strcmp(md5.c_str(), "NULL"))
		{
			for (int i = 0; i < 16; ++i)
			{
				std::string bt = md5_string.substr(i * 2, 2);
				digest[i] = static_cast<BYTE>(strtoul(bt.c_str(), NULL, 16));
				bitStream.Write(digest[i]);
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				digest[i] = NULL;
				bitStream.Write(digest[i]);
			}
		}

		// Send the RPC to the server.
		CRakClientHandler::CustomSend(&bitStream, LOW_PRIORITY, RELIABLE);
	}
}

void CRPCCallback::MD5_File(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the file path sent to us by the server.
	unsigned char file[MAX_PATH + 1];

	// Read data sent to us by the server (which in this case is the file name)
	if (bsData.ReadString(file))
	{
		// MD5 that file and store the result.
		std::string result = CLoader::GtaDirectory.MD5_Specific_File(Misc::utf8_decode((char*)file));

		// Convert the file into a std::string, so we can remove any macros that were sent in the file name.
		std::string szFile(reinterpret_cast<char*>(file));

		// Find the occurance of $(GtaDirectory) macro.
		int i = szFile.find("$(GtaDirectory)/");

		RakNet::BitStream bsData;

		// Add header info to packet.
		bsData.Write((unsigned char)PACKET_RPC);
		bsData.Write(ON_FILE_CALCULATED);

		// Cut out the $(GtaDirectory) macro when we send it back to the server.
		std::string szFileInGTADirectory = std::string(szFile.substr(i + 16));

		bsData.Write((unsigned short)szFileInGTADirectory.length());
		bsData.Write((const char*)szFileInGTADirectory.c_str(), szFileInGTADirectory.length());

		// convert md5 string to bytes
		BYTE digest[16];

		// if string isn't null
		if (strcmp(result.c_str(), "NULL"))
		{
			for (int i = 0; i < 16; ++i)
			{
				std::string bt = result.substr(i * 2, 2);
				digest[i] = static_cast<BYTE>(strtoul(bt.c_str(), NULL, 16));
				bsData.Write(digest[i]);
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				digest[i] = NULL;
				bsData.Write(digest[i]);
			}
		}

		// Call the RPC.
		CRakClientHandler::CustomSend(&bsData, LOW_PRIORITY, RELIABLE);
	}
}

void CRPCCallback::ToggleSwitchReload(RakNet::BitStream &bsData, int iExtra)
{
	// Create a variable to hold what the server wants to do
	bool toggle;
	
	// Read the data sent to us by the server.
	if (bsData.Read(toggle))
	{
		// Unprotect the switch reload address.
		DWORD dwOldProt;
		VirtualProtect(SWITCH_RELOAD, 6, PAGE_EXECUTE_READWRITE, &dwOldProt);

		// Set the player's ability to switch reload.
		if (!toggle)
		{
			memcpy(SWITCH_RELOAD, "\x90\x90\x90\x90\x90\x90", 6); // nop
		}
		else
		{
			memcpy(SWITCH_RELOAD, "\x89\x88\xA8\x05\x00\x00", 6); // mov [eax+000005A8],ecx
		}
	}
}

void CRPCCallback::SetFrameLimit(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the new frame limit.
	int fpslimit;

	// Read the value sent to us by the server.
	if (bsData.Read(fpslimit))
	{
		// Make sure the frame limiter is disabled in the game.
		CHookManager::SetFrameLimiterPatch();

		// Set the new frame limit.
		Misc::SetFPSLimit(fpslimit);
	}
}

void CRPCCallback::ToggleCrouchBug(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the value the server sent.
	int toggle;

	// Read the value the server sent us.
	if (bsData.Read(toggle))
	{
		// Set the crouch bug status to the one sent to us by the server.
		Misc::SetCrouchBug(toggle);
	}
}

void CRPCCallback::ExitThisProcess(RakNet::BitStream &bsData, int iExtra)
{
	ExitProcess(0);
}

void CRPCCallback::VersionNotCompatible(RakNet::BitStream &bsData, int iExtra)
{
	// Disconnect from the server.
	//Network::Disconnect();
	CRakClientHandler::Disconnect();
}

void CRPCCallback::ToggleUnlimitedSprint(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the server data.
	bool toggle;

	// Read the data
	if (bsData.Read(toggle))
	{
		if (toggle)
		{
			CMem::Cpy((void*)0x60A5B0, "\x68\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90", 13); // nop
		}
		else
		{
			// push C3160000
			// call 00420800
			// fstp dword ptr [esi+18]
			CMem::Cpy((void*)0x60A5B0, "\x68\x00\x00\x16\xC3\xE8\x46\x62\xE1\xFF\xD9\x5E\x18", 13);
		}
	}
}

void CRPCCallback::ToggleMacroLimits(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the servers new toggle value.
	bool toggle;

	// Read the data the server sent.
	if (bsData.Read(toggle))
	{
		// Set the value the server sent.
		Misc::SetMacroLocks(toggle);
	}
}

void CRPCCallback::SetSprintLimit(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the value the server sent.
	float speed;

	// Read the value the server sent us.
	if (bsData.Read(speed))
	{
		// Set the sprint speed limit to the one sent to us by the server.
		Misc::SetSprintLimit(speed);
	}
}

void CRPCCallback::ToggleSprintOnAllSurfaces(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the servers new toggle value.
	bool toggle;

	// Read the data the server sent.
	if (bsData.Read(toggle))
	{
		// Set the value the server sent.
		Misc::SetSprintOnAllSurfaces(toggle);

		// Actually toggle the value.
		CHookManager::ToggleSprintOnAllSurfaces(toggle);
	}
}

void CRPCCallback::ToggleVehicleBlips(RakNet::BitStream &bsData, int iExtra)
{
	// Create a new variable to hold the servers new toggle value.
	bool toggle;

	// Read the data the server sent.
	if (bsData.Read(toggle))
	{
		// Set the value the server sent.
		Misc::SetVehicleBlips(toggle);
	}
}