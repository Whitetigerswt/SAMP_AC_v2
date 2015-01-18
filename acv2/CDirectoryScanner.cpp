#include "CDirectoryScanner.h"
#include <boost/network/protocol/http/client.hpp>
#include "IMG.h"
#include <md5.h>
#include "../Shared/Network/BitStream.h"
#include "Network\Network.h"
#include "../Shared/Network/CRPC.h"
#include "Misc.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "Addresses.h"
#include "Network\CRakClientHandler.h"

#include <Boost\filesystem.hpp>

CDirectoryScanner::CDirectoryScanner()
{

}


CDirectoryScanner::~CDirectoryScanner()
{

}

void CDirectoryScanner::Scan(std::string path)
{
	// todo change "models/gta3.img" to that address that litteraly reads models/gta3.img in gta_sa.exe memory

	path.append("\\");
	char* gta3img = new char[16];
	memcpy(gta3img, VAR_GTA3_IMG_PATH, 15);
	path.append(gta3img);
	delete[] gta3img;

	img_scan(path);
}

std::string CDirectoryScanner::MD5_Specific_File(std::string path)
{
	// Create an MD5 object so we can calculate MD5's
	MD5 md5obj = MD5();

	// Get the GTA directory and store it in a string.
	std::string gtadir = Misc::GetGTADirectory();

	// Replace the $(GtaDirectory) macro with the actual GTA directory.
	boost::replace_first(path, "$(GtaDirectory)", gtadir);

	// Calculate the MD5 of the file and store it in a C-style char value.
	char* CStyleMD5 = md5obj.digestFile((char*)path.c_str());

	// Convert the result to an std::string
	std::string md5(CStyleMD5);

	// And return it.
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

		std::map<std::string, std::string> Gta3ImgDefaults = Cmd5Info::GetIMGMD5s();

		// Create a new MD5 object.
		MD5 md5obj = MD5();
		
		// Loop through all IMG entrys.
		for (auto& entry = img.begin(); entry != img.end(); ++entry)
		{
			// Get the file name
			const char* filename = (*entry).GetFilename();

			// Make sure the filename has an entry in the files we're checking
			if (Gta3ImgDefaults[filename].empty())
			{
				continue;
			}

			// allocate enough memory to hold all of the file contents in the entry.
			filecontents = (char*)malloc(sizeof(char)*(*entry).GetFilesize());

			// Get all of the file contents
			(*entry).ReadEntireFile((void*)filecontents);

			// MD5 each entry
			std::string md5 = md5obj.digestMemory((BYTE*)filecontents, (*entry).GetFilesize());

			// Compare the md5 calculated to the list of files we got from the "Internet"
			if (md5.compare(Gta3ImgDefaults[filename]) != 0)
			{
				// Tell the server that the MD5 doesn't match on this file.
				RakNet::BitStream bitStream;

				// Add header info
				bitStream.Write((unsigned char)PACKET_RPC);
				bitStream.Write(ON_IMG_FILE_MODIFIED);

				// Add strings
				bitStream.Write((unsigned short)strlen(filename));
				bitStream.Write((const char*)filename, strlen(filename));
				bitStream.Write((unsigned short)md5.length());
				bitStream.Write((const char*)md5.c_str(), md5.length());

				// Send
				CRakClientHandler::CustomSend(&bitStream);
			}

			// Free memory!
			free(filecontents);
		}
	}
}
