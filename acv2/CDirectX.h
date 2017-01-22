#pragma once

#include "DirectX Hooks\CD3D9DeviceProxy.h"
#include "DirectX Hooks\CD3D9Proxy.h"
#include "DirectX Hooks\CMessageProxy.h"


#include <Detours\detours.h>

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>

typedef IDirect3D9 *(WINAPI* Direct3DCreate9_t)(UINT);

class CDirectX
{
public:

	// PURPOSE: Hook directX functions.
	// REQUIRES: NULL
	static void HookD3DFunctions();

	// PURPOSE: Save a copy of our Direct X interface and direct3D pointer object.
	// REQUIRES: The directX objects to have been created.
	static void Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd);

private:
	static Direct3DCreate9_t  m_pDirect3DCreate9;
	static IDirect3DDevice9* m_pDevice;
	static IDirect3D9* m_pDirect3D;
	static ID3DXSprite* m_pSprite;
	static ID3DXFont* m_pFont;

	// PURPOSE: Hook jump function for the US gta_sa.exe
	// REQUIRES: NULL
	static void DirectXCreationHookUS();

	// PURPOSE: Hook jump function for the EU gta_sa.exe
	// REQUIRES: NULL
	static void DirectXCreationHookEU();

	// PURPOSE: Unify the EU and US gta_sa.exe directX creation.
	// REQUIRES: Be called from one of the directX creation hooks.
	static void DirectXCreationProxy();

	// PURPOSE: Load AC logo on player's screen
	// REQUIRES: DirectX hooked.
	static void LoadImages();

	static void OnLost();
	static void OnReset();
};