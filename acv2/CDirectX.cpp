#include "CDirectX.h"
#include "Addresses.h"
#include "CMem.h"
#include "Misc.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;
ID3DXFont* CDirectX::m_pFont = NULL;
ID3DXSprite* CDirectX::m_pSprite = NULL;
Sprite* CDirectX::logo = NULL;

#define STUPID_STUFF 0

void CDirectX::HookD3DFunctions()
{
	// Make sure the right d3d9 is loaded.
	char	filename[MAX_PATH];
	GetSystemDirectory(filename, (UINT)(MAX_PATH - strlen("\\d3d9.dll") - 1));
	strcat(filename, "\\d3d9.dll");
	HMODULE d3d9 = LoadLibrary(filename);

	// Get the address of the DirectX creation func
	m_pDirect3DCreate9 = (Direct3DCreate9_t)GetProcAddress(GetModuleHandle("d3d9.dll"), "Direct3DCreate9");

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

void CDirectX::LoadImages()
{
#if STUPID_STUFF == 1
	if (m_pDevice != NULL && logo == NULL)
	{
		/*D3DXCreateSprite(m_pDevice, &m_pSprite);
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		D3DXCreateFont(m_pDevice, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &m_pFont);
		*/

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		logo = new Sprite((float)width / 2, (float)height / 2);

		logo->Init(m_pDevice, 311, 152);
	}
#endif
}

void CDirectX::Present()
{
	if (logo)
	{
		if (logo->isInitialized() && ADDRESS_LOADED > 8)
		{
			logo->Draw();
		}
	}
}

void CDirectX::OnReset()
{
	if (logo)
	{
		logo->OnResetDevice();
	}
}

void CDirectX::OnLost()
{
	if (logo)
	{
		logo->OnLostDevice();
	}
}