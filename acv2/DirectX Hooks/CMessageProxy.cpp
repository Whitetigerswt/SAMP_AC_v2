#include "CMessageProxy.h"
#include "../CDirectX.h"
#include <Windows.h>
#include "../CLog.h"
#include <RakNet\BitStream.h>
#include "../Network/Network.h"
#include "../../Shared/Network/CRPC.h"

HWND CMessageProxy::m_hWindowOrig;
WNDPROC CMessageProxy::m_wProcOrig;

void CMessageProxy::Initialize(HWND hWindow)
{
	if (m_wProcOrig != NULL || m_hWindowOrig == hWindow)
		return;

	m_wProcOrig = (WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG)(UINT*)Process);
	m_hWindowOrig = hWindow;
}

HWND CMessageProxy::GetWindowHandle()
{
	return m_hWindowOrig;
}

void CMessageProxy::Uninitialize()
{
	if (m_hWindowOrig == NULL)
		return;

	SetWindowLong(m_hWindowOrig, GWL_WNDPROC, (LONG)(UINT*)m_wProcOrig);
}

WNDPROC CMessageProxy::GetOriginalProcedure()
{
	return m_wProcOrig;
}

//TODO: use Process for something useful
LRESULT CALLBACK CMessageProxy::Process(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (GetForegroundWindow() != *(HWND*)0xC97C1C)
	{
		return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);
	}

	UINT vKey = (UINT)wparam;

	switch (umsg)
	{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (GetAsyncKeyState(vKey) & 0x8000)
			{
				return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);
			}
			else
			{
				return 0;
			}
		}
		case WM_HOTKEY:
		{
			return 0;
		}
		default:
		{

		}
	}
	return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);

}