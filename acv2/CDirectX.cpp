#include "CDirectX.h"
#include "CLog.h"
#include "Addresses.h"
#include "CMem.h"
#include "Misc.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;

void CDirectX::HookD3DFunctions()
{
	char	filename[MAX_PATH];
	GetSystemDirectory(filename, (UINT)(MAX_PATH - strlen("\\d3d9.dll") - 1));
	strcat(filename, "\\d3d9.dll");
	HMODULE d3d9 = LoadLibrary(filename);

	if (Misc::GetGameVersion() == 1)
	{
		CMem::ApplyJmp(DIRECT_CREATE_US, (DWORD)DirectXCreationHookUS, 5);
	}
	else
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
	DIRECT_D3D9 = new CD3D9Proxy(Direct3DCreate9(D3D_SDK_VERSION));
}