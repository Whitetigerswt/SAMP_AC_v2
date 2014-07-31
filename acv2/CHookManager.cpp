#include "CHookManager.h"
#include "CPatch.h"
#include "Hooks.h"
#include "CMem.h"
#include <Windows.h>

// Small children look away, this is gonna get ugly...
// This is the most poorly documented file, and the most confusing in all of the project.

static DWORD LoadScriptsJmpBack = 0x05DE687;
static DWORD ShotgunBullet1JmpBack = 0x073FC6A;
static DWORD LoadBulletJmpBack = 0x73FC0B;

static DWORD CameraXHook1JmpBack = 0x523F97;
static DWORD CameraXHook2JmpBack = 0x5241CC;
static DWORD CameraXHook3JmpBack = 0x510D72;
static DWORD CameraXHook4JmpBack = 0x50E573;
static DWORD CameraXHook5JmpBack = 0x515675;
static DWORD CameraXHook6JmpBack = 0x5225D3;
static DWORD CameraXHook7JmpBack = 0x5222BC;

static DWORD CameraXAccess1JmpBack = 0x5237E5;
static DWORD CameraXAccess2JmpBack = 0x523804;
static DWORD CameraXAccess3JmpBack = 0x523844;
static DWORD CameraXAccess4JmpBack = 0x5241B5;
static DWORD CameraXAccess5JmpBack = 0x5241D2;
static DWORD CameraXAccess6JmpBack = 0x5241DF;
static DWORD CameraXAccess7JmpBack = 0x5241F3;
static DWORD CameraXAccess8JmpBack = 0x5242CF;
static DWORD CameraXAccess9JmpBack = 0x5242DD;
static DWORD CameraXAccess10JmpBack = 0x509C62;

// 11 is skipped

static DWORD CameraXAccess12JmpBack = 0x5231D0;
static DWORD CameraXAccess13JmpBack = 0x5232B9;
static DWORD CameraXAccess14JmpBack = 0x5232CD;

// WHILE AIMING CAMERA X HOOKS //

static DWORD CameraXAccess15JmpBack = 0x52A89E;
static DWORD CameraXAccess16JmpBack = 0x52A8BD;
static DWORD CameraXAccess17JmpBack = 0x52A8DA;
static DWORD CameraXAccess18JmpBack = 0x515614;
static DWORD CameraXAccess19JmpBack = 0x522983;
static DWORD CameraXAccess20JmpBack = 0x522997;

float CHookManager::CameraXPos = 0.0f;

