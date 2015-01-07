#pragma once

class CRakClientHandler
{
public:
	CRakClientHandler();
	void CustomRPC(RakNet::BitStream *bs);
	void Send();

private:
};