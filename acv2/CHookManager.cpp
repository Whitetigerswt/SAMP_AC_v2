#include <Windows.h>
#include <Boost\thread.hpp>

#include "Network\CRakClientHandler.h"
#include "CHookManager.h"
#include "CPatch.h"
#include "Hooks.h"
#include "CMem.h"
#include "Misc.h"
#include "Network\Network.h"
#include "../Shared/Network/CRPC.h"
#include "PatternScan.h"
#include "DirectX Hooks\CMessageProxy.h"
#include "s0beit\samp.h"
#include "ManualInjection.h"
#include "CPacketIntegrity.h"
#include "CLog.h"
#include "CMemProtect.h"
#include "Detours\detours.h"

// Small children look away, this is gonna get ugly...
// This is the most poorly documented file, and the most confusing in all of the project.

static DWORD LoadScriptsJmpBack = 0x05DE687;
static DWORD ShotgunBullet1JmpBack = 0x073FC6A;
static DWORD LoadBulletJmpBack = 0x73FC0B;
static DWORD WidescreenJmpBack = 0x5894B5;

static DWORD CameraXHook1JmpBack = 0x523F97;
static DWORD CameraXHook2JmpBack = 0x5241CC;
static DWORD CameraXHook3JmpBack = 0x510D72;
static DWORD CameraXHook4JmpBack = 0x50E573;
static DWORD CameraXHook5JmpBack = 0x515675;
static DWORD CameraXHook6JmpBack = 0x5225D3;
static DWORD CameraXHook7JmpBack = 0x5222BC;
static DWORD CameraXHook8JmpBack = 0x524C51;
static DWORD CameraXHook9JmpBack = 0x52590C;
static DWORD CameraXHook10JmpBack = 0x525B0E;
static DWORD CameraXHook11JmpBack = 0x509C75;
static DWORD CameraXHook12JmpBack = 0x511DBB;

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
static DWORD CameraXAccess21JmpBack = 0x5117C1;
static DWORD CameraXAccess22JmpBack = 0x5117D5;
static DWORD CameraXAccess23JmpBack = 0x5118D1;
static DWORD CameraXAccess24JmpBack = 0x511999;
static DWORD CameraXAccess25JmpBack = 0x520B7E;
static DWORD CameraXAccess26JmpBack = 0x520B9A;

// WHILE DRIVING CAMERA X HOOKS //

static DWORD CameraXAccess27JmpBack = 0x524FA1;
static DWORD CameraXAccess28JmpBack = 0x525000;
static DWORD CameraXAccess29JmpBack = 0x52590C;
static DWORD CameraXAccess30JmpBack = 0x524FC0;
static DWORD CameraXAccess31JmpBack = 0x525AED;
static DWORD CameraXAccess32JmpBack = 0x525B1A;
static DWORD CameraXAccess33JmpBack = 0x525B37;
static DWORD CameraXAccess34JmpBack = 0x5251F8;
static DWORD CameraXAccess35JmpBack = 0x525296;
static DWORD CameraXAccess36JmpBack = 0x52532C;
static DWORD CameraXAccess37JmpBack = 0x525B4B;
static DWORD CameraXAccess38JmpBack = 0x525C5C;
static DWORD CameraXAccess39JmpBack = 0x525C71;
static DWORD CameraXAccess40JmpBack = 0x524CAA;
static DWORD CameraXAccess41JmpBack = 0x511B96;
static DWORD CameraXAccess42JmpBack = 0x511E47;
static DWORD CameraXAccess43JmpBack = 0x511E5B;

// CAMERA Y WRITE HOOK JMP BACKS //

static DWORD CameraYWrite2JmpBack = 0x524153;
static DWORD CameraYWrite3JmpBack = 0x524153;
static DWORD CameraYWrite4JmpBack = 0x524164;
static DWORD CameraYWrite5JmpBack = 0x5225E3;
static DWORD CameraYWrite6JmpBack = 0x5222C8;
static DWORD CameraYWrite7JmpBack = 0x523239;
static DWORD CameraYWrite8JmpBack = 0x5232A9;
static DWORD CameraYWrite9JmpBack = 0x5233CC;
static DWORD CameraYWrite10JmpBack = 0x52260D;
static DWORD CameraYWrite11JmpBack = 0x522911;
static DWORD CameraYWrite12JmpBack = 0x511E31;
static DWORD CameraYWrite13JmpBack = 0x511DD2;
static DWORD CameraYWrite14JmpBack = 0x51157D;
static DWORD CameraYWrite15JmpBack = 0x5117AB;
static DWORD CameraYWrite16JmpBack = 0x511789;
static DWORD CameraYWrite17JmpBack = 0x51159C;

static DWORD CameraYAccess1JmpBack = 0x523BA7;
static DWORD CameraYAccess2JmpBack = 0x524153;
static DWORD CameraYAccess3JmpBack = 0x524176;
static DWORD CameraYAccess4JmpBack = 0x52418F;
static DWORD CameraYAccess5JmpBack = 0x52420C;
static DWORD CameraYAccess6JmpBack = 0x5225F0;
static DWORD CameraYAccess7JmpBack = 0x522904;
static DWORD CameraYAccess8JmpBack = 0x522921;
static DWORD CameraYAccess9JmpBack = 0x52295B;
static DWORD CameraYAccess10JmpBack = 0x5229B4;
static DWORD CameraYAccess11JmpBack = 0x50AD8B;
static DWORD CameraYAccess12JmpBack = 0x52293A;
static DWORD CameraYAccess13JmpBack = 0x5232E8;
static DWORD CameraYAccess14JmpBack = 0x5240A2;
static DWORD CameraYAccess15JmpBack = 0x5240C8;
static DWORD CameraYAccess16JmpBack = 0x5240EF;
static DWORD CameraYAccess17JmpBack = 0x511566;
static DWORD CameraYAccess18JmpBack = 0x511585;
static DWORD CameraYAccess19JmpBack = 0x51177C;
static DWORD CameraYAccess20JmpBack = 0x511793;
static DWORD CameraYAccess21JmpBack = 0x5117B1;
static DWORD CameraYAccess22JmpBack = 0x5117F0;
static DWORD CameraYAccess23JmpBack = 0x5118E1;
static DWORD CameraYAccess24JmpBack = 0x5119A9;
static DWORD CameraYAccess25JmpBack = 0x524112;
static DWORD CameraYAccess26JmpBack = 0x524127;
static DWORD CameraYAccess27JmpBack = 0x5240DD;
static DWORD CameraYAccess28JmpBack = 0x5232A9;
static DWORD CameraYAccess29JmpBack = 0x511E1A;
static DWORD CameraYAccess30JmpBack = 0x511E37;
static DWORD CameraYAccess31JmpBack = 0x511E76;

