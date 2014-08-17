#pragma once

#include "../../Shared/RakNet/RakNetTypes.h"

enum eClientConnectionState
{
	AWAITING,
	CONNECTED
};

class CClientSocketInfo
{
public:

	// PURPOSE: Initialize The client socket info
	// REQUIRES: A Raknet system address.
	// REQUIRES: A raknet GUID.
	CClientSocketInfo(RakNet::SystemAddress systemAddress, RakNet::RakNetGUID guid);

	virtual inline ~CClientSocketInfo() {};

	// PURPOSE: Change the connection state of the server.
	// REQUIRES: a new state to set.
	inline void SetState(eClientConnectionState state) { m_state = state; };

	// PURPOSE: Get the current RakNet system address
	// REQUIRES: NULL
	inline RakNet::SystemAddress& GetSystemAddress() { return m_systemAddress; };

	// PURPOSE: Get the current RakNet GUID.
	// REQUIRES: NULL
	inline RakNet::RakNetGUID& GetGUID() { return m_guid; };

	// PURPOSE: Get the current connection state.
	// REQUIRES: NULL
	inline eClientConnectionState GetState() { return m_state; };

private:
	RakNet::SystemAddress m_systemAddress;
	RakNet::RakNetGUID m_guid;
	eClientConnectionState m_state;

};