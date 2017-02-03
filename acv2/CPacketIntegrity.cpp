#include <boost\thread.hpp>
#include <Windows.h>
#include <atomic>

#include "Network\CRakClientHandler.h"
#include "s0beit\samp.h"
#include "CPacketIntegrity.h"
#include "Misc.h"
#include "CLog.h"

const int MAX_TIME_DIFFERENCE = 1500;
std::atomic_ulong callCounter;

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

// If FramesToSkip = 0, first BackTrace entry points to this function (?)
USHORT WINAPI FIX_RtlCaptureStackBackTrace(ULONG FramesToSkip, ULONG FramesToCapture, PVOID *BackTrace, PULONG BackTraceHash)
{
	typedef USHORT(WINAPI *CaptureStackBackTraceType)(ULONG, ULONG, PVOID*, PULONG);
	static CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "RtlCaptureStackBackTrace"));

	return func(FramesToSkip, FramesToCapture, BackTrace, BackTraceHash);
}

void CPacketIntegrity::Check(const char *data, int size_in_bits)
{
	// FIRST PART: Packet integrity checks

	// Maybe this check should be done in another thread?
	// Is MD5'ing every packet slowing down sending process? or causing fps drops?

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
			// PACKET LOST!
			// Force crash?
		}

		delete p.first;
	}

	// SECOND PART: Outside calls checks

	const int kMaxCallers = 62; // 62 is maximum allowed value on Windows XP
	void* callers[kMaxCallers];
	int count = FIX_RtlCaptureStackBackTrace(0, kMaxCallers, callers, NULL); // FramesToSkip = 4 -> skip: (1) FIX_RtlCaptureStackBackTrace wrapper, (2) this function, (3) RakPeer__SendBufferedHook, (4) original RakPeer::SendBuffered

	MEMORY_BASIC_INFORMATION mbi;
	HMODULE mod;
	char buffer[512];

	unsigned curCall = callCounter++;

	for (int i = 0; i < count; i++)
	{
		mod = 0;
		ZeroMemory(buffer, sizeof(buffer));

		if (VirtualQuery(callers[i], &mbi, sizeof(mbi)))
		{
			mod = (HMODULE)mbi.AllocationBase;
			GetModuleFileNameA(mod, buffer, sizeof(buffer));
		}
		if ((DWORD)mod == getSampBaseAddress())
		{
			strcpy(buffer, "samp.dll");
		}

		CLog("salut").Write("[%d] %d called from %x, modbase: %x, modname: %s", curCall, i, (unsigned)callers[i], mod, buffer);
	}
}