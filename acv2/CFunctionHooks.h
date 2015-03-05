#pragma once
class CFunctionHooks
{
public:
	static bool CheckHookIntegrity(DWORD address);
	static bool InstallHook(DWORD address, DWORD redirect);
};

