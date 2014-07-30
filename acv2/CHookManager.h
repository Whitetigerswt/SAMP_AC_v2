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
	static void CheckMemoryAddr(void* address, int size, char* tomatch);
	static void VerifyFilePaths();
	static void LoadShotgunBullet();
	static void LoadBullet();
	
};

