#pragma once

#include "DirectX Hooks\CD3D9DeviceProxy.h"
#include "DirectX Hooks\CD3D9Proxy.h"
#include "DirectX Hooks\CMessageProxy.h"
#include "CSprite.h"

#include <Detours\detours.h>

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>

typedef IDirect3D9 *(WINAPI* Direct3DCreate9_t)(UINT);

class CDirectX
{
public:
	static void HookD3DFunctions();
	static IDirect3D9* WINAPI HOOK_Direct3DCreate9(UINT SDKVersion);
	static void Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd);
	static void CheckD3DHooks();
	static void DirectXCreationHookUS();
	static void DirectXCreationHookEU();
	static void DirectXCreationProxy();
	static void LoadImages();
	static void Present();
	static void OnLost();
	static void OnReset();

	static Sprite* logo;

private:
	static Direct3DCreate9_t  m_pDirect3DCreate9;
	static IDirect3DDevice9* m_pDevice;
	static IDirect3D9* m_pDirect3D;
	static ID3DXSprite* m_pSprite;
	static ID3DXFont* m_pFont;
};