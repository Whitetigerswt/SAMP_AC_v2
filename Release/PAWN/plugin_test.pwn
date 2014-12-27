#include <a_samp>

native Float:GetGravity();
native ac_gateway(...) = GetGravity;

public OnFilterScriptInit()
{
	ac_gateway("ain't nobody got time fo' dat");
}
