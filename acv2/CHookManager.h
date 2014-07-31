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

	static float CameraXPos;

	static void CameraXWriteHook1();
	static void CameraXWriteHook2();
	static void CameraXWriteHook3();
	static void CameraXWriteHook4();
	static void CameraXWriteHook5();
	static void CameraXWriteHook6();
	static void CameraXWriteHook7();

	static void CameraXAccessHook1();
	static void CameraXAccessHook2();
	static void CameraXAccessHook3();
	static void CameraXAccessHook4();
	static void CameraXAccessHook5();
	static void CameraXAccessHook6();
	static void CameraXAccessHook7();
	static void CameraXAccessHook8();
	static void CameraXAccessHook9();
	static void CameraXAccessHook10();
	static void CameraXAccessHook11();
	static void CameraXAccessHook12();
	static void CameraXAccessHook13();
	static void CameraXAccessHook14();


	
};

