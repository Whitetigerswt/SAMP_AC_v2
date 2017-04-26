#pragma once

#include "Addresses.h"

// CLEO HOOKS //
#define FUNC_CRunningScript_AddScriptToList (void*)0x464C00
#define FUNC_Init_SCM1						(void*)0x53BDD7
#define FUNC_Init_SCM2						(void*)0x5BA340

// WERNER HOOKS //
#define FUNC_ShotgunBullet					(BYTE*)0x73FC5F 
#define FUNC_DeagleBullet					(BYTE*)0x73FC04 

// OTHER HEALTH HOOKS //
//#define FUNC_CPed_Special_Flags				(BYTE*)0x5448E0
#define FUNC_Fatulous1						(BYTE*)0x6D802A
#define FUNC_Fatulous2						(BYTE*)0x4B330E
#define FUNC_Fatulous3						(BYTE*)0x64EEE0

// WIDESCREEN MOD HOOK //
#define FUNC_WidescreenPatch				(BYTE*)0x5894AF

// FOV MOD HOOK //
#define FUNC_FOVPatch						(BYTE*)0x522F63

// Key press PATCH - DETECT KEY PRESSING //
#define FUNC_KeyPress						(BYTE*)0x541C98

// Sprint hook - limit sprint to a certain number
#define FUNC_SprintHook						(BYTE*)0x60A757

// FRAME LIMITER PATCH //
#define FUNC_FrameLimiter					(void*)0x57CED1

// LITE FOOT PATCH //
#define FUNC_LiteFoot						(BYTE*)0x60A740

// GRAVITY PATCH - ANTI SOBEIT
#define FUNC_Gravity						(BYTE*)0x54307B

// MARKERS HOOK.
#define FUNC_Markers						(BYTE*)0x584770

// PAUSE PATCH - DETECT WHEN THE GAME IS PAUSED
#define FUNC_GamePaused						(BYTE*)0x576C27

#define FUNC_SetCursorPos					(BYTE*)0x74542B

// slide fix patch
#define FUNC_SlideFix						(BYTE*)0x686C2F

#define FUNC_WallHack						(BYTE*)0x61A5B0

// ModLoader patches
#define FUNC_WinMain						(BYTE*)0x8246E7
#define FUNC_rIdle							(BYTE*)0x53ECB8
#define FUNC_rfIdle							(BYTE*)0x53ECCB

// More CLEO patches
#define FUNC_CleoHook						(BYTE*)0x46A21B

// Main loading patch
#define FUNC_MainLoad						(BYTE*)0x7F9B0D
#define FUNC_MainLoadAlt					(BYTE*)0x7F9B4D

// Triggerbot hooks //
#define FUNC_CPad__ProcessKeyboard1_Hook	(BYTE*)0x541C6F
#define FUNC_CPad__ProcessKeyboard2_Hook	(BYTE*)0x541C8D
#define FUNC_CPad__Clear_Hook				(BYTE*)0x53EF80

// CAMERA X WRITE HOOKS //
#define FUNC_CameraXWriteHook1				(void*)0x5241C2
#define FUNC_CameraXWriteHook2				(BYTE*)0x523F91
#define FUNC_CameraXWriteHook3				(BYTE*)0x510D66

// WHILE AIMING CAMERA X WRITE HOOKS //
#define FUNC_CameraXWriteHook4				(BYTE*)0x50E56D
#define FUNC_CameraXWriteHook5				(BYTE*)0x51566E
#define FUNC_CameraXWriteHook6				(BYTE*)0x5225C7
#define FUNC_CameraXWriteHook7				(BYTE*)0x5222B0

// WHILE DRIVING CAMERA X HOOKS //
#define FUNC_CameraXWriteHook8				(BYTE*)0x524C4B
#define FUNC_CameraXWriteHook9				(BYTE*)0x525906
#define FUNC_CameraXWriteHook10				(BYTE*)0x525B08
#define FUNC_CameraXWriteHook11				(BYTE*)0x509C6F
#define FUNC_CameraXWriteHook12				(BYTE*)0x511D98

// CAMERA X ACCESS HOOKS //

