#pragma once

#include <map>

class CAntiCheatHandler
{
public:
	static CAntiCheat* GetAntiCheat(unsigned int playerid);
	static void Init(unsigned int playerid);
	static bool IsConnected(unsigned int playerid);
	static void Remove(unsigned int playerid);
private:
	static std::map<unsigned int, CAntiCheat*> players;
};

