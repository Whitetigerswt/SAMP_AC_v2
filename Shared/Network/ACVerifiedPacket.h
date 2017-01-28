#pragma once

#include "BitStream.h"

namespace ACVerifiedPacket
{
	const unsigned char MIN_ARRAY_SIZE = 8;
	const unsigned char MAX_ARRAY_SIZE = 16;

	// PURPOSE: Takes a bitstream and apply verified packet algorithm to it.
	void Verify(RakNet::BitStream& bs, unsigned char *preparedData, unsigned char& preparedDataSize);
}
