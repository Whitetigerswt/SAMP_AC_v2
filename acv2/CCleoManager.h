#pragma once
#include <map>

#define HOOK __declspec(naked) void

class CCleoManager
{
public:
	static void Load();
	static void Monitor();
private:
	static void LoadScripts();
	
};

