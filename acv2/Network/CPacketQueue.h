#pragma once

#include <queue>

class CPacketQueue
{
public:
	struct stPacketInfo {
		BitStream *bs;
		PacketPriority priority;
		PacketReliability reliability; 
		char orderingChannel;
	};

	// PURPOSE: Add the BitStream to the queue.
	static void Add(RakNet::BitStream *bs, PacketPriority priority, PacketReliability reliability, char orderingChannel);

	// PURPOSE: Send queued packets.
	static void Process();

private:
	static std::queue<stPacketInfo> m_Queue;
	static boost::shared_mutex m_Access;
};