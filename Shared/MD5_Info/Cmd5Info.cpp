#include "Cmd5Info.h"
#include <boost/network/protocol/http/client.hpp>
#include <string>
#include <sstream>
#include <iostream>

#include "CLog.h"

using namespace boost::network;

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
		std::string md5 = it->substr(i+1);

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

std::map<std::string, std::vector<std::string> > Cmd5Info::GetGtaDirectoryFiles()
{
	// Create an std::map
	std::map<std::string, std::vector<std::string> > gtaDirFiles;

	// Get website body holding our MD5 info for all bad executable files.
	// This will output something like "md5\nmd5\nmd5"
	std::string html = GetWebsiteText(AC_DIR_MD5_INFO);

	std::vector<std::string> split_html;

	// split the string into an std::vector by every "\n" in the string
	// one string becomes "md5"
	boost::split(split_html, html, boost::is_any_of("\n"));

	// Loop through every instance of "filename,md5"
	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		std::string szLine((*it));
		std::vector<std::string> md5_list;

		std::size_t i = szLine.find(",");

		std::string filename = szLine.substr(0, 1);

		boost::split(md5_list, filename, boost::is_any_of(","));

		std::vector<std::string> final_list;
		for (std::vector<std::string>::iterator iterator = md5_list.begin(); iterator != md5_list.end(); ++iterator)
		{
			std::string szMD5((*iterator));
			final_list.push_back(szMD5);
		}

		gtaDirFiles.insert(std::pair<std::string, std::vector<std::string> >(filename, final_list));
	}
	return gtaDirFiles;
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