static DWORD CPed_Special_FlagsJmpBack = 0x5448E9;
static DWORD Fatulous1JmpBack = 0x6D8033;
static DWORD Fatulous2JmpBack = 0x4B3314;
static DWORD Fatulous3JmpBack = 0x64EEEA;

static DWORD FOVPatchJmpBack = 0x522F68;

static DWORD KeyPressJmpBack = 0x541C9D;

static DWORD GravityHookJmpBack1 = 0x543081;
static DWORD GravityHookJmpBack2 = 0x543093;

static DWORD PauseJmpBack = 0x576C2D;

static DWORD SetCursorPosHookJmpBack = 0x745433;
static DWORD SlideFixJmpBack = 0x686C39;

static DWORD WinMainHookJmpBack = 0x8246F1;
static DWORD rIdleHookJmpBack = 0x53ECC2;

static DWORD CLEO_Func_JmpBack = 0x46A222;
static DWORD MainLoadingJmpBack = 0x7F9B12;

static DWORD sampInfoAddr = NULL;
static boolean hooks_install_once = false;

float CHookManager::CameraXPos = 0.0f;
float CHookManager::CameraYPos = 0.0f;

int CHookManager::FireKeyState;

void CHookManager::Load()
{
	DWORD dwOldProt;
	DWORD samp_base_addr = getSampBaseAddress();
	DWORD samp;

	if (samp_base_addr)
	{
		if (!hooks_install_once)
		{
			samp = FindPattern("\x8B\x86\xCD\x03\x00\x00\x8B\x40\x18\x85\xC0", "xxxxxxxxxxx", getSampBaseAddress(), getSampSize());

			if (samp != 0)
			{
				// Save memory so we can remove hook later.
				sampInfoAddr = samp;

				// Unprotect memory so we can apply a jmp
				VirtualProtect((void*)samp, 6, PAGE_EXECUTE_READWRITE, &dwOldProt);

				// Install hook
				CMem::ApplyJmp((BYTE*)samp, (DWORD)GetSampInfo, 6);

				hooks_install_once = true;
			}

			samp = samp_base_addr + FUNC_OFFSET_RAKPEER__SENDBUFFERED;
		    m_pfnRakPeer__SendBuffered = (RakPeer__SendBuffered_t)DetourFunction((PBYTE)samp, (PBYTE)CHookManager::RakPeer__SendBufferedHook);

			FindProtectedMemoryAddresses();
		}
	}

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

	// Disable Werner patch
	CMem::ApplyJmp(FUNC_ShotgunBullet, (DWORD)LoadShotgunBullet, 11);
	CMem::ApplyJmp(FUNC_DeagleBullet, (DWORD)LoadBullet, 7);
	
	if (Misc::GetGameVersion() == 1)
	{
		// Hook a main loading function of SAMP and GTA. 
		CMem::ApplyJmp(FUNC_MainLoad, (DWORD)MainLoading, 5);
	}
	else if (Misc::GetGameVersion() == 2)
	{
		CMem::ApplyJmp(FUNC_MainLoadAlt, (DWORD)MainLoading, 5);
		MainLoadingJmpBack = 0x7F9B52;
	}

	// Triggerbot hooks
	CMem::ApplyJmp(FUNC_CPad__ProcessKeyboard1_Hook, (DWORD)ProcessKeyboard1_Hook, 0x9);
	CMem::ApplyJmp(FUNC_CPad__ProcessKeyboard2_Hook, (DWORD)ProcessKeyboard2_Hook, 0x9);
	CMem::ApplyJmp(FUNC_CPad__Clear_Hook, (DWORD)ClearKeyState_Hook, 0xE2 - 0x80);

	// -------------------------------------------------------------------------
	// Hook camera position patches below, so aimbots cannot edit the camera position.
	// Leave your brain behind, because this will fuck it
	// -------------------------------------------------------------------------

	// Prevent some weird setting of camera X pos
	VirtualProtect(FUNC_CameraXWriteHook1, 2, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_CameraXWriteHook1, "\xEB\x08", 2);

	CMem::ApplyJmp(FUNC_CameraXWriteHook2, (DWORD)CameraXWriteHook2, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook3, (DWORD)CameraXWriteHook3, 24);
	CMem::ApplyJmp(FUNC_CameraXWriteHook4, (DWORD)CameraXWriteHook4, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook5, (DWORD)CameraXWriteHook5, 7); // looks like array loop in asm 
	CMem::ApplyJmp(FUNC_CameraXWriteHook6, (DWORD)CameraXWriteHook6, 12);
	CMem::ApplyJmp(FUNC_CameraXWriteHook7, (DWORD)CameraXWriteHook7, 12);
	CMem::ApplyJmp(FUNC_CameraXWriteHook8, (DWORD)CameraXWriteHook8, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook9, (DWORD)CameraXWriteHook9, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook10, (DWORD)CameraXWriteHook10, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook11, (DWORD)CameraXWriteHook11, 6);
	CMem::ApplyJmp(FUNC_CameraXWriteHook12, (DWORD)CameraXWriteHook12, 35);

	CMem::ApplyJmp(FUNC_CameraXAccessHook1, (DWORD)CameraXAccessHook1, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook2, (DWORD)CameraXAccessHook2, 8);
	CMem::ApplyJmp(FUNC_CameraXAccessHook3, (DWORD)CameraXAccessHook3, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook4, (DWORD)CameraXAccessHook4, 10);
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
	CMem::ApplyJmp(FUNC_CameraXAccessHook19, (DWORD)CameraXAccessHook19, 16);
	CMem::ApplyJmp(FUNC_CameraXAccessHook20, (DWORD)CameraXAccessHook20, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook21, (DWORD)CameraXAccessHook21, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook22, (DWORD)CameraXAccessHook22, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook23, (DWORD)CameraXAccessHook23, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook24, (DWORD)CameraXAccessHook24, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook25, (DWORD)CameraXAccessHook25, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook26, (DWORD)CameraXAccessHook26, 6);

	CMem::ApplyJmp(FUNC_CameraXAccessHook27, (DWORD)CameraXAccessHook27, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook28, (DWORD)CameraXAccessHook28, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook29, (DWORD)CameraXAccessHook29, 12);
	CMem::ApplyJmp(FUNC_CameraXAccessHook30, (DWORD)CameraXAccessHook30, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook31, (DWORD)CameraXAccessHook31, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook32, (DWORD)CameraXAccessHook32, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook33, (DWORD)CameraXAccessHook33, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook34, (DWORD)CameraXAccessHook34, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook35, (DWORD)CameraXAccessHook35, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook36, (DWORD)CameraXAccessHook36, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook37, (DWORD)CameraXAccessHook37, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook38, (DWORD)CameraXAccessHook38, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook39, (DWORD)CameraXAccessHook39, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook40, (DWORD)CameraXAccessHook40, 32);
	CMem::ApplyJmp(FUNC_CameraXAccessHook41, (DWORD)CameraXAccessHook41, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook42, (DWORD)CameraXAccessHook42, 6);
	CMem::ApplyJmp(FUNC_CameraXAccessHook43, (DWORD)CameraXAccessHook43, 6);

	// CAMERA Y HOOKS //

	VirtualProtect(FUNC_CameraYWriteHook1, 1, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_CameraYWriteHook1, "\xEB", 1);

	CMem::ApplyJmp(FUNC_CameraYWriteHook2, (DWORD)CameraYWriteHook2, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook3, (DWORD)CameraYWriteHook3, 8);
	CMem::ApplyJmp(FUNC_CameraYWriteHook4, (DWORD)CameraYWriteHook4, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook5, (DWORD)CameraYWriteHook5, 12);
	CMem::ApplyJmp(FUNC_CameraYWriteHook6, (DWORD)CameraYWriteHook6, 12);
	CMem::ApplyJmp(FUNC_CameraYWriteHook7, (DWORD)CameraYWriteHook7, 6);
	//CMem::ApplyJmp(FUNC_CameraYWriteHook8, (DWORD)CameraYWriteHook8, 12);
	CMem::ApplyJmp(FUNC_CameraYWriteHook9, (DWORD)CameraYWriteHook9, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook10, (DWORD)CameraYWriteHook10, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook11, (DWORD)CameraYWriteHook11, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook12, (DWORD)CameraYWriteHook12, 10);
	CMem::ApplyJmp(FUNC_CameraYWriteHook13, (DWORD)CameraYWriteHook13, 10);
	CMem::ApplyJmp(FUNC_CameraYWriteHook14, (DWORD)CameraYWriteHook14, 10);
	CMem::ApplyJmp(FUNC_CameraYWriteHook15, (DWORD)CameraYWriteHook15, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook16, (DWORD)CameraYWriteHook16, 6);
	CMem::ApplyJmp(FUNC_CameraYWriteHook17, (DWORD)CameraYWriteHook17, 10);

	CMem::ApplyJmp(FUNC_CameraYAccessHook1, (DWORD)CameraYAccessHook1, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook2, (DWORD)CameraYAccessHook2, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook3, (DWORD)CameraYAccessHook3, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook4, (DWORD)CameraYAccessHook4, 8);
	CMem::ApplyJmp(FUNC_CameraYAccessHook5, (DWORD)CameraYAccessHook5, 8);
	CMem::ApplyJmp(FUNC_CameraYAccessHook6, (DWORD)CameraYAccessHook6, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook7, (DWORD)CameraYAccessHook7, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook8, (DWORD)CameraYAccessHook8, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook9, (DWORD)CameraYAccessHook9, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook10, (DWORD)CameraYAccessHook10, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook11, (DWORD)CameraYAccessHook11, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook12, (DWORD)CameraYAccessHook12, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook13, (DWORD)CameraYAccessHook13, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook14, (DWORD)CameraYAccessHook14, 8);
	CMem::ApplyJmp(FUNC_CameraYAccessHook15, (DWORD)CameraYAccessHook15, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook16, (DWORD)CameraYAccessHook16, 8);
	CMem::ApplyJmp(FUNC_CameraYAccessHook17, (DWORD)CameraYAccessHook17, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook18, (DWORD)CameraYAccessHook18, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook19, (DWORD)CameraYAccessHook19, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook20, (DWORD)CameraYAccessHook20, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook21, (DWORD)CameraYAccessHook21, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook22, (DWORD)CameraYAccessHook22, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook23, (DWORD)CameraYAccessHook23, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook24, (DWORD)CameraYAccessHook24, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook25, (DWORD)CameraYAccessHook25, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook26, (DWORD)CameraYAccessHook26, 8);
	CMem::ApplyJmp(FUNC_CameraYAccessHook27, (DWORD)CameraYAccessHook27, 10);
	//CMem::ApplyJmp(FUNC_CameraYAccessHook28, (DWORD)CameraYAccessHook28, 12);
	CMem::ApplyJmp(FUNC_CameraYAccessHook29, (DWORD)CameraYAccessHook29, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook30, (DWORD)CameraYAccessHook30, 6);
	CMem::ApplyJmp(FUNC_CameraYAccessHook31, (DWORD)CameraYAccessHook31, 6);

	// Fix for fatulous.exe health hack
	CMem::ApplyJmp(FUNC_Fatulous1, (DWORD)Fatulous1, 9);
	CMem::ApplyJmp(FUNC_Fatulous2, (DWORD)Fatulous2, 6);
	CMem::ApplyJmp(FUNC_Fatulous3, (DWORD)Fatulous3, 10);

	// Patch widescreen_lite.asi mod
	// Parts of it's source code are here: https://github.com/ThirteenAG/Widescreen_Fixes_Pack/tree/master/GTASA_widescreen_fix
	CMem::ApplyJmp(FUNC_WidescreenPatch, (DWORD)WidescreenPatch, 6);

	CMem::ApplyJmp(FUNC_Gravity, (DWORD)GravityHook, 6);

	// Make it so we can launch more than 1 gta_sa.exe (reversed addresses from http://ugbase.eu/releases-52/gtasa-multiprocess/)
	// This removes the necessity for samp_elevator.exe

	CMem::Cpy((void*)0x406946, "\x00\x00\x00\x00", 4);

	// Prevent modloader from working
	CMem::ApplyJmp(FUNC_WinMain, (DWORD)WinMainHook, 5);
	CMem::ApplyJmp(FUNC_rIdle, (DWORD)rIdleHook, 5);

	// Lets just make CLEO crash so extensions of it don't work.
	CMem::Cpy((void*)FUNC_CleoHook, "\x90\x90", 2);
	CMem::ApplyJmp(FUNC_CleoHook+2, (DWORD)CleoHook, 5);

	BYTE dest[5];
	CMem::Cpy(dest, (void*)FUNC_WallHack, 6);
	BYTE WallDefault[] = { 0x8B, 0x0D, 0xA8, 0x44, 0xB7, 0x00 };
	for (int i = 0; i < sizeof(WallDefault); ++i)
	{
		if (dest[i] != WallDefault[i])
		{
			CMem::Cpy((void*)FUNC_WallHack, "\x90\x90\x90\x90\x90", 5);
		}
	}

	CMem::Cpy(dest, (void*)FUNC_CRunningScript_AddScriptToList, 5);

	BYTE CleoDefault[] = { 0xE9, 0x8B, 0xCD, 0x0F, 0x01 };
	for (int i = 0; i < sizeof(CleoDefault); ++i)
	{
		if (dest[i] != CleoDefault[i])
		{
			CMem::Cpy((void*)FUNC_CRunningScript_AddScriptToList, "\x90\x90\x90\x90\x90", 5);
		}
	}

	// Check data file integrity.
	VerifyFilePaths();
}

