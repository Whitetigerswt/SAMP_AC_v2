#include <string.h>
#include "SDK/samp-sdk/amx/amx.h"
#include "SDK/samp-sdk/plugincommon.h"

#include "Network/Network.h"
#include "CThreadSync.h"

AMX_NATIVE_INFO* GetPluginNatives();
extern CThreadSync *pMainThreadSync;