#pragma once
#include "CCheats.h"
class CInjectedLibraries :
	public CCheats
{
public:
	CInjectedLibraries();
	~CInjectedLibraries();

	// PURPOSE: Scan for new processes.
	// REQUIRES: NULL
	// PROMISE: A newly generated list of processes.
	void Scan();

};

