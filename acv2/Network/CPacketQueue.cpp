#include <boost\thread.hpp>
#include <queue>

#include "Network\CRakClientHandler.h"
#include "CPacketQueue.h"

std::queue<CPacketQueue::stPacketInfo> CPacketQueue::m_Queue;
boost::shared_mutex CPacketQueue::m_Access;

void CPacketQueue::Add(RakNet::BitStream *bs, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	boost::unique_lock<boost::shared_mutex> lock(m_Access);
	
	stPacketInfo packet;
	packet.bs = new BitStream;
	packet.bs->Write(bs);
	packet.priority = priority;
	packet.reliability = reliability;
	packet.orderingChannel = orderingChannel;

	m_Queue.push(packet);
}

void CPacketQueue::Process()
{
	if (!CRakClientHandler::IsConnected()) return;

	boost::unique_lock<boost::shared_mutex> lock(m_Access);
	while (!m_Queue.empty())
	{
		stPacketInfo packet = m_Queue.front();
		m_Queue.pop();

		CRakClientHandler::CustomSend(packet.bs, packet.priority, packet.reliability, packet.orderingChannel);
		delete packet.bs;
	}
}