#pragma once

#include <Windows.h>

class CMessageProxy
{
public:
	static void Initialize(HWND hWindow);
	static void Uninitialize();

	static HWND GetWindowHandle();
	static WNDPROC GetOriginalProcedure();

private:
	static LRESULT CALLBACK Process(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	static void Pulse();

	static HWND m_hWindowOrig;
	static WNDPROC m_wProcOrig;

};