void CHookManager::FindProtectedMemoryAddresses()
{
	DWORD dwSAMPBase = getSampBaseAddress();
	BYTE *addr;

	// these addresses are NOPped in order to disable LOS and other checks for nametags
	// many thanks to springfield: http://ugbase.eu/Thread-NameTagHack-0-3-7-CLEO-EXE-ASI
	
	addr = (BYTE*)(dwSAMPBase + 0x6FCF3);
	if (memcmp(addr, "\x0F\x84", 2) != 0)
		ExitProcess(0);
	else
		CMemProtect::Add(addr, NULL, 6);

	addr = (BYTE*)(dwSAMPBase + 0x6FD14);
	if (memcmp(addr, "\x0F\x8A", 2) != 0)
		ExitProcess(0);
	else
		CMemProtect::Add(addr, NULL, 6);
	
	addr = (BYTE*)(dwSAMPBase + 0x70E24);
	if (memcmp(addr, "\x0F\x8A", 2) != 0)
		ExitProcess(0);
	else
		CMemProtect::Add(addr, NULL, 6);

	addr = (BYTE*)(dwSAMPBase + 0x70F38);
	if (memcmp(addr, "\x74", 1) != 0)
		ExitProcess(0);
	else
		CMemProtect::Add(addr, NULL, 2);

	addr = (BYTE*)(dwSAMPBase + 0x6FE28);
	if (memcmp(addr, "\x74", 1) != 0)
		ExitProcess(0);
	else
		CMemProtect::Add(addr, NULL, 2);
}

