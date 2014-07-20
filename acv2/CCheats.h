#pragma once

#include <vector>
#include <map>

class CCheats
{
public:
	CCheats();
	~CCheats();

	// PURPOSE: Add cheat MD5 to the cheat list
	// REQUIRES: a file's MD5.
	// PROMISE: Cheat is added to the cheat list.
	void Add(std::string);
	
	// PURPOSE: Remove a cheat from the cheat list
	// REQUIRES: A file's MD5.
	// PROMISE: The cheat is removed from the cheat list.
	void Remove(std::string);

	// PURPOSE: Remove a cheat from the cheat list
	// REQUIRES: An index to the cheat list vector.
	// PROMISE: The cheat is removed from the cheat list.
	void Remove(int i);

	// PURPOSE: Check if an MD5 string is in the list (cheat).
	// REQUIRES: A file's MD5.
	// PROMISE: Returns wether the file is a cheat or not.
	bool IsCheat(std::string);

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

	// PURPOSE: Remove a file from the file list by the full path.
	// REQUIRES: The file already being on the list of internal files.
	// PROMISE: The file will be removed from the internal list of files.
	void RemoveFile(std::string);

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
	bool DoesFileExist(std::string);

	// PURPOSE: Get a list of detected cheats from the last scans.
	// REQUIRES: NULL
	// PROMISE: A returned list of cheats.
	std::map<std::string, std::string> CCheats::GetCheatList();

	void ResendFiles();

	// TODO: Add RPC to send cheats

protected:
	// PURPOSE: Check if a file exists.
	// REQUIRES: a string (file path)
	bool FileExists(std::string file);

	// PURPOSE: Get a file's MD5
	// REQUIRES: A file path that exists.
	std::string GetFileMD5(std::string);

private:
	// PURPOSE: Cheats by MD5 in an std::vector.
	std::vector<std::string> m_CheatList;

	// PURPOSE: Cheats by file path in an std::vector.
	std::vector<std::string> m_FilePaths;

	// PURPOSE: Sends the RPC to the server letting it know we've run a new file.
	// REQUIRES: A valid file and MD5.
	void OnFileExecuted(const char* file, const char* md5);
};

