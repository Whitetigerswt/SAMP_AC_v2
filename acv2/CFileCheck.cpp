#include "CFileCheck.h"
#include "md5.h"
#include "../Shared/Network/CRPC.h"
#include "Network\Network.h"
#include "Network\CRakClientHandler.h"

#include <Windows.h>
#include <string>

CFileCheck::CFileCheck()
{
	// Initialize with clear lists.
	m_FilePaths.clear();
	m_MD5List.clear();
}


CFileCheck::~CFileCheck()
{
	// Clean up
	m_FilePaths.clear();
	m_MD5List.clear();
}

std::wstring CFileCheck::GetFileMD5(std::wstring file)
{
	// Create an MD5 object to calculate the MD5 of the file path
	if (FileExists(file))
	{
		// Create a new MD5 object.
		MD5 objmd5 = MD5();

		// Calculate the file path.
		std::wstring md5 = objmd5.digestFile((wchar_t*)file.c_str());

		return md5;
	}
	return TEXT("NULL");
}

bool CFileCheck::FileExists(std::wstring name)
{
	// Open the file
	if (FILE *file = _tfopen(name.c_str(), TEXT("r")))
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

void CFileCheck::AddFile(std::wstring file)
{
	AddFile(file, GetFileMD5(file));
}

void CFileCheck::AddFile(std::wstring file, std::wstring md5)
{
	if (!file.empty() && !md5.empty())
	{
		// Add the file to our list of executed files.
		m_FilePaths.push_back(file);

		// Add the MD5 of the file to our list of MD5 hashes we've already checked.
		m_MD5List.push_back(md5);

		// Send the info to the server.
		OnFileExecuted(file.c_str(), md5.c_str());
	}
}

void CFileCheck::OnFileExecuted(const wchar_t* file, const wchar_t* md5)
{
	// Make sure the length is greater than 0 characters.
	if (_tcslen(file) > 0)
	{
		// Convert it to an std::string.
		std::wstring szFile(file);

		// Find the last instance of a \, cause we only want the file name and not the complete path.
		int i = szFile.rfind(TEXT("\\"));

		// Change the string to only the files name and not it's complete path.
		szFile = szFile.substr(i + 1);

		// Prepare to send the info to the server.
		RakNet::BitStream bitStream;

		// Add header info
		bitStream.Write((unsigned char)PACKET_RPC);
		bitStream.Write(ON_FILE_EXECUTED);

		bitStream.Write((unsigned short)szFile.length());
		bitStream.Write((const char*)szFile.c_str(), szFile.length());

		// convert md5 string to bytes
		BYTE digest[16];
		std::wstring md5_string(md5);

		// if string isn't null
		if (_tcscmp(md5, TEXT("NULL")))
		{
			for (int i = 0; i < 16; ++i)
			{
				std::wstring bt = md5_string.substr(i * 2, 2);
				digest[i] = static_cast<BYTE>(_tcstoul(bt.c_str(), NULL, 16));
				bitStream.Write(digest[i]);
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				digest[i] = NULL;
				bitStream.Write(digest[i]);
			}
		}

		// Send the RPC to the server.
		CRakClientHandler::CustomSend(&bitStream, LOW_PRIORITY, RELIABLE);
	}
	return;
}

void CFileCheck::ResendFiles()
{
	// Loop through the file collection
	for (unsigned int i = 0; i < m_FilePaths.size(); i++)
	{
		// re-send the packets to the server
		OnFileExecuted(m_FilePaths.at(i).c_str(), GetFileMD5(m_FilePaths.at(i).c_str()).c_str());

		// Give the cpu a break sometimes...
		Sleep(150);
	}
}

bool CFileCheck::DoesFileExist(std::wstring file)
{
	// Get the MD5 hash of this file
	std::wstring md5 = GetFileMD5(file);

	// Search the MD5 list to check if this MD5 hash exists.
	for (std::vector<std::wstring>::iterator i = m_MD5List.begin(); i != m_MD5List.end(); ++i)
	{
		// Compare the MD5 of the file we calculated, to an index in our list of MD5s.
		if ((*i).compare(md5) == 0)
		{
			return true;
		}
	}

	return false;
}
