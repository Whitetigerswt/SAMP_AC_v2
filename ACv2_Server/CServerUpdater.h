#pragma once

#ifdef WIN32
#define AC_UPDATE_URL "http://whitetigerswt.github.io/SAMP_AC_v2/API/Server_Windows_Updater.html"
#else
#define AC_UPDATE_URL "http://whitetigerswt.github.io/SAMP_AC_v2/API/Server_Linux_Updater.html"
#endif

#define CURRENT_VERSION 0.01f
#define CURRENT_MAJOR_VERSION 0.01f

#include <string>

#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>

#include <fstream>
#include <iostream>

namespace http = boost::network::http;
namespace uri = boost::network::uri;

namespace
{
	std::string get_filename(const uri::uri &url)
	{
		std::string path = uri::path(url);
		std::size_t index = path.find_last_of('/');
		std::string filename = path.substr(index + 1);
		return filename.empty() ? "index.html" : filename;
	}
}

class CServerUpdater
{
public:
	CServerUpdater();
	~CServerUpdater();

	// PURPOSE: Check for the latest server version.
	// REQUIRES: NULL.
	static void CheckForUpdate();

private:
	
	// PURPOSE: Attempt to download an update.
	// REQUIRES: A url to the latest update .so or .dll.
	static void AttemptToUpdatePlugin(std::string url);

	// PURPOSE: Download update from the specified URL. (.so or .dll)
	// REQUIRES: A URL to a .so or a .dll.
	static std::string DownloadUpdate(std::string url);
};

