#pragma once

#include <vector>
#include <map>

class CFileCheck
{
public:
	CFileCheck();
	~CFileCheck();

	// PURPOSE: Add cheat MD5 to the cheat list
	// REQUIRES: a file's MD5.
	// PROMISE: Cheat is added to the cheat list.
	void Add(std::wstring);
	
	// PURPOSE: Remove a cheat from the cheat list
	// REQUIRES: A file's MD5.
	// PROMISE: The cheat is removed from the cheat list.
	void Remove(std::wstring);

	// PURPOSE: Remove a cheat from the cheat list
	// REQUIRES: An index to the cheat list vector.
	// PROMISE: The cheat is removed from the cheat list.
	void Remove(int i);

	// PURPOSE: Check if an MD5 string is in the list (cheat).
	// REQUIRES: A file's MD5.
	// PROMISE: Returns wether the file is a cheat or not.
	bool IsCheat(std::wstring);

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
	void AddFile(std::wstring, std::wstring);

	// PURPOSE: Remove a file from the file list by the full path.
	// REQUIRES: The file already being on the list of internal files.
	// PROMISE: The file will be removed from the internal list of files.
	void RemoveFile(std::wstring);

	// PURPOSE: Remove a file from the file list by index.
	// REQUIRES: An index less than the size of the internal file list.
	// PROMISE: File removed at idx
	void RemoveFile(int idx);

	// PURPOSE: Clear the entire list of files (so we can re-scan and there won't be multiple entries.)
	// REQUIRES: NULL
	// PROMISE: An empty file list.
	void ClearFileList();

	// PURPOSE: Check if a file is already added to the file list. Does not check if the file actually exists on the hard drive.
	// REQUIRES: A file to check
	bool DoesFileExist(std::wstring);

	// PURPOSE: Get a list of detected cheats from the last scans.
	// REQUIRES: NULL
	// PROMISE: A returned list of cheats.
	std::map<std::wstring, std::wstring> CFileCheck::GetCheatList();

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
	std::wstring GetFileMD5(std::wstring);

private:
	// PURPOSE: Files by MD5 in an std::vector. This is a list of files we've already checked.
	std::vector<std::wstring> m_MD5List;

	// PURPOSE: Files by file path in an std::vector.
	std::vector<std::wstring> m_FilePaths;

	// PURPOSE: Sends the RPC to the server letting it know we've run a new file.
	// REQUIRES: A valid file and MD5.
	void OnFileExecuted(const wchar_t* file, const wchar_t* md5);
};

