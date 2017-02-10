#pragma once

#ifdef WIN32
#define AC_UPDATE_URL "http://samp-ac.com/API/Server_Windows_Updater.html"
#else
#define AC_UPDATE_URL "http://samp-ac.com/API/Server_Linux_Updater.html"
#endif

#define CURRENT_VERSION 2.82f
#define CURRENT_MAJOR_VERSION 2.32f
#define HTTP_REQUEST_USER_AGENT	"ACSVR_2.82" 

#include <string>

#include <fstream>
#include <iostream>

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

