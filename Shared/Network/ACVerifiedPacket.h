#pragma once

namespace ACVerifiedPacket
{
	const unsigned short int MAX_ARRAY_SIZE = 4;
	
	// PURPOSE: Takes a byte and apply verified packet algorithm to it.
	void Verify(unsigned char *byt);
}
