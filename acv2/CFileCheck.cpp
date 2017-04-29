#include "CFileCheck.h"
#include "../Shared/MD5_Info/md5.h"
#include "../Shared/Network/CRPC.h"
#include "Network\Network.h"
#include "Network\CRakClientHandler.h"
#include "Misc.h"

#include <Windows.h>
#include <tchar.h>
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

BYTE* CFileCheck::GetFileMD5(std::wstring file)
{
	// Create an MD5 object to calculate the MD5 of the file path
	if (FileExists(file))
	{
		// Create a new MD5 object.
		MD5 objmd5 = MD5();

		BYTE* md5 = new BYTE[16];

		// Calculate the file path.
		objmd5.digestFile(file.c_str());

		for (int i = 0; i < 16; ++i)
		{
			md5[i] = objmd5.digestRaw[i];
		}

		return md5;
	}
	return NULL;
}

std::string CFileCheck::GetFileMD5Chars(std::wstring file)
{
	// Create an MD5 object to calculate the MD5 of the file path
	if (FileExists(file))
	{
		// Create a new MD5 object.
		MD5 objmd5 = MD5();

		// Calculate the file path.
		std::string md5 = objmd5.digestFileChar(file.c_str());

		return md5;
	}
	return "NULL";
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
	AddFile(file, GetFileMD5Chars(file));
}

void CFileCheck::AddFile(std::wstring file, std::string md5)
{
	if (!file.empty() && !md5.empty())
	{
		// Add the file to our list of executed files.
		m_FilePaths.push_back(file);

		// Add the MD5 of the file to our list of MD5 hashes we've already checked.
		m_MD5List.push_back(md5);

		// Convert wstring to byte array.
		
		BYTE digest[16];
		if (strcmp(md5.c_str(), "NULL"))
		{
			for (int i = 0; i < 16; ++i)
			{
				std::string bt = md5.substr(i * 2, 2);
				digest[i] = static_cast<BYTE>(strtoul(bt.c_str(), NULL, 16));
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				digest[i] = NULL;
			}
		}

		// Send the info to the server.
		OnFileExecuted(file.c_str(), digest);
	}
}

void CFileCheck::OnFileExecuted(const wchar_t* file, BYTE* md5)
{
	// Make sure the length is greater than 3 characters.
	if (_tcslen(file) > 3)
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
		bitStream.Write(Misc::utf8_encode(szFile).c_str(), szFile.length());


		if (md5 == NULL)
		{
			for (int i = 0; i < 16; ++i)
			{
				bitStream.Write(NULL);
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				bitStream.Write(md5[i]);
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
		// TODO: Why are we calculating the file's MD5 again when its held in m_MD5List?
		BYTE* md5 = GetFileMD5(m_FilePaths.at(i).c_str());
		OnFileExecuted(m_FilePaths.at(i).c_str(), md5);
		delete[] md5;

		// Give the cpu a break sometimes...
		Sleep(150);
	}
}

bool CFileCheck::DoesFileExist(std::wstring file)
{
	// Get the MD5 hash of this file
	std::string md5 = GetFileMD5Chars(file);

	// Search the MD5 list to check if this MD5 hash exists.
	for (std::vector<std::string>::iterator i = m_MD5List.begin(); i != m_MD5List.end(); ++i)
	{
		// Compare the MD5 of the file we calculated, to an index in our list of MD5s.
		if ((*i).compare(md5) == 0)
		{
			return true;
		}
	}
	return false;
}