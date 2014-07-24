
#include <map>
#include <vector>
#include <string>

#define AC_IMG_MD5_INFO "http://whitetigerswt.github.io/SAMP_AC_v2"
class Cmd5Info
{
public:
	static std::map<std::string, std::string> GetIMGMD5s();

private:
	static std::map<std::string, std::string> g_mGta3ImgDefaults;
	static std::string GetWebsiteText(std::string url);
};