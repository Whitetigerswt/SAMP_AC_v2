#include "VersionHelper.h"
#include "../Shared/SelfUpdater/CSelfUpdater.h"
#include "CLoader.h"

namespace VersionHelper
{
	const char* AC_UPDATE_URL = "https://whitetigerswt.github.io/SAMP_AC_v2/API/SelfUpdater/Client.html";
	wchar_t AC_CLIENT_VERSION_STRING[16];
	CSelfUpdater::stVersion AC_CLIENT_VERSION{ 2, 83, 0 };
	// Incrementing the version will force the client to update when they run it next.
	// Only MAJOR and/or MINOR version numbers determine if server is compatible with client.
	// If only PATCH version number is different, server and client are compatible.

	void Initialize()
	{
		swprintf_s(AC_CLIENT_VERSION_STRING, L"%d.%02d.%d", AC_CLIENT_VERSION.major, AC_CLIENT_VERSION.minor, AC_CLIENT_VERSION.patch);
	}

	void CheckForUpdate()
	{
		CSelfUpdater updater = CSelfUpdater(AC_CLIENT_VERSION, AC_UPDATE_URL);
		if (!updater.CheckForNewVersion())
		{
			return;
		}
		if (updater.m_currentVersion >= updater.m_newVersion)
		{
			return;
		}
		if (!updater.DownloadUpdate())
		{
			goto UPDATE_FAIL;
		}
		if (!updater.ApplyUpdate())
		{
			goto UPDATE_FAIL;
		}
		
		CLoader::RunElevated();
		return;

	UPDATE_FAIL:
		MessageBox(NULL,
			TEXT("SAMP-AC has failed at installing an update. You will need to update manually to keep using this mod.\n\nYou can download the latest version here: https://whitetigerswt.github.io/SAMP_AC_v2"),
			TEXT("An error occured while updating"),
			MB_OK | MB_ICONEXCLAMATION);
		ExitProcess(0);
	}
}