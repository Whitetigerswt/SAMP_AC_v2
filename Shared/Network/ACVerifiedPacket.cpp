#include "ACVerifiedPacket.h"
#include "../MD5_Info/Cmd5Info.h"
#include "../../acv2/md5.h"

namespace ACVerifiedPacket
{
	std::string RawVerifiedPacket()
	{
		/* 
			Request something to MD5. That thing changes and is never the same which
			is supposed to confuse crackers for some time.

			Plus the URL has a misleading name.
		*/
		std::string response_data = Cmd5Info::GetWebsiteText("http://www.infinite-gaming.ml/khk/ac/unreleased_banlist_blacklist.php");


		// Append more data to make it even more harder to crack
		response_data.append("9f3973ed275e6c49b3f13a1c0968957f");

		// MD5 everything
		MD5 md5 = MD5();
		std::string md5ed_data = std::string(md5.digestString((char*)response_data.c_str()));

		return md5ed_data;
	}
}