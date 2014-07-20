#include "CCheats.h"
#include "md5.h"
#include "CLog.h"
#include "../Shared/Network/CRPC.h"
#include "Network\Network.h"

#include <string>

CCheats::CCheats()
{
	m_CheatList.clear();
}


CCheats::~CCheats()
{
	m_CheatList.clear();
	ClearFileList();
}

void CCheats::Add(std::string md5)
{
	m_CheatList.push_back(md5);
}

void CCheats::Remove(std::string md5)
{
	for (unsigned int i = 0; i < m_CheatList.size(); ++i)
	{
		if (m_CheatList.at(i).compare(md5) == 0)
		{
			Remove(i);
		}
	}
}

void CCheats::Remove(int idx)
{
	m_CheatList.erase(m_CheatList.begin() + idx);
}

bool CCheats::IsCheat(std::string path)
{
	// Create an MD5 object to calculate the MD5 of the file path
	if (FileExists(path))
	{
		MD5 objmd5 = MD5();

		// Calculate the file path.
		std::string md5 = objmd5.digestFile((char*)path.c_str());

		// Search the cheat list to check if this file is a cheat.
		for (unsigned int i = 0; i < m_CheatList.size(); ++i)
		{
			// Compare the MD5 of the file we calculated, to an index in our cheat list of MD5s.
			if (m_CheatList.at(i).compare(md5) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

std::string CCheats::GetFileMD5(std::string file)
{
	// Create an MD5 object to calculate the MD5 of the file path
	if (FileExists(file))
	{
		MD5 objmd5 = MD5();

		// Calculate the file path.
		std::string md5 = objmd5.digestFile((char*)file.c_str());

		return md5;
	}
	return "NULL";
}

bool CCheats::FileExists(std::string name)
{
	if (FILE *file = fopen(name.c_str(), "r")) 
	{
		fclose(file);
		return true;
	}
	else 
	{
		return false;
	}
}

void CCheats::AddFile(std::string file)
{
	m_FilePaths.push_back(file);

	RakNet::BitStream bitStream;
	bitStream.Write(file.c_str());
	bitStream.Write(GetFileMD5(file).c_str());

	Network::SendRPC(ON_FILE_EXECUTED, &bitStream);
}

void CCheats::RemoveFile(std::string file)
{
	for (unsigned int i = 0; i < m_FilePaths.size(); ++i)
	{
		if (m_FilePaths.at(i).compare(file) == 0)
		{
			RemoveFile(i);
		}
	}
}

void CCheats::RemoveFile(int idx)
{
	m_FilePaths.erase(m_FilePaths.begin() + idx);
}


void CCheats::ClearFileList()
{
	m_FilePaths.clear();
}

std::map<std::string, std::string> CCheats::GetCheatList()
{
	// Create std::map as the result.
	std::map<std::string, std::string> result;

	for (unsigned int i = 0; i < m_FilePaths.size(); ++i)
	{
		// Get the file's MD5
		std::string md5 = GetFileMD5(m_FilePaths.at(i));

		// make sure the MD5 returned isn't empty.
		if (!md5.empty())
		{
			// Compare the file's MD5 to our cheat list.
			if (IsCheat(md5))
			{
				// Add the md5 as the key.
				// Add the process path as the value.
				result.insert(std::pair<std::string, std::string>(md5, m_FilePaths.at(i)));
			}
		}
	}

	return result;
}

bool CCheats::DoesFileExist(std::string file)
{
	for (unsigned int i = 0; i < m_FilePaths.size(); ++i)
	{
		if (m_FilePaths.at(i).compare(file) == 0)
		{
			return true;
		}
	}
	return false;
}