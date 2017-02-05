#include "Cmd5Info.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <curl/curl.h>

#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>

#ifndef WIN32
#define MAX_PATH 260
#endif

std::map<std::string, std::string> Cmd5Info::GetIMGMD5s()
{
	// Crate a map to hold our results.
	std::map<std::string, std::string> gta3ImgDefaults;

	// Get website body holding our MD5 info for gta3.img
	// This will output something like "filename,md5\nfilename,md5\nfilename,md5"
	std::string html = GetWebsiteText(AC_IMG_MD5_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "\n" in the string
	// one string becomes "filename,md5"
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "filename,md5"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		// Find the comma, so we can seperate the filename, and m5.
		std::size_t i = it->find(",");

		// Seperate the filename and md5 string.
		std::string fname = it->substr(0, i);
		std::string md5 = it->substr(i + 1);

		// insert it into our std::map the results.
		if (strlen(fname.c_str()) > 0)
			gta3ImgDefaults.insert(std::pair<std::string, std::string>(fname, md5));
	}

	// Return the map of all the fname's and md5's
	return gta3ImgDefaults;
}

std::vector<std::string> Cmd5Info::GetBadExecutableFiles()
{
	// Create a vector to hold our results.
	std::vector<std::string> exeBadFiles;

	// Get website body holding our MD5 info for all bad executable files.
	// This will output something like "md5\nmd5\nmd5"
	std::string html = GetWebsiteText(AC_EXECUTABLE_MD5_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "\n" in the string
	// one string becomes "md5"
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "md5"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		// insert it into our std::vector the results.
		if (!it->empty())
		{
			exeBadFiles.push_back((*it));
		}
	}

	// Return the vector of all md5's
	return exeBadFiles;
}

std::vector<std::string> Cmd5Info::GetGtaDirectoryFilesMd5()
{
	// Create a vector to hold our results.
	std::vector<std::string> dirMd5Info;

	// Get website body holding our MD5 info for all unmodifies/original gta files
	// This will output something like "md5\nmd5\nmd5"
	std::string html = GetWebsiteText(AC_DIR_MD5_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "\n" in the string
	// one string becomes "md5"
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "md5"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		// insert it into our std::vector the results.
		if (!it->empty())
		{
			dirMd5Info.push_back((*it));
		}
	}

	// Return the vector of all md5's
	return dirMd5Info;
}

std::vector<std::string> Cmd5Info::GetGtaDirectoryFilesNames()
{
	// Create a vector to hold our results.
	std::vector<std::string> dirNameInfo;

	// Get website body holding info for all gta file names
	// This will output something like "filename\nfilename\nfilename" (including extension or sub files)
	std::string html = GetWebsiteText(AC_DIR_NAME_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "\n" in the string
	// one string becomes "filename" (including extension or sub files)
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "filename"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		// insert it into our std::vector the results.
		if (!it->empty())
		{
			dirNameInfo.push_back((*it));
		}
	}

	// Return the vector of all gta file names
	return dirNameInfo;
}

std::string data = "";

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
  //buf is a pointer to the data that curl has for us
  //size*nmemb is the size of the buffer

	for (size_t c = 0; c<size*nmemb; c++)
	{
		data.push_back(buf[c]);
	}
	return size*nmemb; //tell curl how many bytes we handled
}

std::string Cmd5Info::GetWebsiteText(std::string url)
{
	try
	{
		CURL* curl; //our curl object
		curl = curl_easy_init();

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			// If destination is secured
			if (url.compare(0, 5, "https") == 0)
			{
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
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

				/*
				* If the site you're connecting to uses a different host name that what
				* they have mentioned in their server certificate's commonName (or
				* subjectAltName) fields, libcurl will refuse to connect. You can skip
				* this check, but this will make the connection less secure.
				*/
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			}

			data.clear(); // string must be cleared before adding new data
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			curl_easy_perform(curl);

			curl_easy_cleanup(curl);
			return data;
		}
		return "";
	}
	catch (std::exception &e)
	{
		return e.what();
	}
	return "";
}

#ifdef WIN32
std::string Cmd5Info::DownloadFile(std::string url, std::wstring fname)
{
	// Create an output stream to paste the URL contents.
	std::ofstream ofs(fname.c_str());

	// Save the URL contents so we can return it later.
	std::string result = GetWebsiteText(url);

	// Paste the URL contents.
	ofs << static_cast<std::string>(result) << std::endl;

	return result;
}
#else
std::string Cmd5Info::DownloadFile(std::string url, std::string fname)
{
	// Create an output stream to paste the URL contents.
	std::ofstream ofs(fname.c_str());

	// Save the URL contents so we can return it later.
	std::string result = GetWebsiteText(url);

	// Paste the URL contents.
	ofs << static_cast<std::string>(result) << std::endl;

	return result;
}
#endif

