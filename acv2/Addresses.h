#pragma once

#define ADDRESS_LOADED							*(int*)0xC8D4C0
#define GET_HWND								*(HWND*)0xC97C1C
#define PLAYER_POINTER							*(int*)0xB6F5F0
#define DIRECT_D3D9								*(IDirect3D9**)0xC97C20
#define DIRECT_DEVICE							*(IDirect3DDevice9**)0xC97C28
#define DIRECT_PARAMETERS						*( D3DPRESENT_PARAMETERS**)0xC9C040
#define SWITCH_RELOAD							(void*)0x060B4FA
#define FRAME_LIMITER							*(BYTE*)0xBA6794
#define RESOLUTION_X							*(int*)0xC9C040
#define RESOLUTION_Y							*(int*)0xC9C044
#define CURRENT_HWND							*(HWND*)0xC97C1C
#define SPRINT_KEY								*(BYTE*)0xB73478 // sprint AND zoom out key, apparently...
#define CROUCH_KEY								*(BYTE*)0xB7347C
#define AIM_KEY									*(BYTE*)0xB73464
#define FIRE_KEY								*(BYTE*)0xB7347A
#define ENTER_CAR_KEY							*(BYTE*)0xB73476
#define JUMP_KEY								*(BYTE*)0xB73474
#define WEAPON_SWITCH_FORWARD					*(BYTE*)0xB73496
#define WEAPON_SWITCH_BACKWARDS					*(BYTE*)0xB73492
#define DIRECT_CREATE_US						(BYTE*)0x7F6312
#define DIRECT_CREATE_EU						(BYTE*)0x7F6352

#define VAR_CURRENT_INTERIOR					*(int*)0xB72914
#define VAR_LITE_FOOT							(void*)0x60A71C
#define VAR_SHOT_TICK							*(int*)0xB5F898
#define VAR_GTA3_IMG_PATH						(void*)0x858AF4
#define VAR_CPED_SPECIAL_FLAGS					*(BYTE*)((PLAYER_POINTER) + 0x42)
#define VAR_CPED_STATE							*(BYTE*)((PLAYER_POINTER) + 0x15C)
#define VAR_CPED_PLAYER_STATE					*(BYTE*)((PLAYER_POINTER) + 0x46C)
#define VAR_SPAWNED								*(int*)0x0C15F50
#define VAR_SPRINT_SPEED						*(float*)0xB7CDB8
#define VAR_CURRENT_VEHICLE						*(int*)0xBA18FC 
#define VAR_CURRENT_WEAPON						*(int*)0xBAA410
#define VAR_SPEED								*(int*)0xB729C0
#define VAR_AIMING								*(int*)0xB6F858 // might be camera mode?
#define VAR_ANIMATION_SOMETHING					*(int*)0xB7CDB0
#define VAR_ANIMATION_SOMETHING_ELSE			*(int*)0xB7CDC0
#define VAR_SOMETHING_STATE						*(int*)0xB5EB28