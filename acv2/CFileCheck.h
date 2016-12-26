#pragma once

#include <vector>
#include <map>
#include <Windows.h>

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
	void AddFile(std::wstring);

	// PURPOSE: Add a file to the list of files from the last scan.
	// REQUIRES: a valid file (exists)
	// REQUIRES: the file's MD5.
	// PROMISE: The file will be added to the internal file list.
	void AddFile(std::wstring, std::string);

	// PURPOSE: Check if a file is already added to the file list. Does not check if the file actually exists on the hard drive.
	// REQUIRES: A file to check
	bool DoesFileExist(std::wstring);

	// PURPOSE: Send the file md5's and paths back to the server (after a disconnect or initial connect)
	// REQUIRES: A non-empty file list
	// PROMISE: File list sent to the server.
	void ResendFiles();

protected:
	// PURPOSE: Check if a file exists.
	// REQUIRES: a string (file path)
	bool FileExists(std::wstring file);

	// PURPOSE: Get a file's MD5
	// REQUIRES: A file path that exists.
	BYTE* GetFileMD5(std::wstring file);

private:
	// PURPOSE: Files by MD5 in an std::vector. This is a list of files we've already checked.
	std::vector<std::string> m_MD5List;

	// PURPOSE: Files by file path in an std::vector.
	std::vector<std::wstring> m_FilePaths;

	// PURPOSE: Sends the RPC to the server letting it know we've run a new file.
	// REQUIRES: A valid file and MD5.
	void OnFileExecuted(const wchar_t* file, BYTE* md5);

	std::string GetFileMD5Chars(std::wstring file);
};

