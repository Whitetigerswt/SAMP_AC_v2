#pragma once
#include <string>
#include <map>
#include <vector>

class CDirectoryScanner
{
public:
	CDirectoryScanner();
	~CDirectoryScanner();

	// PURPOSE: Scan the GTA SA directory for md5 mismatches.
	// REQUIRES: The current gta sa path directory.
	void Scan(std::wstring path);

	// PURPOSE: MD5 a specific file.
	// REQUIRES: The current gta sa path directory.
	std::string MD5_Specific_File(std::wstring path);
private:

	// PURPOSE: Scan the files inside gta3.img.
	// REQUIRES: The path to the gta3.img.
	void img_scan(std::wstring path_to_gta3_img);
};


