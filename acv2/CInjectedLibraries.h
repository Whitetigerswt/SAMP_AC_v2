#pragma once
#include "CFileCheck.h"
class CInjectedLibraries :
	public CFileCheck
{
public:
	CInjectedLibraries();
	~CInjectedLibraries();

	// PURPOSE: Scan for new processes.
	// REQUIRES: NULL
	// PROMISE: A newly generated list of processes.
	void Scan();

};

