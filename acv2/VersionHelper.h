#pragma once

#include "../Shared/SelfUpdater/CSelfUpdater.h"

namespace VersionHelper
{
	extern CSelfUpdater::stVersion AC_CLIENT_VERSION;
	extern wchar_t AC_CLIENT_VERSION_STRING[16];

	// PURPOSE: Construct AC_CLIENT_VERSION_STRING from AC_CLIENT_VERSION at client startup.
	void Initialize();

	// PURPOSE: Check and do self-update.
	void CheckForUpdate();
}
