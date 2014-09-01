#pragma once
#include <Windows.h>
#include <vector>

typedef HMODULE(WINAPI* LoadLibrary_t)(LPCSTR);

class CModuleSecurity
{
public:
	CModuleSecurity();
	~CModuleSecurity();

	// PURPOSE: To hook LoadLibrary
	// REQUIRES: NULL
	// PROMISE: LoadLibrary will be hooked
	static void HookLoadLibrary();

	static void AddAllowedModule(LPCTSTR lpFileName);

	static bool IsAllowedModule(std::string szModule);

	static bool IsAddressInAllowedModule(DWORD address);

	static DWORD WINAPI GetThreadStartAddress(DWORD tid);

	static void AddAllowedModules();

private:
	static LoadLibrary_t m_pLoadLibrary;

	static HMODULE WINAPI HOOK_LoadLibrary(LPCTSTR lpFileName);

	static std::vector<std::string> m_AllowedModules;

	
};

