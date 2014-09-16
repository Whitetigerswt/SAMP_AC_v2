#pragma once
#include <string>
#include <Windows.h>

// Changing CURRENT_VERSION will force the client to update when they run it next.
#define CURRENT_VERSION 2.5f

// Changing CURRENT_MAJOR_VERSION DOES NOT FORCE THE USER TO UPDATE IF IT'S CHANGED!
// Only changing CURRENT_VERSION will force the user to update.
// CURRENT_MAJOR_VERSION is sent to the server and is used ONLY to tell the server if this client is compatible with it.
#define CURRENT_MAJOR_VERSION 2.0f

// The current update URL
#define UPDATE_URL "http://whitetigerswt.github.io/SAMP_AC_v2/API/SAMP_AC_v2_Client_Updater.html"

class CClientUpdater
{
public:
	CClientUpdater();
	~CClientUpdater();

	// PURPOSE: Check for the latest update
	// REQUIRED: The current hModule
	// PROMISE: After the function returns, guaranteed we're on the latest version.
	static void CheckForUpdate(HMODULE hMod);

private:

	// PURPOSE: Update the client
	// REQUIRED: The update URL, the current hModule
	// PROMISE: After the function returns, the client will be updated.
	static void UpdateClient(std::string url, HMODULE hMod);

};
