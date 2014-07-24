
#include <map>
#include <vector>
class Cmd5Info
{
public:
	static std::map<std::string, std::string> GetIMGMD5s();

private:
	static std::map<std::string, std::string> g_mGta3ImgDefaults;
};