#pragma once
#include <map>

#define HOOK __declspec(naked) void

class CHookManager
{
public:
	static void Load();
	static void SetConnectPatches();
	static void SetFrameLimiterPatch();

	static void SetLiteFoot(bool toggle);
private:
	static void LoadScriptsHook();
	static void CheckMemoryAddr(void* address, int size, char* tomatch);
	static void VerifyFilePaths();

	static void LoadShotgunBullet();
	static void LoadBullet();

	static void CPed_Special_Flags();

	static void WidescreenPatch();

	static void FOVPatch();

	static void SprintHook();

	static void LiteFootHook();

	static void GravityHook();

	static float CameraXPos;
	static float CameraYPos;

	static unsigned int iLastTick;
	static int iLastPress;
	static int iTickOffset;

	static float LiteFoot;

	static void CameraXWriteHook1();
	static void CameraXWriteHook2();
	static void CameraXWriteHook3();
	static void CameraXWriteHook4();
	static void CameraXWriteHook5();
	static void CameraXWriteHook6();
	static void CameraXWriteHook7();
	static void CameraXWriteHook8();
	static void CameraXWriteHook9();
	static void CameraXWriteHook10();

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
	static void CameraXAccessHook15();
	static void CameraXAccessHook16();
	static void CameraXAccessHook17();
	static void CameraXAccessHook18();
	static void CameraXAccessHook19();
	static void CameraXAccessHook20();
	static void CameraXAccessHook21();
	static void CameraXAccessHook22();
	static void CameraXAccessHook23();
	static void CameraXAccessHook24();
	static void CameraXAccessHook25();
	static void CameraXAccessHook26();
	static void CameraXAccessHook27();
	static void CameraXAccessHook28();
	static void CameraXAccessHook29();
	static void CameraXAccessHook30();
	static void CameraXAccessHook31();
	static void CameraXAccessHook32();
	static void CameraXAccessHook33();
	static void CameraXAccessHook34();
	static void CameraXAccessHook35();
	static void CameraXAccessHook36();
	static void CameraXAccessHook37();
	static void CameraXAccessHook38();
	static void CameraXAccessHook39();

	static void CameraYWriteHook1();
	static void CameraYWriteHook2();
	static void CameraYWriteHook3();
	static void CameraYWriteHook4();
	static void CameraYWriteHook5();
	static void CameraYWriteHook6();
	static void CameraYWriteHook7();
	//static void CameraYWriteHook8();
	static void CameraYWriteHook9();
	static void CameraYWriteHook10();
	static void CameraYWriteHook11();

	static void CameraYAccessHook1();
	static void CameraYAccessHook2();
	static void CameraYAccessHook3();
	static void CameraYAccessHook4();
	static void CameraYAccessHook5();
	static void CameraYAccessHook6();
	static void CameraYAccessHook7();
	static void CameraYAccessHook8();
	static void CameraYAccessHook9();
	static void CameraYAccessHook10();
	static void CameraYAccessHook11();
	static void CameraYAccessHook12();
	static void CameraYAccessHook13();
	static void CameraYAccessHook14();
	static void CameraYAccessHook15();
	static void CameraYAccessHook16();
	static void CameraYAccessHook17();
	static void CameraYAccessHook18();
	static void CameraYAccessHook19();
	static void CameraYAccessHook20();
	static void CameraYAccessHook21();
	static void CameraYAccessHook22();
	static void CameraYAccessHook23();
	static void CameraYAccessHook24();
	static void CameraYAccessHook25();
	static void CameraYAccessHook26();
	static void CameraYAccessHook27();
	
};

