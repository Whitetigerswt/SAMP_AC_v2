#include "CDirectX.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;

void CDirectX::HookD3DFunctions()
{
	if (!m_pDirect3DCreate9)
	{
		PBYTE addr = DetourFindFunction("d3d9.dll", "Direct3DCreate9");

		if (addr != NULL)
		{
			m_pDirect3DCreate9 = (Direct3DCreate9_t)DetourFunction(addr, (BYTE*)HOOK_Direct3DCreate9);
		}
		else
		{
			MessageBoxA(NULL, "Could not hook DirectX, Try reinstalling DirectX 9.0c.", "SAMP AC v2", MB_ICONERROR);
		}
	}
}

IDirect3D9* WINAPI CDirectX::HOOK_Direct3DCreate9(UINT SDKVersion)
{
	IDirect3D9* Direct3D = m_pDirect3DCreate9(SDKVersion);
	IDirect3D9* Mine_Direct3D = new CD3D9Proxy(Direct3D);
	return Mine_Direct3D;
}

void CDirectX::Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd)
{
	m_pDevice = device;
	m_pDirect3D = direct3D;
}