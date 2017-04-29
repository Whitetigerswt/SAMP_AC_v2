#pragma once

#include "../Shared/SelfUpdater/CSelfUpdater.h"

namespace VersionHelper
{
	extern char HTTP_REQUEST_USER_AGENT[32];
	extern char AC_SERVER_VERSION_STRING[16];
	extern CSelfUpdater::stVersion AC_SERVER_VERSION;

	// PURPOSE: Construct HTTP_REQUEST_USER_AGENT and AC_SERVER_VERSION_STRING from AC_SERVER_VERSION at server startup.
	void Initialize();

	// PURPOSE: Check if server is compatible with client. (major and minor must be equal for both client and server)
	bool IsClientCompatible(CSelfUpdater::stVersion clientVersion);

	// PURPOSE: Check and do self-update.
	void CheckForUpdate();
}
