#include "Cmd5Info.h"
#include <boost/network/protocol/http/client.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace boost::network;

std::map<std::string, std::string> Cmd5Info::g_mGta3ImgDefaults;

std::map<std::string, std::string> Cmd5Info::GetIMGMD5s()
{
	std::string html = GetWebsiteText(AC_IMG_MD5_INFO);
	std::vector<std::string> split_html;
	boost::split(split_html, html, boost::is_any_of("|"));

	for (std::vector<std::string>::iterator it = split_html.begin(); it != split_html.end(); ++it)
	{
		std::vector<std::string> split_string;
		boost::split(split_string, (*it), boost::is_any_of(","));

		g_mGta3ImgDefaults.insert(std::pair<std::string, std::string>(split_string[0], split_string[1]));
	}

	return g_mGta3ImgDefaults;
}

std::string Cmd5Info::GetWebsiteText(std::string url)
{
	try
	{
		http::client client;
		http::client::request request(url);
		http::client::response response = client.get(request);

		std::string data = body(request);
		return data;
	}
	catch (std::exception)
	{

	}
	return "";
}