#include "Network\CRakClientHandler.h"
#include "CPacketIntegrity.h"
#include "CMem.h"
#include "CLog.h"

char *CPacketIntegrity::m_Data = nullptr;
size_t CPacketIntegrity::m_SizeInBits = 0;

void CPacketIntegrity::Send(BitStream *bitStream)
{
	CLog("integrity.txt").Write(__FUNCTION__" called");
	if (m_Data != nullptr) 
	{
		// last packet was 'lost' on the way to RakPeer::SendBuffered() meaning that somebody blocked our packet, or see below:
		// sometimes this function gives false alarms, or at least in early stages of connection initialization
		
		// force quit?

		CLog("integrity.txt").Write("lost packet");
		delete[] m_Data;
	}

	// check if previous packet was sent
	m_SizeInBits = bitStream->GetNumberOfBitsUsed();
	m_Data = new char[BITS_TO_BYTES(m_SizeInBits)]; // m_SizeInBytes can't be 0, right?
	CMem::Cpy(m_Data, bitStream->GetData(), BITS_TO_BYTES(m_SizeInBits));
}

void CPacketIntegrity::Check(const char *data, int& size_in_bits, char *new_data)
{
	CLog("integrity.txt").Write(__FUNCTION__" called");
	if (m_Data != nullptr) 
	{
		if (m_SizeInBits != size_in_bits || memcmp(data, m_Data, BITS_TO_BYTES(m_SizeInBits)) != 0)
		{
			// our packet was modified; we're putting back original data
			size_in_bits = m_SizeInBits;
			new_data = new char[BITS_TO_BYTES(m_SizeInBits)];
			CMem::Cpy(new_data, m_Data, BITS_TO_BYTES(m_SizeInBits));

			CLog("integrity.txt").Write("modified packet");
			// announce the server?
		}
		delete[] m_Data;
		m_Data = nullptr;
	}
}