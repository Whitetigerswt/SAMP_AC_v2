#pragma once
#include "CCheats.h"
#include <string>
#include <Windows.h>

class CProcessList :
	public CCheats
{
public:
	CProcessList();
	~CProcessList();

	void Scan();

private:
	void AdjustTokens(HANDLE process);
};

