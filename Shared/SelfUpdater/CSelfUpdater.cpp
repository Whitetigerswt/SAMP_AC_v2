#ifdef WIN32
	#include <Windows.h>
	EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#else
	#include <unistd.h>
	#include <dlfcn.h>
	#include <stdio.h>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <stdlib.h>
#include "../curl/curl.h"
#include "../MD5_Info/Cmd5Info.h"
#include "../MD5_Info/md5.h"

#include "CSelfUpdater.h"

CSelfUpdater::CSelfUpdater(stVersion currVersion, std::string url)
{
	m_currentVersion = currVersion;
	m_updateInfoURL = url;

	GeneratePaths();
#ifdef WIN32
	if (boost::filesystem::exists(m_oldVerTempFilePath))
	{
		try
		{
			boost::filesystem::remove(m_oldVerTempFilePath);
		}
		catch (const boost::filesystem::filesystem_error&) {}
	}
#endif
}

CSelfUpdater::~CSelfUpdater()
{

}

// THIS FUNCTION IS UNTHREADED, SO IT WILL HANG THE THREAD FOR A WHILE!!!
// URL must return data as follows:
// major.minor.patch|update_file_url|update_file_md5|information that will be printed in console
bool CSelfUpdater::CheckForNewVersion()
{
	std::string updateInfo = Cmd5Info::GetWebsiteText(m_updateInfoURL);
	std::vector<std::string> splitInfo;
	std::vector<std::string> splitVer;

	// split returned data 
	boost::algorithm::split(splitInfo, updateInfo, boost::is_any_of("|"));
	if (splitInfo.size() != 4)
	{
		return 0;
	}
	
	// split version and set `newVersion` accordingly
	boost::algorithm::split(splitVer, splitInfo[0], boost::is_any_of("."));
	if (splitVer.size() != 3)
	{
		return 0;
	}
	try 
	{
		m_newVersion.major = boost::lexical_cast<int>(splitVer[0]);
		m_newVersion.minor = boost::lexical_cast<int>(splitVer[1]);
		m_newVersion.patch = boost::lexical_cast<int>(splitVer[2]);
	}
	catch (boost::bad_lexical_cast const&) 
	{
		return 0;
	}

	m_fileURL = splitInfo[1];
	m_fileMD5 = splitInfo[2];
	m_additionalInfo = splitInfo[3];
	return 1;
}

bool CSelfUpdater::GeneratePaths()
{
	if (!GetModulePath(m_currentFilePath)) return 0;
	boost::filesystem::path path(m_currentFilePath);

	if (!path.has_parent_path()) return 0;
	m_parentPath = path.parent_path().string();

	m_newVerTempFilePath = m_parentPath + "/sampac_new.tmp";
	m_oldVerTempFilePath = m_parentPath + "/sampac_old.tmp";
	return 1;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

// THIS FUNCTION IS UNTHREADED, SO IT WILL HANG THE THREAD FOR A WHILE!!!
bool CSelfUpdater::DownloadUpdate()
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	const char *url = m_fileURL.c_str();

	curl = curl_easy_init();
	if (!curl) return 0;
	
	fp = fopen(m_newVerTempFilePath.c_str(), "wb");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	res = curl_easy_perform(curl);
	/* always cleanup */
	curl_easy_cleanup(curl);
	fclose(fp);

	if (res != CURLE_OK)
	{
		try
		{
			boost::filesystem::remove(m_newVerTempFilePath);
		}
		catch (const boost::filesystem::filesystem_error&) { }

		return 0;
	}

	MD5 md5;
	std::string fileMD5 = md5.digestFileChar(m_newVerTempFilePath.c_str());
	if (!boost::iequals(fileMD5, m_fileMD5))
	{
		try
		{
			boost::filesystem::remove(m_newVerTempFilePath);
		}
		catch (const boost::filesystem::filesystem_error&) {}
	}

	return 1;
}

bool CSelfUpdater::ApplyUpdate()
{
#ifdef WIN32
	if (!MoveFileExA(m_currentFilePath.c_str(), m_oldVerTempFilePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
		return 0;

	if (!MoveFileExA(m_newVerTempFilePath.c_str(), m_currentFilePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
		return 0;
#else
	if (unlink(m_currentFilePath.c_str()) != 0)
		return 0;

	if (rename(m_newVerTempFilePath.c_str(), m_currentFilePath.c_str()) != 0)
		return 0;
#endif
	return 1;
}

bool CSelfUpdater::GetModulePath(std::string& path)
{
	int retVal;
#ifdef WIN32
	char DllPath[MAX_PATH] = { 0 };
	retVal = GetModuleFileNameA((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
	path = DllPath;
#else
	Dl_info dl_info;
	retVal = dladdr((const void *)&write_data, &dl_info);
	path = dl_info.dli_fname;
#endif
	return !!retVal;
}