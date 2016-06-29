#pragma once

#include <string>

namespace ACVerifiedPacket
{
	// PURPOSE: Compute verified AC packet.
	// NOTES: Returned value varies, can never have the same value twice.
	std::string RawVerifiedPacket();
}