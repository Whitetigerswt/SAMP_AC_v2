#pragma once

#include <Windows.h>
#include <boost\thread.hpp>

class CPacketIntegrity 
{
public:
	// NOTE: Must be called before a BitStream is sent to RakClientInterface.
	// PURPOSE: Save the md5 of BitStream data for further checks.
	CPacketIntegrity(BitStream *bitStream);

	// PURPOSE: Delete from memory saved BitStream hash.
	~CPacketIntegrity();

	// PURPOSE: Check if this packet hash is equal to "md5" param.
	bool Compare(std::string const& md5);

	// PURPOSE: Get handles for modules allowed to call RakPeer::SendBuffered
	static void GlobalInitialize();

	// PURPOSE: 1) Look for lost packets.
	// PURPOSE: 2) Clean memory.
	// PURPOSE: 3) Validate RakPeer::SendBuffered calltrace
	static bool Check(const char *data, int size_in_bits);

private:
	std::string m_md5;
	unsigned int m_time;

	static std::vector<CPacketIntegrity*> m_instances;
	static boost::shared_mutex m_instancesAccess;
	static boost::shared_mutex m_lostAccess;
	static int m_lostPackets;

	static std::vector<HMODULE> m_allowedModules;
	static DWORD m_tlsSkipModuleCheck;
};