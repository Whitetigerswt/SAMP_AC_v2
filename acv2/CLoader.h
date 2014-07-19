#include "CDirectX.h"


class CLoader 
{
public:
	static void Initialize();
	static void CheckElevation();
private:
	static BOOL IsProcessElevated();
};