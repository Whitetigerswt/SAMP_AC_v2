#pragma once
#include <string>
#include <map>
#include <vector>

class CDirectoryScanner
{
public:
	CDirectoryScanner();
	~CDirectoryScanner();

	void Scan(std::string path);
	std::string MD5_Specific_File(std::string path);
private:
	void img_scan(std::string path_to_gta3_img);
};


