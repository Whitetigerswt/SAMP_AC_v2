#include <boost\thread.hpp>

#include "Network\CRakClientHandler.h"
#include "CPacketIntegrity.h"
#include "Misc.h"
#include "CLog.h"

const int MAX_TIME_DIFFERENCE = 1500;

std::vector<CPacketIntegrity*> CPacketIntegrity::m_instances;
boost::shared_mutex CPacketIntegrity::m_access;

CPacketIntegrity::CPacketIntegrity(BitStream *bitStream)
{
	m_time = GetTickCount();
	m_md5 = Misc::MD5_Memory(reinterpret_cast<int>(bitStream->GetData()), BITS_TO_BYTES(bitStream->GetNumberOfBitsUsed()));

	boost::unique_lock<boost::shared_mutex> lock(m_access);
	m_instances.push_back(this);
}

CPacketIntegrity::~CPacketIntegrity()
{
	boost::unique_lock<boost::shared_mutex> lock(m_access);
	m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), this), m_instances.end());
}

bool CPacketIntegrity::Compare(std::string const& md5)
{
	return md5 == m_md5;
}

void CPacketIntegrity::Check(const char *data, int size_in_bits)
{
	// Maybe this check should be done in another thread?
	// Is MD5'ing every packet slowing down sending process? or causing fps drops?

	CLog("integrity.txt").Write(__FUNCTION__" called");
	unsigned int tick = GetTickCount();
	std::string md5 = Misc::MD5_Memory(reinterpret_cast<int>(data), BITS_TO_BYTES(size_in_bits));
	std::vector<std::pair<CPacketIntegrity*, bool>> packets_to_remove; // bool = was packet lost?
	
	{
		boost::shared_lock<boost::shared_mutex> lock(m_access);
		for (auto& p : m_instances)
		{
			if (tick - p->m_time > MAX_TIME_DIFFERENCE)
				packets_to_remove.push_back(std::make_pair(p, true));
			else if (p->Compare(md5))
				packets_to_remove.push_back(std::make_pair(p, false));
		}
	}
	
	for (auto& p : packets_to_remove)
	{
		if (p.second)
		{
			CLog("integrity.txt").Write(__FUNCTION__" packet lost");
			// PACKET LOST!
		}

		delete p.first;
	}
}