void CHookManager::Load()
{
	DWORD dwOldProt;

	// Prevent CLEO 4 from loading scripts
	VirtualProtect(FUNC_Init_SCM1, 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_Init_SCM1, "\xE8\x74\xCF\xF2\xFF", 5);

	VirtualProtect(FUNC_Init_SCM2, 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_Init_SCM2, "\xE8\x0B\xEA\xEA\xFF", 5);

	// Prevent CLEO 3's loading functions
	VirtualProtect((void*)FUNC_Scripts_Init1, 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	CPatch::RedirectCall(FUNC_Scripts_Init1, LoadScriptsHook);

	VirtualProtect((void*)FUNC_Scripts_Init2, 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	CPatch::RedirectCall(FUNC_Scripts_Init2, LoadScriptsHook);

	// Prevent Infinite ammo patch
	VirtualProtect(VAR_INF_AMMO, 2, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(VAR_INF_AMMO, "\x90\x90", 2);

	// Prevent infinite oxygen patch
	VirtualProtect(VAR_INF_OXYGEN, 2, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(VAR_INF_OXYGEN, "\x90\x90", 2);

	// Prevent infinite hp patch
	VirtualProtect(VAR_INF_HP, 1, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(VAR_INF_HP, "\xEB", 1);

	// Prevent Full Weapon Aiming patch
	VirtualProtect(VAR_FULL_WEAPON_AIMING, 6, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(VAR_FULL_WEAPON_AIMING, "\x90\x90\x90\x90\x90\x90", 6);

	// Disable Werner patch
	CMem::ApplyJmp(FUNC_ShotgunBullet, (DWORD)LoadShotgunBullet, 11);
	CMem::ApplyJmp(FUNC_DeagleBullet, (DWORD)LoadBullet, 7);

	// -------------------------------------------------------------------------
	// Hook camera position patches below, so aimbots cannot edit the camera position.
	// Leave your brain behind, because this will fuck it
	// -------------------------------------------------------------------------

	// Prevent some weird setting of camera X pos
	VirtualProtect(FUNC_CameraXWriteHook1, 2, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_CameraXWriteHook1, "\xEB\x08", 2);

	CMem::ApplyJmp(FUNC_CameraXWriteHook2, (DWORD)CameraXWriteHook2, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook3, (DWORD)CameraXWriteHook3, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook4, (DWORD)CameraXWriteHook4, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook5, (DWORD)CameraXWriteHook5, 7); // looks like array loop in asm 
	CMem::ApplyJmp(FUNC_CameraXWriteHook6, (DWORD)CameraXWriteHook6, 12);
	CMem::ApplyJmp(FUNC_CameraXWriteHook7, (DWORD)CameraXWriteHook7, 12);

	CMem::ApplyJmp(FUNC_CameraXAccessHook1, (DWORD)CameraXAccessHook1, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook2, (DWORD)CameraXAccessHook2, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook3, (DWORD)CameraXAccessHook3, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook4, (DWORD)CameraXAccessHook4, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook5, (DWORD)CameraXAccessHook5, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook6, (DWORD)CameraXAccessHook6, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook7, (DWORD)CameraXAccessHook7, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook8, (DWORD)CameraXAccessHook8, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook9, (DWORD)CameraXAccessHook9, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook10, (DWORD)CameraXAccessHook10, 18);
	CMem::ApplyJmp(FUNC_CameraXAccessHook11, (DWORD)CameraXAccessHook11, 18);
	CMem::ApplyJmp(FUNC_CameraXAccessHook12, (DWORD)CameraXAccessHook12, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook13, (DWORD)CameraXAccessHook13, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook14, (DWORD)CameraXAccessHook14, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook15, (DWORD)CameraXAccessHook15, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook16, (DWORD)CameraXAccessHook16, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook17, (DWORD)CameraXAccessHook17, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook18, (DWORD)CameraXAccessHook18, 7);
	CMem::ApplyJmp(FUNC_CameraXAccessHook19, (DWORD)CameraXAccessHook19, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook20, (DWORD)CameraXAccessHook20, 6);

	// Check data file integrity.
	VerifyFilePaths();
}

void CHookManager::VerifyFilePaths()
{
	// These addresses tell gta_sa where to find the data files
	// We need to verify they're the same so the game doesn't try to cheat and load the wrong files ;)

	CheckMemoryAddr(VAR_GTA3_IMG_PATH, 15, "MODELS\\GTA3.IMG");
	CheckMemoryAddr(VAR_ANIM_IMG_PATH, 12, "ANIM\\PED.IFP");
	CheckMemoryAddr(VAR_WEAPON_DAT_PATH, 15, "DATA\\WEAPON.DAT");
	CheckMemoryAddr(VAR_CARMODS_DAT_PATH, 16, "DATA\\CARMODS.DAT");
	CheckMemoryAddr(VAR_ANIMGRP_DAT_PATH, 16, "DATA\\ANIMGRP.DAT");
	CheckMemoryAddr(VAR_MELEE_DAT_PATH, 14, "DATA\\melee.dat");
	CheckMemoryAddr(VAR_CLOTHES_DAT_PATH, 16, "DATA\\CLOTHES.DAT");
	CheckMemoryAddr(VAR_OBJECT_DAT_PATH, 15, "DATA\\OBJECT.DAT");
	CheckMemoryAddr(VAR_DEFAULT_DAT_PATH, 16, "DATA\\DEFAULT.DAT");
	CheckMemoryAddr(VAR_SURFACE_DAT_PATH, 16, "data\\surface.dat");
	CheckMemoryAddr(VAR_GTA_DAT_PATH, 12, "DATA\\GTA.DAT");
	CheckMemoryAddr(VAR_WATER_DAT_PATH, 14, "DATA\\water.dat");
	CheckMemoryAddr(VAR_WATER1_DAT_PATH, 15, "DATA\\water1.dat");
	CheckMemoryAddr(VAR_FURNITUR_DAT_PATH, 17, "data\\furnitur.dat");
	CheckMemoryAddr(VAR_PROCOBJ_DAT_PATH, 16, "data\\procobj.dat");
	CheckMemoryAddr(VAR_HANDLING_CFG_PATH, 12, "HANDLING.CFG");
	CheckMemoryAddr(VAR_PEDSTATS_DAT_PATH, 17, "DATA\\PEDSTATS.DAT");
	CheckMemoryAddr(VAR_FONTS_TXD_PATH, 16, "MODELS\\FONTS.TXD");
	CheckMemoryAddr(VAR_PEDS_COLL_PATH, 20, "models\\coll\\peds.col");
	CheckMemoryAddr(VAR_STATDISP_PATH, 17, "DATA\\STATDISP.DAT");
	CheckMemoryAddr(VAR_ARSTATS_DAT_PATH, 17, "DATA\\AR_STATS.DAT");
	CheckMemoryAddr(VAR_SURFINFO_DAT_PATH, 17, "data\\surfinfo.dat");
}

void CHookManager::CheckMemoryAddr(void* address, int size, char* tomatch)
{
	DWORD dwOldProt;

	// Unprotect memory
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &dwOldProt);

	// Create new variable to hold the string that we're going to read from the address.
	char* memory = new char[size];

	// Get the string at the address.
	memcpy(memory, address, size);

	// Compare the string with what it's suppose to match
	if (strcmp(memory, tomatch) != 0)
	{
		// And if it doesn't match, copy what it's suppose to match into the address.
		memcpy(address, tomatch, size);
	}
	
	// Free memory.
	delete[] memory;
}

HOOK CHookManager::LoadScriptsHook()
{
	__asm
	{
		xor eax, eax
		add ecx, 16h
		jmp[LoadScriptsJmpBack]
	}
}

float fDefaultBulletOffset = 1.4f;

HOOK CHookManager::LoadShotgunBullet()
{
	__asm
	{
		mov[esp + 1Ch], 00000000
			fld[fDefaultBulletOffset]
		jmp[ShotgunBullet1JmpBack]
	}
}

HOOK CHookManager::LoadBullet()
{
	__asm
	{
		fld[fDefaultBulletOffset]
		mov[esp+24h], edx
		jmp[LoadBulletJmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook2()
{
	__asm
	{
		fadd dword ptr[CameraXPos]
		mov ecx, edi
		fstp[CameraXPos]
		jmp[CameraXHook1JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook3()
{
	__asm
	{
		fstp[CameraXPos]
		jmp [CameraXHook3JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook4()
{
	__asm
	{
		mov[CameraXPos],eax
		jmp[CameraXHook4JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook5()
{
	__asm
	{
		fstp[CameraXPos]
		jmp[CameraXHook5JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook6()
{
	__asm
	{
		fadd dword ptr[CameraXPos]
		fstp[CameraXPos]
		jmp[CameraXHook6JmpBack]
	}
}
HOOK CHookManager::CameraXWriteHook7()
{
	__asm
	{
		fadd dword ptr[CameraXPos]
		fstp[CameraXPos]
		jmp[CameraXHook7JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook1()
{
	__asm
	{
		fld[CameraXPos]
		push eax
		mov eax, 0x858CB8
		fadd dword ptr[eax]
		pop eax
		jmp[CameraXAccess1JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook2()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		push eax
		mov eax, 0x0858CB8
		fsub dword ptr[eax]
		pop eax
		jmp[CameraXAccess2JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook3()
{
	__asm
	{
		fld dword ptr[esp + 20h]
		fsub dword ptr[CameraXPos]
		jmp[CameraXAccess3JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook4()
{
	__asm
	{
		fsub dword ptr[CameraXPos]
		push eax
		mov eax, 0x8CCE74
		mov[eax], ecx
		pop eax
		jmp[CameraXAccess4JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook5()
{
	__asm
	{
		fld dword ptr[esi]
		mov eax, [CameraXPos]
		fcos
		jmp[CameraXAccess5JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook6()
{
	__asm
	{
		lea eax, [edi + 0x000190]
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess6JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook7()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		mov[eax], ecx
		fsin
		jmp[CameraXAccess7JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook8()
{
	__asm
	{
		fstp dword ptr[esp + 50h]
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess8JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook9()
{
	__asm
	{
		fst dword ptr[esp + 74h]
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess9JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook10()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		push eax
		mov eax, 0x858CB8
		fcomp dword ptr[eax]
		pop eax
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess10JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook11()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		push eax
		mov eax, 0x858CBC
		fadd dword ptr[eax]
		fstp dword ptr[CameraXPos]
		pop eax
		ret
	}
}

HOOK CHookManager::CameraXAccessHook12()
{
	__asm
	{
		fst dword ptr[CameraXPos]
		jmp[CameraXAccess12JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook13()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess13JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook14()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess14JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook15()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess15JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook16()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess16JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook17()
{
	__asm
	{
		fsub dword ptr[CameraXPos]
		jmp[CameraXAccess17JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook18()
{
	__asm
	{
		mov edx, [CameraXPos]
		jmp[CameraXAccess18JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook19()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess19JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook20()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess20JmpBack]
	}
}