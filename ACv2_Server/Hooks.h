#pragma once

#include "SDK/samp-sdk/amx/amx.h"
#include "SDK/samp-sdk/plugincommon.h"
#include "subhook/subhook.h"

void InstallAmxHooks();
void InstallHooks();
bool FindAddresses();
unsigned long FindPattern(char *pattern, char *mask);

