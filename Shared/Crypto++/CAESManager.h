#pragma once

#include <string>

class CAESManager
{
public:
	static std::string Encrypt(std::string);
	static std::string Decrypt(std::string);
};