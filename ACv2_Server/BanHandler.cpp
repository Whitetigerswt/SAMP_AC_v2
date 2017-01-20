#include "BanHandler.h"
#include "GDK/sampgdk.h"
#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"
#include <curl/curl.h>
#include "Utility.h"
#include "CServerUpdater.h"

namespace BanHandler
{
	void AddCheater(unsigned int playerid, std::string reason)
	{
		// Find a CAntiCheat class associated with this player (this was created in Network::HandleConnection earlier in this function)
		CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(playerid);
		if (ac == NULL)
		{
			// error?
			Utility::Printf("failed to add player %d to ban list due to CAntiCheat class error.", playerid);
			return;
		}

		// Get the player's Hardware ID.
		std::string hwid = "";
		hwid = ac->GetPlayerHardwareID();

		// Delcare 2 variables for player's name and IP
		char name[MAX_PLAYER_NAME], ip[16];

		// Get the player's name
		GetPlayerName(playerid, name, sizeof(name));

		// Get the player's IP
		GetPlayerIp(playerid, ip, sizeof ip);

		// Get server info: hostname and port
		char server_name[64];
		GetServerVarAsString("hostname", server_name, sizeof server_name);
		int server_port;
		server_port = GetServerVarAsInt("port");

		boost::thread addBan(BanHandler::addBan, std::string(name), std::string(ip), hwid, reason, std::string(server_name), server_port);
	}

	void addBan(std::string name, std::string ip, std::string hwid, std::string reason, std::string server_name, int server_port)
	{
		// Completely prepared now. Let's send data to the web server (which takes it to database)!
		CURL *curl;
		curl_global_init(CURL_GLOBAL_ALL);

		// curl handle
		curl = curl_easy_init();
		if (curl)
		{
			// Set URL to receive POST data
			curl_easy_setopt(curl, CURLOPT_URL, AC_BAN_HANDLER_ADD);

			/*
			* If you want to connect to a site who isn't using a certificate that is
			* signed by one of the certs in the CA bundle you have, you can skip the
			* verification of the server's certificate. This makes the connection
			* A LOT LESS SECURE.
			*
			* If you have a CA cert for the server stored someplace else than in the
			* default bundle, then the CURLOPT_CAPATH option might come handy for
			* you.
			*/
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

			/*
			* If the site you're connecting to uses a different host name that what
			* they have mentioned in their server certificate's commonName (or
			* subjectAltName) fields, libcurl will refuse to connect. You can skip
			* this check, but this will make the connection less secure.
			*/
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

			// Adds user agent to request
			curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_REQUEST_USER_AGENT);

			// Escape some parameters
			char
				*escaped_name = curl_easy_escape(curl, name.c_str(), 0),
				*escaped_server_name = curl_easy_escape(curl, server_name.c_str(), 0),
				*escaped_ban_reason = curl_easy_escape(curl, reason.c_str(), 0);

			if (escaped_name && escaped_server_name && escaped_ban_reason)
			{
				// Format POST data
				char str[400];
				snprintf(str, sizeof str, "Cheater=%s&CheaterIP=%s&Hardware=%s&Reason=%s&ServerName=%s&Port=%d",
					escaped_name, ip, hwid.c_str(), escaped_ban_reason, escaped_server_name, server_port);

				// Set POST data
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);

				// Send the request and store result into "res" variable
				CURLcode res;
				res = curl_easy_perform(curl);
			}


			// Clean up
			curl_free(escaped_name);
			curl_free(escaped_server_name);
			curl_free(escaped_ban_reason);
			curl_easy_cleanup(curl);
		}

		// Clean up
		curl_global_cleanup();
	}

	void CheckCheaterWeb(unsigned int playerid, std::string ip, std::string hwid)
	{

		bool ischeater = false;

		CURL *curl;
		curl_global_init(CURL_GLOBAL_ALL);
		// curl handle
		curl = curl_easy_init();
		if (curl)
		{
			// Set URL to receive POST data
			curl_easy_setopt(curl, CURLOPT_URL, AC_BAN_HANDLER_CHECK);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);


			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

			// Adds user agent to request
			curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_REQUEST_USER_AGENT);

			// Format POST data with player's hardware ID.
			char str[64];
			snprintf(str, sizeof str, "Hardware=%s&IP=%s", hwid.c_str(), ip.c_str());

			// Set POST data
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);

			// Send the request and store result into "res" variable
			CURLcode res;
			res = curl_easy_perform(curl);

			// Handle possible errors
			if (res != CURLE_OK)
			{
				Utility::Printf("curl_easy_perform() failed: %s while checking if player %d is in ban list.", curl_easy_strerror(res), playerid);
			}
			else // success
			{
				// Now let's handle response codes
				long response_code;
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

				switch (response_code)
				{
				case 508:
				{
					// Found in ban list.
					ischeater = true;
					break;
				}
				default:
				{
					// Was not found in ban list.
					ischeater = false;
					break;
				}
				}
			}
			// Clean up
			curl_easy_cleanup(curl);
		}
		else
		{
			Utility::Printf("failed to initialize curl handle while trying to add player %d to ban list.", playerid);
		}

		// Clean up
		curl_global_cleanup();

		CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(playerid);

		ac->OnBanChecked(ischeater);
	}

	void CheckCheater(unsigned int playerid)
	{

		// Find a CAntiCheat class associated with this player (this was created in Network::HandleConnection earlier in this function)
		CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(playerid);
		if (ac == NULL)
		{
			// error?
			Utility::Printf("failed while checking if player %d is in ban list due to CAntiCheat class error.", playerid);
		}

		// Get the player's IP
		char ip[16];
		GetPlayerIp(playerid, ip, sizeof ip);

		// Get the player's Hardware ID.
		std::string hwid = "";
		hwid = ac->GetPlayerHardwareID();

		boost::thread NetworkCall(&BanHandler::CheckCheaterWeb, playerid, std::string(ip), hwid);


		// Return whether this is a cheater or not
		//return ischeater;
	}
}