#define FUNC_CameraXAccessHook1				(BYTE*)0x5237DD
#define FUNC_CameraXAccessHook2				(BYTE*)0x5237FC
#define FUNC_CameraXAccessHook3				(BYTE*)0x52383E
#define FUNC_CameraXAccessHook4				(BYTE*)0x5241AB
#define FUNC_CameraXAccessHook5				(BYTE*)0x5241CC
#define FUNC_CameraXAccessHook6				(BYTE*)0x5241D7
#define FUNC_CameraXAccessHook7				(BYTE*)0x5241ED
#define FUNC_CameraXAccessHook8				(BYTE*)0x5242C9
#define FUNC_CameraXAccessHook9				(BYTE*)0x5242D7
#define FUNC_CameraXAccessHook10			(BYTE*)0x509C50
#define FUNC_CameraXAccessHook11			(BYTE*)0x509C83
#define FUNC_CameraXAccessHook12			(BYTE*)0x5231CA
#define FUNC_CameraXAccessHook13			(BYTE*)0x5232B3
#define FUNC_CameraXAccessHook14			(BYTE*)0x5232C7

// WHILE AIMING CAMERA X ACCESS HOOKS //

#define FUNC_CameraXAccessHook15			(BYTE*)0x52A898
#define FUNC_CameraXAccessHook16			(BYTE*)0x52A8B7
#define FUNC_CameraXAccessHook17			(BYTE*)0x52A8D4
#define FUNC_CameraXAccessHook18			(BYTE*)0x51560D
#define FUNC_CameraXAccessHook19			(BYTE*)0x522973
#define FUNC_CameraXAccessHook20			(BYTE*)0x522991
#define FUNC_CameraXAccessHook21			(BYTE*)0x5117BB
#define FUNC_CameraXAccessHook22			(BYTE*)0x5117CF
#define FUNC_CameraXAccessHook23			(BYTE*)0x5118CB
#define FUNC_CameraXAccessHook24			(BYTE*)0x511993
#define FUNC_CameraXAccessHook25			(BYTE*)0x520B78
#define FUNC_CameraXAccessHook26			(BYTE*)0x520B94

// WHILE DRIVING CAMERA X ACCESS HOOKS //

#define FUNC_CameraXAccessHook27			(BYTE*)0x524F9B
#define FUNC_CameraXAccessHook28			(BYTE*)0x524FFA
#define FUNC_CameraXAccessHook29			(BYTE*)0x525900
#define FUNC_CameraXAccessHook30			(BYTE*)0x524FBA
#define FUNC_CameraXAccessHook31			(BYTE*)0x525AE7
#define FUNC_CameraXAccessHook32			(BYTE*)0x525B14
#define FUNC_CameraXAccessHook33			(BYTE*)0x525B31
#define FUNC_CameraXAccessHook34			(BYTE*)0x5251F2
#define FUNC_CameraXAccessHook35			(BYTE*)0x525290
#define FUNC_CameraXAccessHook36			(BYTE*)0x525326
#define FUNC_CameraXAccessHook37			(BYTE*)0x525B45
#define FUNC_CameraXAccessHook38			(BYTE*)0x525C56
#define FUNC_CameraXAccessHook39			(BYTE*)0x525C6B
#define FUNC_CameraXAccessHook40			(BYTE*)0x524C90
#define FUNC_CameraXAccessHook41			(BYTE*)0x511B90
#define FUNC_CameraXAccessHook42			(BYTE*)0x511E41
#define FUNC_CameraXAccessHook43			(BYTE*)0x511E55

// CAMERA Y HOOKS //

#define FUNC_CameraYWriteHook1				(void*)0x52419C
#define FUNC_CameraYWriteHook2				(BYTE*)0x52414D
#define FUNC_CameraYWriteHook3				(BYTE*)0x52417B
#define FUNC_CameraYWriteHook4				(BYTE*)0x52415E
#define FUNC_CameraYWriteHook5				(BYTE*)0x5225D7
#define FUNC_CameraYWriteHook6				(BYTE*)0x5222BC
#define FUNC_CameraYWriteHook7				(BYTE*)0x523233
//#define FUNC_CameraYWriteHook8				(BYTE*)0x52329D
#define FUNC_CameraYWriteHook9				(BYTE*)0x5233C6
#define FUNC_CameraYWriteHook10				(BYTE*)0x522607
#define FUNC_CameraYWriteHook11				(BYTE*)0x52290B
#define FUNC_CameraYWriteHook12 			(BYTE*)0x511E27
#define FUNC_CameraYWriteHook13				(BYTE*)0x511DC8
#define FUNC_CameraYWriteHook14				(BYTE*)0x511573
#define FUNC_CameraYWriteHook15				(BYTE*)0x5117A5
#define FUNC_CameraYWriteHook16				(BYTE*)0x511783
#define FUNC_CameraYWriteHook17				(BYTE*)0x511592

