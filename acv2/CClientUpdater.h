#pragma once
#include <string>
#include <Windows.h>

// Changing CURRENT_VERSION will force the client to update when they run it next.
#define CURRENT_VERSION 0.01f

// Changing CURRENT_MAJOR_VERSION DOES NOT FORCE THE USER TO UPDATE IF IT'S CHANGED!
// Only changing CURRENT_VERSION will force the user to update.
// CURRENT_MAJOR_VERSION is sent to the server and is used ONLY to tell the server if this client is compatible with it.
#define CURRENT_MAJOR_VERSION 0.01f

#define UPDATE_URL "http://whitetigerswt.github.io/SAMP_AC_v2/SAMP_AC_v2_Client_Updater.html"

class CClientUpdater
{
public:
	CClientUpdater();
	~CClientUpdater();

	static void CheckForUpdate(HMODULE hMod);

private:
	static void UpdateClient(std::string url, HMODULE hMod);
	void RemoveOldFile();

};

