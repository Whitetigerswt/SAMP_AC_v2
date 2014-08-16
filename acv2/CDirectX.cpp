#include "CDirectX.h"
#include "CLog.h"

Direct3DCreate9_t  CDirectX::m_pDirect3DCreate9 = NULL;

IDirect3DDevice9* CDirectX::m_pDevice = NULL;
IDirect3D9* CDirectX::m_pDirect3D = NULL;

void CDirectX::HookD3DFunctions()
{

	CLog log = CLog("d3d9.log");
	log.Write("Start");

	// Make sure d3d9.dll is loaded.
	LoadLibrary("d3d9.dll");
	log.Write("Loaded d3d9.dll");

	PBYTE addr = DetourFindFunction("d3d9.dll", "Direct3DCreate9");

	log.Write("Direct3DCreate9: 0x%x", addr);

	if (addr != NULL)
	{
		m_pDirect3DCreate9 = (Direct3DCreate9_t)DetourFunction(addr, (BYTE*)HOOK_Direct3DCreate9);
		
		log.Write("m_pDirect3DCreate9: 0x%x", m_pDirect3DCreate9);

		// Make sure the direct X pointer isn't NULL.
		if (m_pDirect3DCreate9 == NULL)
		{
			MessageBoxA(NULL, "(2) Could not hook DirectX, Try reinstalling DirectX 9.0c.", "SAMP AC v2", MB_ICONERROR);
		}
	}
	else
	{
		MessageBoxA(NULL, "Could not hook DirectX, Try reinstalling DirectX 9.0c.", "SAMP AC v2", MB_ICONERROR);
	}
}

IDirect3D9* WINAPI CDirectX::HOOK_Direct3DCreate9(UINT SDKVersion)
{
	CLog log = CLog("d3d9.log");
	log.Write("Creating directX.");
	IDirect3D9* Direct3D = m_pDirect3DCreate9(SDKVersion);
	log.Write("Direct3D: 0x%x", Direct3D);
	IDirect3D9* Mine_Direct3D = new CD3D9Proxy(Direct3D);
	log.Write("Mine_Direct3D: 0x%x", Mine_Direct3D);
	return Mine_Direct3D;
}

void CDirectX::Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd)
{
	m_pDevice = device;
	m_pDirect3D = direct3D;
}