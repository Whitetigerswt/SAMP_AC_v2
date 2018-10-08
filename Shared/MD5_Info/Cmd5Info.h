#include <map>
#include <vector>
#include <string>

#define AC_IMG_MD5_INFO "https://whitetigerswt.github.io/SAMP_AC_v2/API/gta3img.html"
#define AC_EXECUTABLE_MD5_INFO "https://whitetigerswt.github.io/SAMP_AC_v2/API/badexecutablefiles.html"
#define AC_DIR_MD5_INFO "https://whitetigerswt.github.io/SAMP_AC_v2/API/gtamd5dir.html"
#define AC_DIR_NAME_INFO "https://whitetigerswt.github.io/SAMP_AC_v2/API/gtanamedir.html"

class Cmd5Info
{
public:
	// PURPOSE: Get the list of allowed IMG md5s
	// REQUIRES: NULL
	static std::map<std::string, std::string> GetIMGMD5s();

	// PURPOSE: Get a list of bad md5 .exe, or modules.
	// REQUIRES: NULL
	static std::vector<std::string> GetBadExecutableFiles();

	// PURPOSE: Get the text of a website.
	// REQUIRES: A URL to check.
	static std::string GetWebsiteText(std::string url);

	// PURPOSE: Download a file onto the machine.
	// REQUIRES: A URL to download from.
	// REQUIRES: A filename to place it into.
#ifdef WIN32
	static std::string DownloadFile(std::string url, const std::wstring fname);
#else
	static std::string DownloadFile(std::string url, const std::string fname);
#endif

	// PURPOSE: Get a list of bad gta directory files in md5s.
	// REQUIRES: NULL
	static std::vector<std::string> GetGtaDirectoryFilesMd5();

	// PURPOSE: Get a list of bad gta directory files in file names.
	// REQUIRES: NULL
	static std::vector<std::string> GetGtaDirectoryFilesNames();

private:

};
