#include "Cmd5Info.h"
#include <boost/network/protocol/http/client.hpp>
#include <string>
#include <sstream>
#include <iostream>

#include "CLog.h"

using namespace boost::network;

std::map<std::string, std::string> Cmd5Info::g_mGta3ImgDefaults;

std::map<std::string, std::string> Cmd5Info::GetIMGMD5s()
{
	// Clear g_mGta3ImgDefaults.
	g_mGta3ImgDefaults.clear();

	// Get website body holding our MD5 info for gta3.img
	// This will output something like "filename,md5|filename,md5|filename,md5" -> no trailing |
	std::string html = GetWebsiteText(AC_IMG_MD5_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "|" in the string
	// one string becomes "filename,md5"
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "filename,md5"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		// Find the comma, so we can seperate the filename, and m5.
		std::size_t i = it->find(",");

		// Seperate the filename and md5 string.
		std::string fname = it->substr(0, i);
		std::string md5 = it->substr(i+1);

		// insert it into our std::map the results.
		if (strlen(fname.c_str()) > 0)
			g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>(fname, md5));
	}

	// Return the map of all the fname's and md5's
	return g_mGta3ImgDefaults;
}

std::string Cmd5Info::GetWebsiteText(std::string url)
{
	try
	{
		// Connect to the website
		http::client c;
		http::client::request req(url);
		http::client::response res = c.get(req);

		// put the result in an std::string.
		std::string data = body(res);
		return data;
	}
	catch (std::exception)
	{

	}
	return "";
}