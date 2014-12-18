#include "Network.h"
#include "../../Shared/Network/CRPC.h"
#include "CRPCCallback.h"
#include "../../Shared/RakNet/MessageIdentifiers.h"
#include "../CLog.h"

#include <boost\thread.hpp>

namespace Network
{
	static CRakClient* pRakClient;
	static bool bInitialized;
	static bool bConnected;
	static bool bServerHasPlugin;
	static std::string strAddress;
	static unsigned short usPort;

	void Initialize(std::string address, unsigned short port)
	{
		bInitialized = false;

		strAddress = address;
		usPort = port;

		pRakClient = new CRakClient();
		bConnected = false;
		bServerHasPlugin = false;


		if (pRakClient->Startup() != RakNet::RAKNET_STARTED)
		{
			return;
		}

		boost::thread NetworkThread(&Network::Process);

		bInitialized = true;
	}

	bool IsInitialized()
	{
		return bInitialized;
	}

	void Reconnect()
	{
		RakNet::ConnectionAttemptResult rs;
		do
		{
			if (IsInitialized() && bServerHasPlugin)
			{
				rs = pRakClient->Connect(strAddress.c_str(), usPort, NULL);
				Sleep(5000);
			}
		} while (IsInitialized() && rs != RakNet::ALREADY_CONNECTED_TO_ENDPOINT && bServerHasPlugin);
	}

	void Connect()
	{
		pRakClient->Connect(strAddress.c_str(), usPort, NULL);
	}

	void Disconnect()
	{
		pRakClient->Shutdown(1000);
		bServerHasPlugin = false;
	}

	bool IsConnected()
	{
		return bConnected;
	}

	bool ServerHasPlugin()
	{
		return bServerHasPlugin;
	}

	void Process()
	{
		while (true)
		{
			if (!IsInitialized())
			{
				return;
			}

			RakNet::Packet* pPacket;

			while ((pPacket = pRakClient->Receive()))
			{
				if (!pPacket->length)
				{
					return;
				}

				RakNet::BitStream bitStream(&pPacket->data[1], pPacket->length - 1, false);

				switch (pPacket->data[0])
				{
				case PACKET_PLAYER_REGISTERED:
				{
					bConnected = true;
					bServerHasPlugin = true;
					CRPCCallback::Initialize();

					break;
				}
				case PACKET_RPC:
				{
					unsigned short usRpcId;

					if (bitStream.Read(usRpcId))
					{
						CRPC::Process(usRpcId, bitStream);
					}

					return;
				}
				case PACKET_CONNECTION_REJECTED:
				case PACKET_PLAYER_PROPER_DISCONNECT:
				{
					bServerHasPlugin = false;

					break;
				}
				case ID_DISCONNECTION_NOTIFICATION:
				case ID_CONNECTION_LOST:
				{
					bConnected = false;

					if (ServerHasPlugin())
						boost::thread ConnectionThread(&Reconnect);

					break;
				}
				default:
					break;

				}

				if (pPacket != NULL)
				{
					pRakClient->DeallocatePacket(pPacket);
				}
			}
			Sleep(10);
		}
		return;
	}

	unsigned int Send(Network::ePacketType packetType, RakNet::BitStream* pBitStream, PacketPriority priority, PacketReliability reliability, char cOrderingChannel)
	{
		if (!IsConnected())
			return 0;

		return pRakClient->Send(packetType, *pRakClient->GetRemoteAddress(), pBitStream, priority, reliability, cOrderingChannel);
	}

	unsigned int SendRPC(unsigned short usRPCId, RakNet::BitStream* pBitStream, PacketPriority priority, PacketReliability reliability, char cOrderingChannel)
	{
		if (!IsConnected())
			return 0;

		return pRakClient->SendRPC(usRPCId, *pRakClient->GetRemoteAddress(), pBitStream, priority, reliability, cOrderingChannel);
	}

	CRakClient* GetRakClient()
	{
		return pRakClient;
	}
}