void CHookManager::SetConnectPatches()
{
	// Detect when a player is pausing.
	CMem::ApplyJmp(FUNC_GamePaused, (DWORD)OnPause, 6);

	// Hack to make SA-MP think the game is always unpaused
	CMem::Cpy((void*)0x53E9B3, "\x75\x44\x90\x90\x90\x90", 6); // jne 0x53E9F9

	// Hack to make the game run in the background when paused
	CMem::PutSingle < BYTE >(0x561AF6, 0x00); // mov byte ptr [0xB7CB49],01 -> mov byte ptr [0xB7CB49],00

	if (Misc::GetGameVersion() == 1)
	{
		// Hack to make the game think we never alt tabbed when we have.
		CMem::Cpy((void*)0x748054, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
	} 
	else if (Misc::GetGameVersion() == 2)
	{
		// Hack to make the game think we never alt tabbed when we have.
		CMem::Cpy((void*)0x7480A4, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
	}

	// Hack so SA-MP continues to send packets while alt tabbed.
	CMem::Cpy((void*)0x53EA88, "\x90\x90\x90\x90\x90\x90", 6); // nop 

	// If you alt tab when you're in an interior, some weird graphics bugs happen
	// So fix that:
	CMem::Cpy((void*)0x53EA12, "\x90\x90\x90\x90\x90", 5);

	// Hack to make the game not set cursor position while alt tabbed
	CMem::ApplyJmp(FUNC_SetCursorPos, (DWORD)SetCursorPosHook, 8);

	// Hook key presses, this is an all key presses hook.
	CMem::ApplyJmp(FUNC_KeyPress, (DWORD)KeyPress, 8);

	// Disable changing of FOV. 
	// Source code to this mod: https://github.com/Whitetigerswt/samp-fov-changer
	CMem::ApplyJmp(FUNC_FOVPatch, (DWORD)FOVPatch, 5);

	// Hook markers in the game so we can control if there are vehicle blips or not.
	CMem::ApplyJmp(FUNC_Markers, (DWORD)MarkersHook, 6);

	// Prevent autoaim (Doesn't work anyway because of above camera X/Y hooks)
	CMem::Cpy((void*)0x686905, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x52A93C, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x521A16, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x5221FC, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x52A93C, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x686CE6, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop
	CMem::Cpy((void*)0x686C64, "\x66\xB8\x01\x00\x90", 5); // mov ax,1    nop

	// Fix some slide issues with melee weps
	CMem::ApplyJmp(FUNC_SlideFix, (DWORD)SlideFix, 6);

	if (Misc::GetGameVersion() == 1)
	{
		// Hook a main loading function of SAMP and GTA. 
		CMem::ApplyJmp(FUNC_MainLoad, (DWORD)MainLoading, 5);
	}
	else if (Misc::GetGameVersion() == 2)
	{
		CMem::ApplyJmp(FUNC_MainLoadAlt, (DWORD)MainLoading, 5);
		MainLoadingJmpBack = 0x7F9B52;
	}

}

void CHookManager::ToggleSprintOnAllSurfaces(bool toggle)
{
	DWORD dwOldProt;

	// Unprotect memory.
	VirtualProtect((void*)0x55E870, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProt);
	VirtualProtect((void*)0x55E874, sizeof(WORD), PAGE_EXECUTE_READWRITE, &dwOldProt);

	if (toggle)
	{
		// Allow sprint on any surfaces.
		CMem::PutSingle < DWORD >(0x55E870, 0xC2C03366); 
		CMem::PutSingle < WORD >(0x55E874, 0x0004); 
	}
	else
	{
		// Restore default values to these addresses
		CMem::PutSingle < DWORD >(0x55E870, 0x424448B);
		CMem::PutSingle < WORD >(0x55E874, 0x48D);
	}

}

void CHookManager::SetFrameLimiterPatch()
{
	DWORD dwOldProt;

	// When a player trys to turn on their frame limiter, this is the address that sets the frame limiter on, NOP that.
	VirtualProtect(FUNC_FrameLimiter, 3, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(FUNC_FrameLimiter, "\x90\x90\x90", 3);

	// And set the frame limiter off.
	FRAME_LIMITER = 0;
}

void CHookManager::VerifyFilePaths()
{
	// These addresses tell gta_sa where to find the data files
	// We need to verify they're the same so the game doesn't try to cheat and load the wrong files ;)

	// I question if this can be done in a better way.
	// e.g: Writing asm hooks and changing the address gta_sa.exe reads for these strings to something in this .asi.

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
DWORD MainLoadingAddress = NULL;
HOOK CHookManager::MainLoading()
{

	__asm
	{

		mov edx, [eax]
		pushad
		add edx,44h
		mov eax,[edx]
		mov MainLoadingAddress, eax
	}

	if (*(BYTE*)(MainLoadingAddress) != 0xE9)
	{
		__asm
		{
			popad
			call dword ptr[edx + 44h]
			jmp[MainLoadingJmpBack]
		}
	}

	// cause a crash
	__asm
	{
		ret
	}
}

HOOK CHookManager::SlideFix()
{
	__asm
	{
		mov eax, [esi + 00000480h]
		pushad
	}

	if (VAR_CURRENT_WEAPON <= 15)
	{
		__asm
		{
			popad
			jmp[SlideFixJmpBack]
		}
	}

	__asm
	{
		popad
		or dword ptr[eax + 34h], 08h
		jmp [SlideFixJmpBack]
	}
}

bool isPaused = false;
void CHookManager::OnPauseChange()
{
	isPaused = !isPaused;

	RakNet::BitStream bsData;

	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(TOGGLE_PAUSE);
	bsData.Write(0);
	bsData.Write(isPaused);

	CRakClientHandler::CustomSend(&bsData);
}

#pragma warning(disable:4731)
void CHookManager::OnPause()
{

	__asm pushad

	OnPauseChange();

	__asm
	{
		popad
		mov cl, [esi + 5Ch]
		mov al, [esi + 32h]
		pop edi
		pop esi
		pop ebx
		pop ebp
		jmp[PauseJmpBack]
	}
}

void LoadRakClient()
{
	boost::thread thread(&CRakClientHandler::Load);
}

HOOK CHookManager::GetSampInfo()
{
	__asm
	{
		pushad
		mov g_SAMP, esi
		call LoadRakClient
	}
	// remove hook now that we got the address.
	CMem::Cpy((void*)sampInfoAddr, "\x8B\x86\xCD\x03\x00\x00", 6);
	__asm
	{
		popad
		jmp[sampInfoAddr]
	}
}

RakPeer__SendBuffered_t CHookManager::m_pfnRakPeer__SendBuffered = NULL;
void __thiscall CHookManager::RakPeer__SendBufferedHook(void *_this, const char *data, int numberOfBitsToSend, PacketPriority priority, PacketReliability reliability, char orderingChannel, PlayerID playerId, bool broadcast, /*RemoteSystemStruct::ConnectMode*/ int connectionMode)
{
	if (!CPacketIntegrity::Check(data, numberOfBitsToSend))
		return;

	m_pfnRakPeer__SendBuffered(_this, data, numberOfBitsToSend, priority, reliability, orderingChannel, playerId, broadcast, connectionMode);
}

HOOK CHookManager::GravityHook()
{
	__asm
	{
		pushad
	}

	// Check if a sobeit hook is installed.
	if (*(BYTE*)0x543081 != 0xD9)
	{
		// Crash the client.

		VirtualProtect((void*)0x543081, 5, PAGE_EXECUTE_READWRITE, NULL);
		memcpy((void*)0x543081, "\x90\x90\x90\x90\x90", 5);
	}

	__asm
	{
		popad

		test byte ptr[esi + 1Ch], 01h
		je DONE_EQUAL
		jmp[GravityHookJmpBack1]

		DONE_EQUAL:
		jmp[GravityHookJmpBack2]
	}
}


DWORD gtasa_markers_jmp_pointer = 0x584A79;
HOOK CHookManager::MarkersHook()
{
	__asm pushad

	if (Misc::GetVehicleBlips() == false)
	{
		__asm
		{
			popad
			mov eax, [esp + 04h]
			cmp eax, 000003ECh
			je return_hook
			mov eax, gtasa_markers_jmp_pointer
			jmp dword ptr[eax]


			return_hook:
				xor eax,eax
				ret
		}
	}
	else
	{
		__asm
		{
			popad
			mov eax, gtasa_markers_jmp_pointer
			jmp dword ptr[eax]
		}
	}
}

HOOK CHookManager::WinMainHook()
{
	__asm
	{
		push esi
		push esi
		call edi
		push eax
		mov eax,00748710h
		call eax

		jmp[WinMainHookJmpBack]
	}
}

HOOK CHookManager::rIdleHook()
{
	__asm
	{
		mov edx, [esp + 08h]
		push edx
		mov eax, 0053E920h
		call eax

		jmp[rIdleHookJmpBack]
	}
}

HOOK CHookManager::CleoHook()
{
	__asm
	{
		push eax
		mov eax,00469F00h
		call eax
		pop eax
		cmp edi,ebx
		jmp[CLEO_Func_JmpBack]
	}
}
HOOK CHookManager::SetCursorPosHook()
{

	__asm pushad


	if (Misc::GetAltTabState())
	{
		__asm
		{
			popad
			jmp[SetCursorPosHookJmpBack]
		}
	}

	__asm
	{
		popad
		push eax // y
		push ecx // x
		call SetCursorPos
		jmp[SetCursorPosHookJmpBack]
	}
}

DWORD Fatulous1AlternativeJmpBack = 0x6D827E;
HOOK CHookManager::Fatulous1()
{
	__asm
	{
		test ah, 41h
		jne jne_not_equal
		jmp [Fatulous1JmpBack]

		jne_not_equal:
		jmp [Fatulous1AlternativeJmpBack]
	}
}

HOOK CHookManager::Fatulous2()
{
	__asm
	{
		fld dword ptr[esi + 0x540]
		jmp [Fatulous2JmpBack]
	}
}

HOOK CHookManager::Fatulous3()
{
	__asm
	{
		mov eax, [esp + 04h]
		fld dword ptr[eax + 540h]
		jmp[Fatulous3JmpBack]
	}
}

DWORD KeyPressCall = 0x53EF80;
DWORD dwLastCrouch = 0;
HOOK CHookManager::KeyPress()
{
	__asm
	{
		pushad
	}

	if (PLAYER_POINTER == 0 || VAR_CPED_PLAYER_STATE == 0)
	{
		__asm
		{
			popad

			call[KeyPressCall]
			lea ecx, [ebx + 78h]
			jmp[KeyPressJmpBack]
		}
	}

	// Check if the sprint key is pressed & we're on foot, and it wasn't pressed in the last frame.
	if (SPRINT_KEY != 0 && Misc::GetSprintLimit() != 0.0)
	{
		// If not in a vehicle and macro locks is off.
		if (VAR_CURRENT_VEHICLE == 0 && Misc::GetMacroLocks() == true)
		{
			// Set the player's sprint speed to whatever the limit is
			VAR_SPRINT_SPEED = Misc::GetSprintLimit();
		}
	}
	// Sprint key is NOT pressed
	/*else
	{
		// And aim key is pressed
		if (AIM_KEY != 0 || FIRE_KEY != 0)
		{
			// Set sprint to 0
			VAR_SPRINT_SPEED = 0.0f;
		}
	}*/

	/*if (SPRINT_KEY > 0)
	{
		// fixes: [00:02] [U]27: you have to shoot, then hold all keys down afterwards at the same time (space, C, a, s, d, w (w/e direction) and aim (rmb)
		// slide issue
		if (VAR_CURRENT_WEAPON >= 23 && VAR_CURRENT_WEAPON <= 33 && VAR_CPED_STATE != 102 && VAR_CPED_STATE != 205 && AIM_KEY > 0)
		{
			AIM_KEY = 0;
		}
	}*/

	// Fixes switch weapon desync while sprinting/running
	/*if ((VAR_CPED_STATE == 102 || VAR_CPED_STATE == 205) && AIM_KEY == 0 && VAR_CURRENT_WEAPON > 15)
	{
		if (FIRE_KEY > 0)
		{
			AIM_KEY = 128;
			SPRINT_KEY = 0;
		}
	}*/

	// Check if the crouch key is pressed.
	if (CROUCH_KEY > 0)
	{
		// Check the tick variable since the last shot.
		if (VAR_SHOT_TICK > Misc::GetCrouchBug())
		{
			// Unset the crouch key.
			CROUCH_KEY = 0;
		}

		// Disable floor bug, checks if the player is punching
		if (VAR_SOMETHING_STATE == 77000016)
		{
			CROUCH_KEY = 0;
		}

		dwLastCrouch = GetTickCount() + 250;
	}

	// fix slide bug.
	/*if (VAR_ANIMATION_SOMETHING == 1069547520 && VAR_AIMING == 53 && (VAR_CPED_STATE == 102 || VAR_CPED_STATE == 205) && VAR_CURRENT_WEAPON > 15)
	{
		AIM_KEY = 0;
	}*/

	// Fix weird client side slide 
	// causes to many weird issues, probably because i have no idea what VAR_ANIMATION_SOMETHING_ELSE or VAR_ANIMATION_SOMETHING actually are
	/*if (VAR_AIMING == 53 && VAR_ANIMATION_SOMETHING_ELSE == 0 && VAR_ANIMATION_SOMETHING == 1074301065)
	{
		VAR_ANIMATION_SOMETHING = 0;
		//AIM_KEY = 0;
	}*/

	// Prevent jumping while alt tabbed
	if (Misc::GetAltTabState())
	{
		JUMP_KEY = 0;
	}

	__asm
	{
		popad

		call[KeyPressCall]
		lea ecx, [ebx + 78h]
		jmp[KeyPressJmpBack]
	}
}

/*DWORD ecx_copy = 0;
DWORD CPed_Special_flags_Alt = 0x5448FE;
HOOK CHookManager::CPed_Special_Flags()
{
	__asm
	{
		mov ecx_copy, ecx
		pushad
	}

	if (*(BYTE*)(ecx_copy + 64) == VAR_CPED_SPECIAL_FLAGS)
	{
		__asm
		{
			popad
			test dword ptr[ecx + 40h], 40000h
			jmp[CPed_Special_FlagsJmpBack]
		}
	}
	else
	{
		__asm
		{
			popad
			test dword ptr[ecx + 40h], 40000h
			jne jne_not_equal
			jmp[CPed_Special_FlagsJmpBack]

		jne_not_equal:
			jmp[CPed_Special_flags_Alt]
		}
	}
}*/

HOOK CHookManager::FOVPatch()
{
	__asm
	{
		fnstsw ax
		test ah, 05h
		pushad
	}

	*(float*)0x0858CE0 = 70.0f;
	*(float*)0x0B6F250 = 70.0f;

	__asm
	{
		popad
		jmp[FOVPatchJmpBack]
	}
}

HOOK CHookManager::WidescreenPatch()
{
	__asm
	{
		push eax
		mov eax, 0x859524
		fmul dword ptr[eax]
		pop eax
		jmp[WidescreenJmpBack]
	}
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

void CHookManager::CheckFireKeyState(const char *funcname)
{
	if (FireKeyState != FIRE_KEY && FireKeyState == 0)
	{
		CLog("trigger.log").Write("%s: expected: %d, found: %d", funcname, FireKeyState, (DWORD)FIRE_KEY);
	}
}

void CHookManager::SaveFireKeyState()
{
	FireKeyState = FIRE_KEY;
}

__declspec(naked) void CHookManager::ClearKeyState_Hook() // this function has been entirely moved inside AC module
{
	__asm {
		push ecx // save _this pointer
	}

	CheckFireKeyState(__FUNCTION__);

	__asm {
		pop ecx // restore _this pointer

		xor eax, eax
		mov[ecx + 0x06], ax
		mov[ecx + 0x04], ax
		mov[ecx + 0x02], ax
		mov[ecx + 0x00], ax
		mov[ecx + 0x0E], ax
		mov[ecx + 0x0C], ax
		mov[ecx + 0x0A], ax
		mov[ecx + 0x08], ax
		mov[ecx + 0x16], ax
		mov[ecx + 0x14], ax
		mov[ecx + 0x12], ax
		mov[ecx + 0x10], ax
		mov[ecx + 0x1A], ax
		mov[ecx + 0x18], ax
		mov[ecx + 0x22], ax
		mov[ecx + 0x20], ax
		mov[ecx + 0x1E], ax
		mov[ecx + 0x1C], ax
		mov[ecx + 0x26], ax
		mov[ecx + 0x24], ax
		mov[ecx + 0x28], ax
		mov[ecx + 0x2A], ax
		mov[ecx + 0x2C], ax
		mov[ecx + 0x2E], ax
	}

	SaveFireKeyState();

	__asm {
		ret
	}
}

DWORD ProcessKeyboard1_JumpBack = 0x541C78;
__declspec(naked) void CHookManager::ProcessKeyboard1_Hook()
{
	CheckFireKeyState(__FUNCTION__);

	__asm {
		mov ecx, 0xC
		mov edi, ebx
		repe movsd
	}

	SaveFireKeyState();

	__asm {
		jmp[ProcessKeyboard1_JumpBack]
	}
}

DWORD ProcessKeyboard2_JumpBack = 0x541C96;
__declspec(naked) void CHookManager::ProcessKeyboard2_Hook()
{
	CheckFireKeyState(__FUNCTION__);

	__asm {
		mov ecx, 0xC
		mov edi, ebx
		repe movsd
	}

	SaveFireKeyState();

	__asm {
		jmp[ProcessKeyboard2_JumpBack]
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
		fadd[CameraXPos]
		fstp[CameraXPos]
		fadd[CameraYPos]
		fstp[CameraYPos]
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

HOOK CHookManager::CameraXWriteHook8()
{
	__asm
	{
		fst dword ptr[CameraXPos]
		jmp[CameraXHook8JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook9()
{
	__asm
	{
		fstp dword ptr[CameraXPos]
		jmp[CameraXHook9JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook10()
{
	__asm
	{
		mov[CameraXPos], edx
		jmp[CameraXHook10JmpBack]
	}
}

HOOK CHookManager::CameraXWriteHook11()
{
	__asm
	{
		fstp dword ptr[CameraXPos]
		jmp[CameraXHook11JmpBack]
	}
}

DWORD RandomCall = 0x509C50;
HOOK CHookManager::CameraXWriteHook12()
{
	__asm
	{
		fadd dword ptr[CameraXPos]
		fstp dword ptr[CameraXPos]

		fadd dword ptr[CameraYPos]
		fstp dword ptr[CameraYPos]

		call [RandomCall]

		fld dword ptr[CameraYPos]
		jmp[CameraXHook12JmpBack]
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
		mov ecx, [CameraYPos]
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
		fld dword ptr[CameraYPos]
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
		fld[CameraYPos]
		push 0
		fcos
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

HOOK CHookManager::CameraXAccessHook21()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess21JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook22()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess22JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook23()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess23JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook24()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess24JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook25()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess25JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook26()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess26JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook27()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess27JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook28()
{
	__asm
	{
		fsub dword ptr[CameraXPos]
		jmp[CameraXAccess28JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook29()
{
	__asm
	{
		fadd dword ptr[CameraXPos]
		fstp dword ptr[CameraXPos]
		jmp[CameraXAccess29JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook30()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess30JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook31()
{
	__asm
	{
		fsub dword ptr[CameraXPos]
		jmp[CameraXAccess31JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook32()
{
	__asm
	{
		mov eax, [CameraXPos]
		jmp[CameraXAccess32JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook33()
{
	__asm
	{
		fld dword ptr [CameraXPos]
		jmp[CameraXAccess33JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook34()
{
	__asm
	{
		fsubr dword ptr[CameraXPos]
		jmp[CameraXAccess34JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook35()
{
	__asm
	{
		fsubr dword ptr[CameraXPos]
		jmp[CameraXAccess35JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook36()
{
	__asm
	{
		fsubr dword ptr[CameraXPos]
		jmp[CameraXAccess36JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook37()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess37JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook38()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess38JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook39()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess39JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook40()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		fcos
		fmul st(0), st(1)
		fchs
		fstp dword ptr[esp + 54h]
		mov ebx, [esp + 54h]
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess40JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook41()
{
	__asm
	{
		mov ecx, [CameraXPos]
		jmp[CameraXAccess41JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook42()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess42JmpBack]
	}
}

HOOK CHookManager::CameraXAccessHook43()
{
	__asm
	{
		fld dword ptr[CameraXPos]
		jmp[CameraXAccess43JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook2()
{
	__asm
	{
		fadd dword ptr[CameraYPos]
		fstp dword ptr[CameraYPos]
		fld dword ptr[CameraYPos]
		jmp[CameraYWrite2JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook3()
{
	__asm
	{
		fstp[CameraYPos]
		mov[edi + 0x0000B0], ecx
		jmp[CameraYWrite3JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook4()
{
	__asm
	{
		mov edx, [esp + 4Ch]
		mov[CameraYPos], edx
		jmp[CameraYWrite4JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook5()
{
	__asm
	{
		fadd dword ptr[CameraYPos]
		fstp dword ptr[CameraYPos]
		jmp[CameraYWrite5JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook6()
{
	__asm
	{
		fadd dword ptr[CameraYPos]
		fstp dword ptr[CameraYPos]
		jmp[CameraYWrite6JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook7()
{
	__asm
	{
		mov[CameraYPos], esi
		jmp[CameraYWrite7JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook9()
{
	__asm
	{
		fstp dword ptr[CameraYPos]
		jmp[CameraYWrite9JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook10()
{
	__asm
	{
		mov[CameraYPos],ecx
		jmp[CameraYWrite10JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook11()
{
	__asm
	{
		fstp dword ptr[CameraYPos]
		jmp[CameraYWrite11JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook12()
{
	__asm
	{
		mov[CameraYPos], 0xBFC7F1E7
		jmp[CameraYWrite12JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook13()
{
	__asm
	{
		mov[CameraYPos], 0x3F860A92
		jmp[CameraYWrite13JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook14()
{
	__asm
	{
		mov[CameraYPos], 0x3F860A92
		jmp[CameraYWrite14JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook15()
{
	__asm
	{
		mov[CameraYPos],eax
		jmp[CameraYWrite15JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook16()
{
	__asm
	{
		fstp dword ptr[CameraYPos]
		jmp[CameraYWrite16JmpBack]
	}
}

HOOK CHookManager::CameraYWriteHook17()
{
	__asm
	{
		mov[CameraYPos], 0xBFBF0243
		jmp[CameraYWrite17JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook1()
{
	__asm
	{
		fld dword ptr[esp + 18h]
		fsub dword ptr[CameraYPos]
		jmp[CameraYAccess1JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook2()
{
	__asm
	{
		fadd dword ptr[CameraYPos]
		fstp dword ptr[CameraYPos]
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess2JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook3()
{
	__asm
	{
		fchs
		fcom dword ptr[CameraYPos]
		fnstsw ax
		jmp[CameraYAccess3JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook4()
{
	__asm
	{
		push eax 
		mov eax, 0x8CCE74
		fld dword ptr[eax]
		pop eax
		fsub[CameraYPos]
		jmp[CameraYAccess4JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook5()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		fsin
		fstp dword ptr[esp + 7Ch]
		jmp[CameraYAccess5JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook6()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess6JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook7()
{
	__asm
	{
		fcom dword ptr[CameraYPos]
		jmp[CameraYAccess7JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook8()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess8JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook9()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess9JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook10()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess10JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook11()
{
	__asm
	{
		fadd dword ptr[CameraYPos]
		jmp[CameraYAccess11JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook12()
{
	__asm
	{
		fmul dword ptr[CameraYPos]
		jmp[CameraYAccess12JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook13()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess13JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook14()
{
	__asm
	{
		push eax
		mov eax, 0x858C28
		fsub dword ptr[eax]
		pop eax
		fcomp dword ptr[CameraYPos]
		jmp[CameraYAccess14JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook15()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		fcomp dword ptr[esp + 48h]
		jmp[CameraYAccess15JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook16()
{
	__asm
	{
		push eax
		mov eax, 0x858C28
		fadd dword ptr[eax]
		pop eax
		fcomp dword ptr[CameraYPos]
		jmp[CameraYAccess16JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook17()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess17JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook18()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess18JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook19()
{
	__asm
	{
		fcom dword ptr[CameraYPos]
		jmp[CameraYAccess19JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook20()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess20JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook21()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess21JmpBack]
	}
}
HOOK CHookManager::CameraYAccessHook22()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess22JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook23()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess23JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook24()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess24JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook25()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		fcomp dword ptr[esp + 48h]
		jmp[CameraYAccess25JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook26()
{
	__asm
	{
		fsub dword ptr[CameraYPos]
		mov[edi + 0x0000B0], ecx
		jmp[CameraYAccess26JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook27()
{
	__asm
	{
		fsub dword ptr[CameraYPos]
		xor ecx,ecx
		mov[edi + 0x0000B0], ecx
		jmp[CameraYAccess27JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook28()
{
	__asm
	{
		fstp dword ptr[CameraYPos]
		fld dword ptr[CameraYPos]

		jmp[CameraYAccess28JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook29()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess29JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook30()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess30JmpBack]
	}
}

HOOK CHookManager::CameraYAccessHook31()
{
	__asm
	{
		fld dword ptr[CameraYPos]
		jmp[CameraYAccess31JmpBack]
	}
}
