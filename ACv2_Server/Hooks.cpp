#include "Hooks.h"
#include "Utility.h"

extern void *pAMXFunctions;
static SubHook amx_Register_hook;
AMX_NATIVE RedirectedNative;

bool hooked = false;

cell AMX_NATIVE_CALL SecretProc(AMX* pAmx, cell* pParams)
{
	Utility::Printf("called in plugin, lolz");

	return 1;
}

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	SubHook::ScopedRemove remove(&amx_Register_hook);

	int i = 0;
	while (!hooked && nativelist[i].func > 0)
	{
		if (!strcmp(nativelist[i].name, "GetGravity"))
		{
			hooked = true;
			nativelist[i].func = SecretProc;
		}
		i++;
	}

	return amx_Register(amx, nativelist, number);
}

void InstallHooks()
{
	amx_Register_hook.Install((void*)*(unsigned long*)((unsigned long)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
}