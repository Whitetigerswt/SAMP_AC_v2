#include "CDirectoryScanner.h"
#include "CLog.h"
#include "IMG.h"
#include <md5.h>
#include <BitStream.h>
#include "Network\Network.h"
#include "../Shared/Network/CRPC.h"
#include "Misc.h"
#include "../Shared/MD5_Info/Cmd5Info.h"

#include <Boost\filesystem.hpp>

CDirectoryScanner::CDirectoryScanner()
{

}


CDirectoryScanner::~CDirectoryScanner()
{

}

void CDirectoryScanner::Scan(std::string path)
{

	// Create a new MD5 object, cause we need to calculate all md5's of the files
	MD5 md5obj = MD5();

	// Loop through all folders and files recursively.
	for (boost::filesystem::recursive_directory_iterator end, dir(path); dir != end; ++dir) 
	{
		// Turn it into a path object, because it's easier to deal with.
		boost::filesystem::path pFile = (*dir).path();
		
		// We can't calculate the MD5 of a directory...
		if (boost::filesystem::is_directory(pFile) || !pFile.has_extension())
		{
			continue;
		}
		else if (!pFile.filename().generic_string().compare("gta3.img") == 0)
		{
			// It's a file we can calculate the MD5 of.
			std::string md5(MD5_Specific_File(pFile.generic_string()));

			// We have an md5 of the file, now let's insert everything we know into our map.
			g_mFiles.insert(std::pair<std::string, std::string>(pFile.generic_string(), md5));

			RakNet::BitStream bitStream;

			// Remove main GTA SA path so we only pass a relative one to the server, simplifying server results and saving bandwidth.
			bitStream.Write(pFile.generic_string().substr(Misc::GetGTADirectory().length() + 1).c_str());
			bitStream.Write(md5.c_str());

			Network::SendRPC(ON_FILE_CALCULATED, &bitStream);
		}
	}
	// todo change "models/gta3.img" to that address that litteraly reads models/gta3.img in gta_sa.exe memory

	path.append("\\models\\gta3.img");
	img_scan(path);
}

std::string CDirectoryScanner::MD5_Specific_File(std::string path)
{
	MD5 md5obj = MD5();
	char* CStyleMD5 = md5obj.digestFile((char*)path.c_str());
	std::string md5(CStyleMD5);

	return md5;
}

void CDirectoryScanner::img_scan(std::string path_to_gta3_img)
{
	// Open IMG archive file.
	IMG img = IMG();
	img.OpenArchive(path_to_gta3_img.c_str());

	// Make sure it was opened successfully.
	if (img.IsArchiveOpened())
	{
		// Create vars to hold data about each IMG entry.
		char* filecontents = NULL;

		CLog log = CLog("gta3.img.txt");
		
		// Loop through all IMG entrys.
		for (auto& entry = img.begin(); entry != img.end(); ++entry)
		{

			// Get the file name
			const char* filename = (*entry).GetFilename();

			// allocate enough memory to hold all of the file contents in the entry.
			filecontents = (char*)malloc(sizeof(char)*(*entry).GetFilesize());

			// Get all of the file contents
			(*entry).ReadEntireFile((void*)filecontents);

			// MD5 each entry
			MD5 md5obj = MD5();
			std::string md5 = md5obj.digestMemory((BYTE*)filecontents, (*entry).GetFilesize());

			g_mGta3ImgDefaults = Cmd5Info::GetIMGMD5s();

			if (g_mGta3ImgDefaults[filename].empty()) 
			{
				continue;
			}

			if (md5.compare(g_mGta3ImgDefaults[filename]) != 0)
			{
				RakNet::BitStream bitStream;

				bitStream.Write(filename);
				bitStream.Write(md5.c_str());

				Network::SendRPC(ON_IMG_FILE_MODIFIED, &bitStream);
			}

			free(filecontents);
		}
	}
}
