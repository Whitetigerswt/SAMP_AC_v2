#include "CDirectoryScanner.h"
#include <boost\algorithm\string.hpp>
#include "IMG.h"
#include "../Shared/MD5_Info/md5.h"
#include "../Shared/Network/BitStream.h"
#include "Network\Network.h"
#include "../Shared/Network/CRPC.h"
#include "Misc.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "Addresses.h"
#include "Network\CRakClientHandler.h"

#include <boost\filesystem.hpp>
#include <tchar.h>

CDirectoryScanner::CDirectoryScanner()
{

}


CDirectoryScanner::~CDirectoryScanner()
{

}

void CDirectoryScanner::Scan(std::wstring path)
{
	path.append(TEXT("\\"));
	wchar_t* gta3img = new wchar_t[16];
	memcpy(gta3img, VAR_GTA3_IMG_PATH, 15);
	path.append(gta3img);
	delete[] gta3img;

	img_scan(path);
}

std::string CDirectoryScanner::MD5_Specific_File(std::wstring path)
{

	// Create an MD5 object so we can calculate MD5's
	MD5 md5obj = MD5();

	// Get the GTA directory and store it in a string.
	std::wstring gtadir = Misc::GetGTADirectory();

	// Replace the $(GtaDirectory) macro with the actual GTA directory.
	boost::replace_first(path, "$(GtaDirectory)", gtadir);
	
	char* CStyleMD5 = "";

	// Make sure the file exists to avoid a crash if it doesn't
	if (boost::filesystem::exists(path))
	{
		// Calculate the MD5 of the file and store it in a C-style char value.
		CStyleMD5 = md5obj.digestFileChar(path.c_str());
	}

	// Convert the result to an std::string
	std::string md5(CStyleMD5);

	// And return it.
	return md5;
}

void CDirectoryScanner::img_scan(std::wstring path_to_gta3_img)
{
	// Open IMG archive file.
	IMG img = IMG();
	img.OpenArchive(path_to_gta3_img.c_str());

	// Make sure it was opened successfully.
	if (img.IsArchiveOpened())
	{
		// Create vars to hold data about each IMG entry.
		wchar_t* filecontents = NULL;

		std::map<std::string, std::string> Gta3ImgDefaults = Cmd5Info::GetIMGMD5s();

		// Create a new MD5 object.
		MD5 md5obj = MD5();
		
		// Loop through all IMG entrys.
		for (auto& entry = img.begin(); entry != img.end(); ++entry)
		{
			// Get the file name
			const wchar_t* filename = (*entry).GetFilename();

			// Make sure the filename has an entry in the files we're checking
			if (Gta3ImgDefaults[Misc::utf8_encode(filename).c_str()].empty())
			{
				continue;
			}

			// allocate enough memory to hold all of the file contents in the entry.
			filecontents = (wchar_t*)malloc(sizeof(wchar_t)*(*entry).GetFilesize());

			// Get all of the file contents
			(*entry).ReadEntireFile((void*)filecontents);

			// MD5 each entry
			std::string md5 = md5obj.digestMemory((BYTE*)filecontents, (*entry).GetFilesize());

			// Compare the md5 calculated to the list of files we got from the "Internet"
			if (md5.compare(Gta3ImgDefaults[Misc::utf8_encode(filename).c_str()]) != 0)
			{

				// Tell the server that the MD5 doesn't match on this file.
				RakNet::BitStream bitStream;

				// Add header info
				bitStream.Write((unsigned char)PACKET_RPC);
				bitStream.Write(ON_IMG_FILE_MODIFIED);

				// Add strings
				bitStream.Write((unsigned short)_tcslen(filename));
				bitStream.Write(Misc::utf8_encode(filename).c_str(), _tcslen(filename));

				// convert md5 string to bytes

				// if string isn't null
				if (strcmp(md5.c_str(), "NULL"))
				{
					for (int i = 0; i < 16; ++i)
					{
						bitStream.Write(md5obj.digestRaw[i]);
					}
				}
				else
				{
					for (int i = 0; i < 16; ++i)
					{
						bitStream.Write(NULL);
					}
				}

				// Send
				CRakClientHandler::CustomSend(&bitStream, LOW_PRIORITY, RELIABLE);
			}

			// Free memory!
			free(filecontents);
		}
	}
}