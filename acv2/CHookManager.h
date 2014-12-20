#pragma once
#include <map>

#define HOOK __declspec(naked) void

class CHookManager
{
public:
	// PURPOSE: Load the main hooks in the game.
	// REQUiRES: NULL
	static void Load();

	// PURPOSE: Install the hooks in the game after the user connects to the server.
	// REQUIRES: NULL
	static void SetConnectPatches();

	// PURPOSE: Install the frame limiter patch.
	// REQUIRES: NULL
	static void SetFrameLimiterPatch();

	// PURPOSE: Toggle lite foot on and off.
	// REQUIRES: NULL
	static void SetLiteFoot(bool toggle);

	// PURPOSE: Toggle on and off the macro limitations.
	// REQUIRES: NULL
	static void SetMacroLocks(bool toggle);

	// PURPOSE: Toggle on and off sprinting on all surfaces.
	// REQUIRES: NULL
	static void ToggleSprintOnAllSurfaces(bool toggle);

private:

	// PURPOSE: Hook CLEO funcs and make CLEO of various versions not load cleo scripts 
	// REQUIRES: NULL
	// NOTES: This doesn't prevent CLEO.asi from loading, it will load, what it does prevent is cleo *.cs scripts from loading.
	// NOTES: e.g: If you paused the game it will always say "Cleo scripts loaded: 0"
	static void LoadScriptsHook();

	// PURPOSE: Check a memory address equals a certain value.
	// REQUIRES: address, size, and a string to match.
	static void CheckMemoryAddr(void* address, int size, char* tomatch);

	// PURPOSE: Scans gta_sa.exe for edited file paths redirecting to a different .dat, .cfg, or .img
	// REQUIRES: NULL
	static void VerifyFilePaths();

	// PURPOSE: Prevent werner from working
	static void LoadShotgunBullet();
	static void LoadBullet();

	//static void CPed_Special_Flags();

	// hooks for when pauseing
	static void OnPause();
	static void OnUnPause();
	static void OnPauseChange();

	// fatulous: some random hack from 2006
	static void Fatulous1();
	static void Fatulous2();
	static void Fatulous3();

	// name tag hack: http://ugbase.eu/releases/)2014(-nametag-distance-los-(0-3z)/
	static void NameTagHook();

	// Makes widescreen mod crash the game
	static void WidescreenPatch();

	// Makes https://github.com/Whitetigerswt/samp-fov-changer crash the game
	static void FOVPatch();

	// Hook for all key presses
	// protip: keylogger found here
	static void KeyPress();

	// Hook to limit sprint
	static void SprintHook();

	// Hook to prevent litefooting, hooks inside some weird sprint function after you shoot
	// note: only partially works, people can still litefoot :(
	// needs further research...
	static void LiteFootHook();

	// Hooks gravity function which causes sobeit to crash.
	static void GravityHook();

	// Hook to allow us to turn on/off vehicle blips
	static void MarkersHook();

	static void SetCursorPosHook();

	static float CameraXPos;
	static float CameraYPos;

	static unsigned int iLastTick;
	static int iLastPress;
	static int iTickOffset;

	static int NameTagHookAddr;

	static float LiteFoot;

	// TOO MANY HOOKS TO DOCUMENT
	// basically all the camera hooks do is redirect the game's X and Y camera pos's to the variables "CameraXPos" and "CameraYPos"
	// We hook this to prevent aimbots from editing the traditional addresses.

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
	static void CameraXWriteHook11();

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
	static void CameraXAccessHook40();

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

