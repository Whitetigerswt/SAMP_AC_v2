#include "Hooks.h"
#include "Utility.h"
#include "plugin.h"

extern void *pAMXFunctions;
static SubHook amx_Register_hook;
bool hooked = false;

AMX_NATIVE GetGravity = NULL;

cell AMX_NATIVE_CALL SecretProc(AMX* pAmx, cell* pParams)
{
	// If the number of parameters is greater than 0
	if (pParams[0] > 0)
	{
		// Get a list of all natives the plugin registered
		AMX_NATIVE_INFO* natives = GetPluginNatives();

		// gateway function sends the function name as a string as the first parameter always, so get the function we're trying to call
		char* func;
		amx_StrParam(pAmx, pParams[1], func);

		// Remove the first parameter string from the params
		pParams[0] -= 4;
		pParams[1] = pParams[0];

		// Loop through all natives registered
		int i = 0;
		while (natives[i].func > 0)
		{
			// If a native matches the function we're trying to call
			if (!strcmp(natives[i].name, func))
			{
				// Call it, and return 
				return natives[i].func(pAmx, &pParams[1]);
				break;
			}
			i++;
		}
	}

	// If we couldn't find the function, or no parameters were sent, call GetGravity as usual.
	return GetGravity(pAmx, pParams);
}

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	SubHook::ScopedRemove remove(&amx_Register_hook);

	int i = 0;

	// Loop through all natives being registered
	while (!hooked && nativelist[i].func > 0)
	{
		// If one matches GetGravity
		if (!strcmp(nativelist[i].name, "GetGravity"))
		{
			// Hook it
			hooked = true;
			GetGravity = nativelist[i].func;
			nativelist[i].func = SecretProc;
			break;
		}
		i++;
	}

	return amx_Register(amx, nativelist, number);
}

void InstallHooks()
{
	amx_Register_hook.Install((void*)*(unsigned long*)((unsigned long)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
}