#include "ACVerifiedPacket.h"

namespace ACVerifiedPacket
{
	void Verify(RakNet::BitStream& bs, unsigned char *preparedData, unsigned char& preparedDataSize)
	{
		unsigned char nBytes;
		unsigned char keyByte;
		
		for (int i = 0; i < MIN_ARRAY_SIZE; ++i)
			bs.Read(preparedData[i]);
		
		bs.Read(keyByte);
		bs.Read(nBytes);
		
		for (int i = MIN_ARRAY_SIZE; i < nBytes; ++i)
			bs.Read(preparedData[i]);

		for (int i = 0; i < nBytes; ++i)
			preparedData[i] ^= preparedData[(keyByte + preparedData[(i + nBytes / 2) % nBytes]) % nBytes];

		preparedDataSize = nBytes;
	}
}
