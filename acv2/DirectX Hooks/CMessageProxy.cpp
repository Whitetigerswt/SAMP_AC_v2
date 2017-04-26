#include "CMessageProxy.h"
#include "../CDirectX.h"
#include "../../Shared/Network/BitStream.h"
#include "../../Shared/Network/CRPC.h"
#include "../../Shared/Network/Network.h"
#include "../Addresses.h"
#include "../Misc.h"
#include "../Network/CRakClientHandler.h"
#include "../CLog.h"

#include <Boost\thread.hpp>

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

LRESULT CALLBACK CMessageProxy::Process(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	// Make sure we're connected to the AC server
	if (CRakClientHandler::HasEverConnected())
	{
		UINT vKey = (UINT)wparam;

		switch (umsg)
		{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			case WM_IME_KEYDOWN:
			{
				switch (umsg)
				{
					case WM_LBUTTONDOWN:
						vKey = VK_LBUTTON;
						break;
					case WM_RBUTTONDOWN:
						vKey = VK_RBUTTON;
						break;
					case WM_MBUTTONDOWN:
						vKey = VK_MBUTTON;
						break;
					case WM_XBUTTONDOWN:
						if (HIWORD(wparam) & XBUTTON1)
							vKey = VK_XBUTTON1;
						else if (HIWORD(wparam) & XBUTTON2)
							vKey = VK_XBUTTON2;
						break;
				}
				if ((GetAsyncKeyState(vKey) & 0x8000) || Misc::GetMacroLocks() == false)
				{
					return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);
				}
				else
				{
					if (umsg == WM_XBUTTONDOWN) // this button is kinda special, see remarks: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646245(v=vs.85).aspx
						return 1;
					else
						return 0;
				}
			}

			case WM_HOTKEY:
			{
				return 0;
			}

			case WM_SETFOCUS:
			{
				// Tell the server we came back into the game from an alt tab
				RakNet::BitStream bsData;

				// Write header info
				bsData.Write((unsigned char)PACKET_RPC);
				bsData.Write(TOGGLE_PAUSE);

				// main info
				bsData.Write(1);
				bsData.Write(true);

				CRakClientHandler::CustomSend(&bsData);

				Misc::SetAltTabState(false);

				// Ignore alt tabs (allow the game to run in the background)
				return false;
			}

			case WM_KILLFOCUS:
			{
				RakNet::BitStream bsData;

				// you get the idea...
				bsData.Write((unsigned char)PACKET_RPC);
				bsData.Write(TOGGLE_PAUSE);

				bsData.Write(1);
				bsData.Write(false);

				CRakClientHandler::CustomSend(&bsData);

				Misc::SetAltTabState(true);

				// Ignore alt tabs (allow the game to run in the background)
				return false;
			}

			case WM_KEYUP:
			{
				// If pressed f8
				if (vKey == VK_F8)
				{
					// Tell the server we took a screenshot
					RakNet::BitStream bsData;

					bsData.Write((unsigned char)PACKET_RPC);
					bsData.Write(TAKE_SCREENSHOT);

					CRakClientHandler::CustomSend(&bsData);
				}
			}

			case WM_ACTIVATE:
			{
				if ((WORD)wparam == WA_INACTIVE)
				{
					Misc::SetAltTabState(true);
					return false;
				}
			}

			default:
			{

			}
		}
	}
	return CallWindowProc(CMessageProxy::GetOriginalProcedure(), wnd, umsg, wparam, lparam);
}