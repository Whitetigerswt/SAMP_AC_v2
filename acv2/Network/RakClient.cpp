/* 
 * PROJECT: mod_sa
 * COPYRIGHT: FYP
 * DATE: 21.02.2013
 * FILE: RakClient.cpp
*/

#include <Windows.h>

#include "../../Shared/Network/BitStream.h"
#include "CRakClientHandler.h"
#include "../s0beit/stdtypes.h"
#include "RakClient.h"

RakClient::RakClient( void *pRakClientInterface )
{
	pRakClient = (RakClientInterface *)pRakClientInterface;
}

bool RakClient::RPC( int rpcId, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp )
{
	if ( !pRakClient )
		return false;

	return pRakClient->RPC( &rpcId, bitStream, priority, reliability, orderingChannel, shiftTimestamp );
}

bool RakClient::Send(RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	if ( !pRakClient )
		return false;

	return pRakClient->Send( bitStream, priority, reliability, orderingChannel );
}

// misc functions
void  RakClient::SendDeath( uint16_t killerId, uint8_t reason )
{
	RakNet::BitStream bsDeath;

	bsDeath.Write( reason );
	bsDeath.Write( killerId );
	RPC( RPC_Death, &bsDeath, (PacketPriority)1, (PacketReliability)9  );
}

void RakClient::SendPickUp( int pickupId )
{
	RakNet::BitStream bsPickup;

	bsPickup.Write( pickupId );
	
	RPC( RPC_PickedUpPickup, &bsPickup );
}

void  RakClient::RequestClass( int classId )
{
	RakNet::BitStream bsClass;

	bsClass.Write( classId );
	RPC( RPC_RequestClass, &bsClass );
}

void  RakClient::SendSCMEvent( int vehicleID, int eventId, int param1, int param2 )
{
	RakNet::BitStream bsScmEvent;

	bsScmEvent.Write ( vehicleID );
	bsScmEvent.Write ( param1 );
	bsScmEvent.Write ( param2 );
	bsScmEvent.Write ( eventId );

	RPC(RPC_SCMEvent, &bsScmEvent);
}
void RakClient::SendSpawn( void )
{
	RakNet::BitStream bsSpawn;

	RPC( RPC_RequestSpawn, &bsSpawn, (PacketPriority)1, (PacketReliability)9 );
	RPC(RPC_Spawn, &bsSpawn, (PacketPriority)1, (PacketReliability)9);
}