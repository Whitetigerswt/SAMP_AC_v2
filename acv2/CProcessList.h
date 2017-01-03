#pragma once
#include "CFileCheck.h"
#include <string>
#include <Windows.h>

class CProcessList :
	public CFileCheck
{
public:
	// PROMISE: Adjusts token privileges on the current process.
	CProcessList();
	~CProcessList();

	// PURPOSE: Scan for new processes.
	// REQUIRES: NULL
	// PROMISE: A newly generated list of processes.
	void Scan();

private:
	// PURPOSE: Adjust token privileges for the process.
	// REQUIRES: A process handle.
	// PROMISE: Adjust token privileges if we have permissions to.
	void AdjustTokens(HANDLE process);
};

