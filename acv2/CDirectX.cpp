#include "CDirectX.h"
#include "CLog.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;
DirectInput8Create_t CDirectX::m_pDirectInput8Create = NULL;

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

	if (!m_pDirectInput8Create)
	{
		PBYTE addr = DetourFindFunction("dinput8.dll", "DirectInput8Create");
		if (addr != NULL)
		{
			m_pDirectInput8Create = (DirectInput8Create_t)DetourFunction(addr, (BYTE*)HOOK_DirectInput8Create);
		}
	}
}

HRESULT WINAPI CDirectX::HOOK_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, void** ppvOut, IUnknown* punkOuter)
{
	HRESULT hr = m_pDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	IDirectInput8A* pDInput = (IDirectInput8A*)*ppvOut;
	CDInput8Proxy* pDInputHook = new CDInput8Proxy(pDInput);
	*ppvOut = pDInputHook;
	return hr;
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

void CDirectX::PreDeviceReset()
{

}

void CDirectX::PostDeviceReset()
{

}

void CDirectX::PostEndScene()
{

}

void CDirectX::PreEndScene()
{

}