#pragma once

#include "DirectX Hooks\CD3D9DeviceProxy.h"
#include "DirectX Hooks\CD3D9Proxy.h"
#include "DirectX Hooks\CMessageProxy.h"

#include <Detours\detours.h>

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

private:
	static Direct3DCreate9_t  m_pDirect3DCreate9;
	static IDirect3DDevice9* m_pDevice;
	static IDirect3D9* m_pDirect3D;
};