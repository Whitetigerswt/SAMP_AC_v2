#include "CServerUpdater.h"
#include "../Shared/MD5_Info/Cmd5Info.h"
#include "Utility.h"

#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <boost/filesystem.hpp>

namespace http = boost::network::http;
namespace uri = boost::network::uri;

CServerUpdater::CServerUpdater()
{
}


CServerUpdater::~CServerUpdater()
{
}

void CServerUpdater::CheckForUpdate()
{
	// Get the website's response
	Utility::Printf("checking for an update...");
	try {
		std::string updatestring = Cmd5Info::GetWebsiteText(AC_UPDATE_URL);

		// Create new variables to split up the version and the update URL.
		float version = 0.0f;
		char* site = new char[256];

		// Split up the version and update URL.
		if (sscanf(updatestring.c_str(), "%f %s", &version, site) != EOF)
		{
			// If the version returned from the website is greater than the current version
			if (version > CURRENT_VERSION)
			{
				// Force the user to update.
				Utility::Printf("_____________________________________________________________");
				Utility::Printf("");
				Utility::Printf("SAMP_AC Server plugin is out of date, and will not continue to work after this point.");
				Utility::Printf("The current version is %.2f and the latest version is %.2f!", CURRENT_VERSION, version);
				Utility::Printf("You can download the latest version at: %s", site);
				Utility::Printf("To continue using this plugin, you must update it.");
				Utility::Printf("We're going to attempt to update automatically, but it may not succeed.");
				Utility::Printf("If you no longer wish to use this plugin, remove it from the server.cfg and adjust your scripts accordingly.");
				Utility::Printf("The server will now close.");
				Utility::Printf("_____________________________________________________________");

				AttemptToUpdatePlugin(site);
				exit(0);
			}
			else
			{
				Utility::Printf("_____________________________________________________________");
				Utility::Printf("");
				Utility::Printf("** Already up to date!");
				Utility::Printf("");
				Utility::Printf("_____________________________________________________________");
			}
		}
		delete[] site;
	}
	catch (std::exception &e)
	{
		Utility::Printf("error: %s", e.what());
	}
}

void CServerUpdater::AttemptToUpdatePlugin(std::string url)
{
	std::string filepath = DownloadUpdate(url);

	std::string randompath = "plugins/sampac_old_version_";
	randompath.append(boost::filesystem::unique_path().generic_string());

#ifdef WIN32
	rename("plugins/sampac.dll", randompath.c_str());
	rename(filepath.c_str(), "plugins/sampac.dll");
#else
	rename("plugins/sampac.so", randompath.c_str());
	rename(filepath.c_str(), "plugins/sampac.so");
#endif
}

std::string CServerUpdater::DownloadUpdate(std::string url)
{
	http::client client;
	http::client::request request(url);
	http::client::response response = client.get(request);

	std::string filename = "plugins/";

	filename.append(get_filename(request.uri()));

	std::ofstream ofs(filename.c_str());
	ofs << static_cast<std::string>(body(response)) << std::endl;

	return filename;
}