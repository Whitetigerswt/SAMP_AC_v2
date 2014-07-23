#pragma once
#include <string>
#include <Windows.h>

#define CURRENT_VERSION 0.1f

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

