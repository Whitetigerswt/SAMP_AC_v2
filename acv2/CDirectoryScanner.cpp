#include "CDirectoryScanner.h"
#include "CLog.h"
#include "IMG.h"

#include <Boost\filesystem.hpp>
#include <md5.h>

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
		if (boost::filesystem::is_directory(pFile))
		{
			continue;
		}
		else if (boost::filesystem::file_size(pFile) < 3000000)
		{
			// It's a file we can calculate the MD5 of.
			char* CStyleMD5 = md5obj.digestFile((char*)pFile.generic_string().c_str());
			std::string md5(CStyleMD5);

			// We have an md5 of the file, now let's insert everything we know into our map.
			g_mFiles.insert(std::pair<std::string, std::string>(pFile.filename().generic_string(), md5));
		}
	}
	// todo change "models/gta3.img" to that address that litteraly reads models/gta3.img in gta_sa.exe memory

	path.append("\\models\\gta3.img");
	img_scan(path);
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
		char* filename = new char[256];
		
		// Loop through all IMG entrys.
		for (auto& entry = img.begin(); entry != img.end(); ++entry)
		{

			// Get the file name, with a max size of 256
			(*entry).GetFilenameWithoutExtension(filename, 256);

			// allocate enough memory to hold all of the file contents in the entry.
			filecontents = (char*)malloc(sizeof(char)*(*entry).GetFilesize());

			// Get all of the file contents
			(*entry).ReadEntireFile((void*)filecontents);

			// MD5 each entry
			// NOTE: maybe switch to something more efficent than md5 for this. There is roughly 2,000 files.
			MD5 md5obj = MD5();
			std::string md5 = md5obj.digestMemory((BYTE*)filecontents, (*entry).GetFilesize());

			// todo

			free(filecontents);
		}
		delete filename;
	}
}