#define FUNC_CameraYAccessHook1				(BYTE*)0x523BA1
#define FUNC_CameraYAccessHook2				(BYTE*)0x52414D
#define FUNC_CameraYAccessHook3				(BYTE*)0x524170
#define FUNC_CameraYAccessHook4				(BYTE*)0x524187
#define FUNC_CameraYAccessHook5				(BYTE*)0x524204
#define FUNC_CameraYAccessHook6				(BYTE*)0x5225EA
#define FUNC_CameraYAccessHook7				(BYTE*)0x5228FE
#define FUNC_CameraYAccessHook8				(BYTE*)0x52291B
#define FUNC_CameraYAccessHook9				(BYTE*)0x522955
#define FUNC_CameraYAccessHook10			(BYTE*)0x5229AE
#define FUNC_CameraYAccessHook11			(BYTE*)0x50AD85
#define FUNC_CameraYAccessHook12			(BYTE*)0x522934
#define FUNC_CameraYAccessHook13			(BYTE*)0x5232E2
#define FUNC_CameraYAccessHook14			(BYTE*)0x52409A
#define FUNC_CameraYAccessHook15			(BYTE*)0x5240C2
#define FUNC_CameraYAccessHook16			(BYTE*)0x5240E7
#define FUNC_CameraYAccessHook17			(BYTE*)0x511560
#define FUNC_CameraYAccessHook18			(BYTE*)0x51157F
#define FUNC_CameraYAccessHook19			(BYTE*)0x511776
#define FUNC_CameraYAccessHook20			(BYTE*)0x51178D
#define FUNC_CameraYAccessHook21			(BYTE*)0x5117AB
#define FUNC_CameraYAccessHook22			(BYTE*)0x5117EA
#define FUNC_CameraYAccessHook23			(BYTE*)0x5118DB
#define FUNC_CameraYAccessHook24			(BYTE*)0x5119A3
#define FUNC_CameraYAccessHook25			(BYTE*)0x52410C
#define FUNC_CameraYAccessHook26			(BYTE*)0x52411F
#define FUNC_CameraYAccessHook27			(BYTE*)0x5240D3
#define FUNC_CameraYAccessHook28			(BYTE*)0x52329D
#define FUNC_CameraYAccessHook29			(BYTE*)0x511E14
#define FUNC_CameraYAccessHook30			(BYTE*)0x511E31
#define FUNC_CameraYAccessHook31			(BYTE*)0x511E70

// CLEO HOOKS //

#define FUNC_Scripts_Init1					0x53BDE1
#define FUNC_Scripts_Init2					0x5BA345

// GTA:SA SINGLE PLAYER CHEAT HOOKS //

#define VAR_INF_AMMO						(void*)0x7428A6
#define VAR_INF_OXYGEN						(void*)0x60A8E2
#define VAR_INF_HP							(void*)0x4B322C

// GTA:SA PATH CHECKS //

#define VAR_ANIM_IMG_PATH					(void*)0x85C718
#define VAR_WEAPON_DAT_PATH					(void*)0x86AA28
#define VAR_CARMODS_DAT_PATH				(void*)0x869668
#define VAR_ANIMGRP_DAT_PATH				(void*)0x86A7F4
#define VAR_MELEE_DAT_PATH					(void*)0x86AAB4
#define VAR_CLOTHES_DAT_PATH				(void*)0x8671F8
#define VAR_OBJECT_DAT_PATH					(void*)0x869B20
#define VAR_DEFAULT_DAT_PATH				(void*)0x863A90
#define VAR_SURFACE_DAT_PATH				(void*)0x864318
#define VAR_GTA_DAT_PATH					(void*)0x863B10
#define VAR_WATER_DAT_PATH					(void*)0x872148
#define VAR_WATER1_DAT_PATH					(void*)0x872158
#define VAR_FURNITUR_DAT_PATH				(void*)0x86AF80
#define VAR_PROCOBJ_DAT_PATH				(void*)0x867014
#define VAR_HANDLING_CFG_PATH				(void*)0x86A964
#define VAR_PEDSTATS_DAT_PATH				(void*)0x86A698
#define VAR_FONTS_TXD_PATH					(void*)0x86A648
#define VAR_PEDS_COLL_PATH					(void*)0x86A51C
#define VAR_STATDISP_PATH					(void*)0x863F80
#define VAR_ARSTATS_DAT_PATH				(void*)0x863FA0
#define VAR_SURFINFO_DAT_PATH				(void*)0x864DB4

// RakNet hooks:
#define FUNC_OFFSET_RAKPEER__SENDBUFFERED	0x37490