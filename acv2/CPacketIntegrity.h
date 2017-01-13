#pragma once

class CPacketIntegrity 
{
public:
	// NOTE: Must be called before a BitStream is sent to RakClientInterface.
	// PURPOSE: Save the BitStream data for further checks.
	static void Send(BitStream *bitStream);
	// PURPOSE: Check if packet that is about to be sent was modified.
	static void Check(const char *data, int& size_in_bits, char *new_data);
private:
	static char* m_Data;
	static size_t m_SizeInBits;
};