#pragma once

#include <vector>
#include <map>

class CFileCheck
{
public:
	CFileCheck();
	~CFileCheck();

	// PURPOSE: Scan for cheats running.
	// REQUIRES: NULL
	// PROMISE: Cheats will be checked and refreshed.
	virtual void Scan() = 0;

	// PURPOSE: Add a file to the list of files from the last scan.
	// REQUIRES: a valid file (exists)
	// PROMISE: The file will be added to the internal file list.
	void AddFile(std::string);

	// PURPOSE: Add a file to the list of files from the last scan.
	// REQUIRES: a valid file (exists)
	// REQUIRES: the file's MD5.
	// PROMISE: The file will be added to the internal file list.
	void AddFile(std::string, std::string);

	// PURPOSE: Check if a file is already added to the file list. Does not check if the file actually exists on the hard drive.
	// REQUIRES: A file to check
	bool DoesFileExist(std::string);

	// PURPOSE: Send the file md5's and paths back to the server (after a disconnect or initial connect)
	// REQUIRES: A non-empty file list
	// PROMISE: File list sent to the server.
	void ResendFiles();

protected:
	// PURPOSE: Check if a file exists.
	// REQUIRES: a string (file path)
	bool FileExists(std::string file);

	// PURPOSE: Get a file's MD5
	// REQUIRES: A file path that exists.
	std::string GetFileMD5(std::string);

private:
	// PURPOSE: Files by MD5 in an std::vector. This is a list of files we've already checked.
	std::vector<std::string> m_MD5List;

	// PURPOSE: Files by file path in an std::vector.
	std::vector<std::string> m_FilePaths;

	// PURPOSE: Sends the RPC to the server letting it know we've run a new file.
	// REQUIRES: A valid file and MD5.
	void OnFileExecuted(const char* file, const char* md5);
};
