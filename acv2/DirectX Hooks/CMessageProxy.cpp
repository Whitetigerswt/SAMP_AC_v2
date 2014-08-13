#include "CMessageProxy.h"
#include "../CDirectX.h"
#include <Windows.h>
#include "../CLog.h"
#include <RakNet\BitStream.h>
#include "../Network/Network.h"
#include "../../Shared/Network/CRPC.h"
#include "../Addresses.h"

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
	if (GetForegroundWindow() != CURRENT_HWND)
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
		case WM_SETFOCUS:
		{
			RakNet::BitStream bsData;
			bsData.Write(1);
			bsData.Write(true);

			Network::SendRPC(TOGGLE_PAUSE, &bsData);
		}
		case WM_KILLFOCUS:
		{
			RakNet::BitStream bsData;
			bsData.Write(1);
			bsData.Write(false);

			Network::SendRPC(TOGGLE_PAUSE, &bsData);
		}
		case WM_KEYUP:
		{
			if (vKey == VK_F8)
			{
				Network::SendRPC(TAKE_SCREENSHOT);
			}
		}
		default:
		{

		}
	}
	return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);

}