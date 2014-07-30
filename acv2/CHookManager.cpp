#include "CHookManager.h"
#include "CPatch.h"
#include "Addresses.h"
#include "CMem.h"
#include <Windows.h>

static DWORD LoadScriptsJmpBack = 0x05DE687;
static DWORD ShotgunBullet1JmpBack = 0x073FC6A;
static DWORD LoadBulletJmpBack = 0x73FC0B;

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
	CPatch::RedirectCall(FUNC_Scripts_Init1, LoadScripts);

	VirtualProtect((void*)FUNC_Scripts_Init2, 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	CPatch::RedirectCall(FUNC_Scripts_Init2, LoadScripts);

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

	CMem::ApplyJmp(FUNC_ShotgunBullet, (DWORD)LoadShotgunBullet, 11);
	CMem::ApplyJmp(FUNC_DeagleBullet, (DWORD)LoadBullet, 7);

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

HOOK CHookManager::LoadScripts()
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