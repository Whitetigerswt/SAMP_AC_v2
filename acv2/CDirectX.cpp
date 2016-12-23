#include "CDirectX.h"
#include "Addresses.h"
#include "CMem.h"
#include "Misc.h"

#include <tchar.h>

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;

void CDirectX::HookD3DFunctions()
{
	// Make sure the right d3d9 is loaded.
	wchar_t	filename[MAX_PATH];
	GetSystemDirectory(filename, (UINT)(MAX_PATH - strlen("\\d3d9.dll") - 1));
	_tcscat(filename, L"\\d3d9.dll");
	HMODULE d3d9 = LoadLibrary(filename);

	// Get the address of the DirectX creation func
	m_pDirect3DCreate9 = (Direct3DCreate9_t)GetProcAddress(GetModuleHandle(L"d3d9.dll"), "Direct3DCreate9");

	if (m_pDirect3DCreate9 == NULL)
	{
		// What now?
	}

	if (Misc::GetGameVersion() == 1) // is game us version
	{
		CMem::ApplyJmp(DIRECT_CREATE_US, (DWORD)DirectXCreationHookUS, 5);
	}
	else // else it's eu
	{
		CMem::ApplyJmp(DIRECT_CREATE_EU, (DWORD)DirectXCreationHookEU, 5);
	}
}

void CDirectX::Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd)
{
	m_pDevice = device;
	m_pDirect3D = direct3D;
}

DWORD DirectXCreationHookUSJmpBack = 0x7F6317;
__declspec(naked) void CDirectX::DirectXCreationHookUS()
{
	__asm
	{
		pushad
		call DirectXCreationProxy
		popad
		jmp[DirectXCreationHookUSJmpBack]
	}
}

DWORD DirectXCreationHookEUJmpBack = 0x7F6357;
__declspec(naked) void CDirectX::DirectXCreationHookEU()
{
	__asm
	{
		pushad
		call DirectXCreationProxy
		popad
		jmp[DirectXCreationHookEUJmpBack]
	}
}

void CDirectX::DirectXCreationProxy()
{
	DIRECT_D3D9 = new CD3D9Proxy(m_pDirect3DCreate9(D3D_SDK_VERSION));
}