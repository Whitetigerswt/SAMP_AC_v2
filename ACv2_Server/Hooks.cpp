#include "Hooks.h"
#include "Utility.h"
#include "plugin.h"
#include "Structs.h"
#include "Network/CRPCCallback.h"

#include "../Shared/Network/Network.h"
#include "../Shared/Network/CRPC.h"

#ifdef _WIN32
	#include <Psapi.h>
#endif

extern void *pAMXFunctions;

static SubHook amx_Register_hook;
static SubHook GetPacketID_hook;

bool bHooksInstalled = false;

AMX_NATIVE GetGravityFunc = NULL;

unsigned long FUNC_GetPacketID = NULL;

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
	return GetGravityFunc(pAmx, pParams);
}

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	SubHook::ScopedRemove remove(&amx_Register_hook);

	int i = 0;

	// Loop through all natives being registered
	while (!bHooksInstalled && nativelist[i].func > 0)
	{
		// If one matches GetGravity
		if (!strcmp(nativelist[i].name, "GetGravity"))
		{
			// Hook it
			bHooksInstalled = true;
			GetGravityFunc = nativelist[i].func;
			nativelist[i].func = SecretProc;
			break;
		}
		i++;
	}

	// note: will not allow YSF redirected natives...
	return amx_Register(amx, nativelist, number);
}

typedef BYTE(*getPacketId)(Packet *p);

// GetPacketID hook
BYTE GetPacketID(Packet *p)
{
	if (p == 0) return 255;

	if ((unsigned char)p->data[0] == 36)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else return (unsigned char)p->data[0];
}

unsigned int LastTicks[MAX_PLAYERS];
static BYTE HOOK_GetPacketID(Packet *p)
{
	SubHook::ScopedRemove remove(&GetPacketID_hook);

	BYTE packetId = GetPacketID(p);

	// Check for invalid packets
	if (packetId == 0xFF) {
		return 0xFF;
	}

	// If packetId is our custom RPC sending function
	if (packetId == PACKET_RPC)
	{
		// Read the data sent
		RakNet::BitStream bsData(&p->data[1], p->length - 1, false);
		unsigned short usRpcId;

		if (bsData.Read(usRpcId))
		{
			// Process the RPC
			CRPC::Process(usRpcId, bsData, p->playerIndex);
		}

		// return invalid packet (so SA-MP doesn't process it)
		if (GetPacketID_hook.GetTrampoline() == 0)
		{
			return 0xFF;
		}
	}

	if (packetId == ID_PLAYER_SYNC)
	{
		CSyncData *pSyncData = (CSyncData*)(&p->data[1]);
		
		// Crash fix
		if (isnan(pSyncData->fQuaternion[0]) || isnan(pSyncData->fQuaternion[1]) || isnan(pSyncData->fQuaternion[2]) || isnan(pSyncData->fQuaternion[3]) ||
			isnan(pSyncData->vecPosition.fX) || isnan(pSyncData->vecPosition.fY) || isnan(pSyncData->vecPosition.fZ) ||
			isnan(pSyncData->vecSurfing.fX) || isnan(pSyncData->vecSurfing.fY) || isnan(pSyncData->vecSurfing.fZ) || 
			isnan(pSyncData->vecVelocity.fX) || isnan(pSyncData->vecVelocity.fY) || isnan(pSyncData->vecVelocity.fZ))
		{
			return packetId;
		}

		// Crash fix
		if (isnan(pSyncData->fQuaternion[0]) || isnan(pSyncData->fQuaternion[1]) || isnan(pSyncData->fQuaternion[2]) || isnan(pSyncData->fQuaternion[3]) ||
			isnan(pSyncData->vecPosition.fX) || isnan(pSyncData->vecPosition.fY) || isnan(pSyncData->vecPosition.fZ) ||
			isnan(pSyncData->vecSurfing.fX) || isnan(pSyncData->vecSurfing.fY) || isnan(pSyncData->vecSurfing.fZ) || 
			isnan(pSyncData->vecVelocity.fX) || isnan(pSyncData->vecVelocity.fY) || isnan(pSyncData->vecVelocity.fZ))
		{
			return packetId;
		}


		// Fix, yet another, slide issue (punch, switch to any bullet weapon). might be temporary,
		// though it's sooo much easier to just leave this here than look for an asm hook
		if (pSyncData->wKeys & KEY_FIRE && pSyncData->byteWeapon < 15)
		{
			LastTicks[p->playerIndex] = Utility::getTickCount() + 350;
		}

		if (LastTicks[p->playerIndex] > Utility::getTickCount() && pSyncData->byteWeapon > 15)
		{
			pSyncData->byteWeapon = 0;
		}
	}

	// for some reason GetTrampoline only returns a useful value if YSF is loaded. (and crashes otherwise)

	// if YSF is not loaded
	if (GetPacketID_hook.GetTrampoline() == 0)
	{
		return packetId;
	}
	// If YSF is loaded
	else
	{
		return ((getPacketId)GetPacketID_hook.GetTrampoline())(p);
	}
}

bool FindAddresses()
{
#ifdef _WIN32
	FUNC_GetPacketID = FindPattern("\x8B\x44\x24\x04\x85\xC0\x75\x03\x0C\xFF\xC3", "xxxxxxx???x");
#else
	FUNC_GetPacketID = FindPattern("\x55\xB8\x00\x00\x00\x00\x89\xE5\x8B\x55\x00\x85\xD2", "xx????xxxx?xx");
#endif

	return FUNC_GetPacketID > 0;
}

void InstallAmxHooks()
{
	amx_Register_hook.Install((void*)*(unsigned long*)((unsigned long)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
}

void InstallHooks()
{
	if (FUNC_GetPacketID != NULL)
	{
		CRPCCallback::Initialize();
		GetPacketID_hook.Install((void*)FUNC_GetPacketID, (void*)HOOK_GetPacketID);
	}
	else
	{
		Utility::Printf("Error: Invalid SA-MP server version found. (Or YSF loaded before sampac)");
	}
}

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

unsigned long FindPattern(char *pattern, char *mask)
{
	unsigned long i;
	unsigned long size;
	unsigned long address;
#ifdef _WIN32
	MODULEINFO info = { 0 };

	address = (unsigned long)GetModuleHandle(NULL);
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(MODULEINFO));
	size = (unsigned long)info.SizeOfImage;
#else
	address = 0x804b480; // around the elf base
	size = 0x8128B80 - address;
#endif
	for (i = 0; i < size; ++i)
	{
		if (memory_compare((BYTE *)(address + i), (BYTE *)pattern, mask))
			return (unsigned long)(address + i);
	}
	return 0;
}
