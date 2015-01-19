#include "CCheats.h"
#include "md5.h"
#include "../Shared/Network/CRPC.h"
#include "Network\Network.h"
#include "Network\CRakClientHandler.h"

#include <Windows.h>
#include <string>

CCheats::CCheats()
{
	// Initialize with a clear list.
	m_CheatList.clear();
}


CCheats::~CCheats()
{
	// Clean up
	m_CheatList.clear();
	ClearFileList();
}

void CCheats::Add(std::string md5)
{
	// Add the new file to the vector.
	m_CheatList.push_back(md5);
}

void CCheats::Remove(std::string md5)
{
	// Loop through the list of bad md5's
	for (std::vector<std::string>::iterator i = m_CheatList.begin(); i != m_CheatList.end(); ++i)
	{
		// Compare the loop index to the md5 passed in the parameter.
		if ((*i).compare(md5) == 0)
		{
			// Get the distance between the begining of the vector and the index we're at now.
			int distance = std::distance(m_CheatList.begin(), i);

			// Remove it from the list.
			Remove(distance);
		}
	}
}

void CCheats::Remove(int idx)
{
	// Remove the index from the list of bad md5's
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
		// Create a new MD5 object.
		MD5 objmd5 = MD5();

		// Calculate the file path.
		std::string md5 = objmd5.digestFile((char*)file.c_str());

		return md5;
	}
	return "NULL";
}

bool CCheats::FileExists(std::string name)
{
	// Open the file
	if (FILE *file = fopen(name.c_str(), "r")) 
	{
		// It was successful at opening the file, now close it.
		fclose(file);

		// And return true
		return true;
	}
	else 
	{
		// We were not successful at opening the file, it must not exist.
		return false;
	}
}

void CCheats::AddFile(std::string file)
{
	if (!file.empty())
	{
		// Add the file to our list of executed files.
		m_FilePaths.push_back(file);

		// Send the info to the server.
		OnFileExecuted(file.c_str(), GetFileMD5(file.c_str()).c_str());
	}
}

void CCheats::AddFile(std::string file, std::string md5)
{
	if (!file.empty() && !md5.empty())
	{
		// Add the file to our list of executed files.
		m_FilePaths.push_back(file);

		// Send the info to the server.
		OnFileExecuted(file.c_str(), md5.c_str());
	}
}

void CCheats::OnFileExecuted(const char* file, const char* md5)
{
	// Make sure the length is greater than 3 characters.
	if (strlen(file) > 3) 
	{
		// Convert it to an std::string.
		std::string szFile(file);

		// Find the last instance of a \, cause we only want the file name and not the complete path.
		int i = szFile.rfind("\\");

		// Change the string to only the files name and not it's complete path.
		szFile = szFile.substr(i + 1);

		// Prepare to send the info to the server.
		RakNet::BitStream bitStream;

		// Add header info
		bitStream.Write((unsigned char)PACKET_RPC);
		bitStream.Write(ON_FILE_EXECUTED);

		bitStream.Write((unsigned short)szFile.length());
		bitStream.Write((const char*)szFile.c_str(), szFile.length());
		bitStream.Write((unsigned short)strlen(md5));
		bitStream.Write((const char*)md5, strlen(md5));

		// Send the RPC to the server.
		CRakClientHandler::CustomSend(&bitStream, MEDIUM_PRIORITY);
	}
	return;
}

void CCheats::ResendFiles()
{
	// Loop through the file collection
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		if (i->empty()) continue;

		// Don't deal with annoying pointers.
		std::string file = "";
		file = i->c_str();

		// Re-send the info to the server that the file was just executed.
		OnFileExecuted(file.c_str(), GetFileMD5(file).c_str());

		// Sleep
		Sleep(150);
	}
}

void CCheats::RemoveFile(std::string file)
{
	// Loop through the list of files.
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		// Check if the file name matches the file passed in as a string.
		if ((*i).compare(file) == 0)
		{
			// Get the distance between the begining of the vector and where we are at now.
			int distance = std::distance(m_FilePaths.begin(), i);

			// Remove that index.
			RemoveFile(distance);
		}
	}
}

void CCheats::RemoveFile(int idx)
{
	// Erase the index from the vector.
	m_FilePaths.erase(m_FilePaths.begin() + idx);
}


void CCheats::ClearFileList()
{
	// Reset the files.
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
	// Loop through the list of files.
	for (std::vector<std::string>::iterator i = m_FilePaths.begin(); i != m_FilePaths.end(); ++i)
	{
		// Check if the file in the loop matches the one sent in as a parameter.
		if ((*i).compare(file) == 0)
		{
			// Return true if it does.
			return true;
		}
	}
	return false;
}