#include "CDirectX.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;
DirectInput8Create_t CDirectX::m_pDirectInput8Create = NULL;
SetCursorPos_t  CDirectX::m_pSetCursorPos = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;

bool CDirectX::m_bCursorEnabled = NULL;

void CDirectX::HookD3DFunctions()
{
	if (!m_pDirect3DCreate9)
	{
		m_pDirect3DCreate9 = (Direct3DCreate9_t)DetourFunction(DetourFindFunction("d3d9.dll", "Direct3DCreate9"), (BYTE*)HOOK_Direct3DCreate9);
	}

	if (!m_pSetCursorPos)
	{
		m_pSetCursorPos = (SetCursorPos_t)DetourFunction(DetourFindFunction("user32.dll", "SetCursorPos"), (BYTE*)HOOK_SetCursorPos);
	}

	if (!m_pDirectInput8Create)
	{
		m_pDirectInput8Create = (DirectInput8Create_t)DetourFunction(DetourFindFunction("dinput8.dll", "DirectInput8Create"), (BYTE*)HOOK_DirectInput8Create);
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

BOOL WINAPI CDirectX::HOOK_SetCursorPos(int iX, int iY)
{
	if (CMessageProxy::OnSetCursorPos(iX, iY))
		m_pSetCursorPos(iX, iY);

	return false;
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

BOOL CDirectX::OnCursorMove(int X, int Y)
{
	return false;
}

void CDirectX::PreEndScene()
{

}

void CDirectX::PostEndScene()
{

}

BOOL CDirectX::ToggleCursor(bool toggle)
{
	m_bCursorEnabled = toggle;
	return m_pDevice->ShowCursor(toggle);
}

BOOL CDirectX::IsCursorEnabled()
{
	return m_bCursorEnabled;
}