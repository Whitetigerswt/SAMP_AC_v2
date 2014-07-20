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
	for (std::vector<std::string>::iterator i = m_CheatList.begin(); i != m_CheatList.end(); ++i)
	{
		if ((*i).compare(md5) == 0)
		{
			int distance = std::distance(m_CheatList.begin(), i);
			Remove(distance);
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
		for (std::vector<std::string>::iterator i = m_CheatList.begin(); i != m_CheatList.end(); ++i)
		{
			// Compare the MD5 of the file we calculated, to an index in our cheat list of MD5s.
			if ((*i).compare(md5) == 0)
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

	OnFileExecuted(file.c_str(), GetFileMD5(file.c_str()).c_str());
}

void CCheats::AddFile(std::string file, std::string md5)
{
	m_FilePaths.push_back(file);

	OnFileExecuted(file.c_str(), md5.c_str());
}

void CCheats::OnFileExecuted(const char* file, const char* md5)
{
	if (strlen(file) > 3) 
	{
		std::string szFile(file);
		int i = szFile.rfind("\\");
		szFile = szFile.substr(i + 1, std::string::npos);

		RakNet::BitStream bitStream;
		bitStream.Write(szFile.c_str());
		bitStream.Write(md5);

		Network::SendRPC(ON_FILE_EXECUTED, &bitStream);
	}
}

void CCheats::ResendFiles()
{
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		std::string file(*i);
		OnFileExecuted(file.c_str(), GetFileMD5(file).c_str());
		Sleep(100);
	}
}

void CCheats::RemoveFile(std::string file)
{
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		if ((*i).compare(file) == 0)
		{
			int distance = std::distance(m_FilePaths.begin(), i);
			RemoveFile(distance);
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

	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		// Get the file's MD5
		std::string md5 = GetFileMD5((*i));

		// make sure the MD5 returned isn't empty.
		if (!md5.empty())
		{
			// Compare the file's MD5 to our cheat list.
			if (IsCheat(md5))
			{
				// Add the md5 as the key.
				// Add the process path as the value.
				result.insert(std::pair<std::string, std::string>(md5, (*i)));
			}
		}
	}

	return result;
}

bool CCheats::DoesFileExist(std::string file)
{
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		if ((*i).compare(file) == 0)
		{
			return true;
		}
	}
	return false;
}