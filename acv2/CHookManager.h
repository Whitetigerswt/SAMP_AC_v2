#pragma once
#include <map>

#define HOOK __declspec(naked) void

class CHookManager
{
public:
	static void Load();
	static void Monitor();
private:
	static void LoadScripts();
	
};

