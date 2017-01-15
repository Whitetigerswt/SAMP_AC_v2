#pragma once

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

	// PURPOSE: 1) Look for lost packets.
	// PURPOSE: 2) Clean memory.
	static void Check(const char *data, int size_in_bits);

private:
	std::string m_md5;
	unsigned int m_time;

	static std::vector<CPacketIntegrity*> m_instances;
	static boost::shared_mutex m_access;
};