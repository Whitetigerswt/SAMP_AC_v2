#include "CClientUpdater.h"
#include "CLoader.h"

#include <Wininet.h>
#include <Urlmon.h>
#include <ctime>


CClientUpdater::CClientUpdater()
{
}


CClientUpdater::~CClientUpdater()
{
}

void CClientUpdater::CheckForUpdate(HMODULE hMod)
{
	// Open the update page, which will read something like "1.0 www.google.com"
	// where the 1.0 is the latest version
	// and the www.google.com is where to download the latest version
	HINTERNET hNet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	// Check the internet.
	if (hNet == NULL)
	{
		return;
	}

	// Open the internet URL link.
	HINTERNET hNetFile = InternetOpenUrlA(hNet, UPDATE_URL, 0, 0, 0, 0);

	// Make sure it was a successful connection
	if (hNetFile == NULL)
	{
		return;
	}

	// Prepare to read the result of the webpage into a buffer.
	char szLatestVersion[256] = "";
	memset(szLatestVersion, 0, sizeof(szLatestVersion));

	DWORD dwBytesRead = 0;
	do
	{
		// Get the text on the URL.
		InternetReadFile(hNetFile, (LPVOID)szLatestVersion, sizeof(szLatestVersion), &dwBytesRead);
	} while (dwBytesRead > 0);

	// Make sure szLatestVersion isn't empty.
	if (strlen(szLatestVersion) > 0)
	{
		// Remove all cache, so we get the latest version.
		DeleteUrlCacheEntry(szLatestVersion);

		// We need to format our variables so we have the version by itself, and the download link by itself.
		float version = 0.0f;
		char* downloadLink = new char[256];
		sscanf_s(szLatestVersion, "%f %s", &version, downloadLink);

		// Convert the download link to an std::string, cause they're easier to deal with...
		std::string szDownloadLink(downloadLink);

		// No memory leaks!
		delete[] downloadLink;

		// Compare the version on the web page to our #define of CURRENT_VERSION.
		if (version > CURRENT_VERSION)
		{
			// If it's greater than our define, we need to update.
			UpdateClient(szDownloadLink, hMod);
		}
	}
}

void CClientUpdater::UpdateClient(std::string downloadLink, HMODULE hMod)
{
	// Get the temporary folder, this is where our old .asi version will go.
	char tempDir[MAX_PATH + 1];
	GetTempPath(sizeof(tempDir), tempDir);

	// Generate a unique file name for our old .asi version.
	char tempFile[MAX_PATH + 1];
	srand((unsigned int)std::time(0));
	sprintf_s(tempFile, sizeof(tempFile), "%s%d_%d_%d_%d.asi", tempDir, rand(), rand(), rand(), rand());

	// Get the location of our old .asi version
	char currentFile[MAX_PATH + 1];
	GetModuleFileName(hMod, currentFile, sizeof(currentFile));

	// and move it to our temporary folder.
	MoveFile(currentFile, tempFile);

	// Download the new .asi version from the new technology known as "the internet"
	HRESULT hr = URLDownloadToFile(NULL, downloadLink.c_str(), currentFile, 0, NULL);


	if (SUCCEEDED(hr))
	{
		// If the download succeeded, great! re-run the program as an admin (which re-loads our .asi mod with the newest version, and it may be required anyway!)
		CLoader::RunElevated();
	}
	else
	{
		// Else an error occured and provide the link to the user so they can update manually.
		sprintf_s(currentFile, sizeof(currentFile), "SAMP AC has failed at installing an update. You will need to update manually to keep using this mod.\n\nYou can download the latest version here: %s", downloadLink.c_str());
		MessageBoxA(NULL, currentFile, "An error occured while updating", MB_OK | MB_ICONEXCLAMATION);
	}
}