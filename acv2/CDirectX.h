#include "DirectX Hooks\CD3D9DeviceProxy.h"
#include "DirectX Hooks\CD3D9Proxy.h"
#include "DirectX Hooks\CDInput8DeviceProxy.h"
#include "DirectX Hooks\CDInput8Proxy.h"
#include "DirectX Hooks\CMessageProxy.h"

#include <Detours\detours.h>

typedef IDirect3D9 *(WINAPI* Direct3DCreate9_t)(UINT);
typedef HRESULT(WINAPI* DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

class CDirectX
{
public:
	static void HookD3DFunctions();
	static IDirect3D9* WINAPI HOOK_Direct3DCreate9(UINT SDKVersion);
	static HRESULT WINAPI HOOK_DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, void** ppvOut, IUnknown* punkOuter);
	static void PreDeviceReset();
	static void PostDeviceReset();
	static void PreEndScene();
	static void PostEndScene();
	static void Initialize(IDirect3DDevice9* device, IDirect3D9* direct3D, HWND hwnd);
private:
	static Direct3DCreate9_t  m_pDirect3DCreate9;
	static DirectInput8Create_t m_pDirectInput8Create;
	static IDirect3DDevice9* m_pDevice;
	static IDirect3D9* m_pDirect3D;
	static bool m_bCursorEnabled;

};