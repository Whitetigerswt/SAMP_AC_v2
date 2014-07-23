#pragma once
#include <string>
#include <map>

class CDirectoryScanner
{
public:
	CDirectoryScanner();
	~CDirectoryScanner();

	void Scan(std::string path);
private:
	void img_scan(std::string path_to_gta3_img);
	std::map<std::string, std::string> g_mFiles;
};

