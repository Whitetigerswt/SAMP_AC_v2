#pragma once
#include <map>
#include <vector>
#include <string>

#define AC_IMG_MD5_INFO "http://whitetigerswt.github.io/SAMP_AC_v2/API/gta3img.html"
#define AC_EXECUTABLE_MD5_INFO "http://whitetigerswt.github.io/SAMP_AC_v2/API/badexecutablefiles.html"
#define AC_DIR_MD5_INFO "http://whitetigerswt.github.io/SAMP_AC_v2/API/gtamd5dir.html"
#define AC_DIR_NAME_INFO "http://whitetigerswt.github.io/SAMP_AC_v2/API/gtanamedir.html"

class Cmd5Info
{
public:
	static std::map<std::string, std::string> GetIMGMD5s();
	static std::vector<std::string> GetBadExecutableFiles();
	static std::string GetWebsiteText(std::string url);
	static std::vector<std::string> GetGtaDirectoryFilesMd5();
	static std::vector<std::string> GetGtaDirectoryFilesNames();

private:

};
