#include "Cmd5Info.h"

std::map<std::string, std::string> Cmd5Info::g_mGta3ImgDefaults;

std::map<std::string, std::string> Cmd5Info::GetIMGMD5s()
{

	// SKIN ID 1
	// -------------------------------

	// truth.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("truth.txd", "56e1dbefce02c58f3c6624d7134bd2cf"));

	// truth.dff - 96256 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("truth.dff", "a3daf22fb87d0633cfd006b89ff1c535"));

	// SKIN ID 2
	// -------------------------------

	// maccer.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("maccer.txd", "c98e261669d30d008d741f4e86480b9c"));

	// maccer.dff - 88064 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("maccer.dff", "85e889e76d509b31b4007d062f7300f5"));

	// SKIN ID 3
	// -------------------------------

	// andre.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("andre.txd", "f73bc54a2c1be18d4305f14f1bd09f76"));

	// andre.dff - 98304 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("andre.dff", "0449f8cb8a38942ce2ec75c3ee4b8c22"));

	// SKIN ID 4
	// -------------------------------

	// bbthin.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bbthin.txd", "dd5eeb513eda83015e51258f5110c1d8"));

	// bbthin.dff - 88064 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bbthin.dff", "afe82f991bd88fa5bd97440f021b45bc"));

	// SKIN ID 5
	// -------------------------------

	// bb.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bb.txd", "2ac7cc1f0b8828bf3ac09f57c373ecde"));

	// bb.dff - 83968 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bb.dff", "97cecf43839339b70442c91d99227812"));

	// SKIN ID 6
	// -------------------------------

	// emmet.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("emmet.txd", "335a488b7c8b2f4047edc7fc238f282e"));

	// emmet.dff - 100352 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("emmet.dff", "58535c884d8f0dfcb1ac2d79512363ec"));

	// SKIN ID 7
	// -------------------------------

	// male01.txd - 18432 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("male01.txd", "94eed59773fa5d383a93621443d765e6"));

	// male01.dff - 92160 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("male01.dff", "359e4219b9a949b61e73eb4429eeb2ee"));

	// SKIN ID 8
	// -------------------------------

	// janitor.txd - 264192 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("janitor.txd", "ea491aad857ef6ae818be75e4d5ade60"));

	// janitor.dff - 86016 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("janitor.dff", "0a59f22f1b8533c7852b32f4a3519daf"));

	// SKIN ID 9
	// -------------------------------

	// bfori.txd - 133120 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bfori.txd", "f0dc55329618eb4c1045927ed001fdc9"));

	// bfori.dff - 81920 bytes
	g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>("bfori.dff", "a2933300f9929d5d0cdb905ab212e0cd"));

	return g_mGta3ImgDefaults;
}