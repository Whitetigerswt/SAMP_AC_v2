/*

PROJECT:		mod_sa
LICENSE:		See LICENSE in the top level directory
COPYRIGHT:		Copyright we_sux, FYP

mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

mod_sa is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mod_sa is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <Windows.h>
#include "stdtypes.h"
#include "samp.h"

stSAMP							*g_SAMP = NULL;
DWORD sampBaseAddress = NULL;
DWORD sampSize = NULL;


//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

struct stSAMP *stGetSampInfo(void)
{
	return g_SAMP;
}

void setSampBaseAddress(DWORD mod)
{
	sampBaseAddress = mod;
}

DWORD getSampBaseAddress()
{
	return sampBaseAddress;
}

void setSampSize(DWORD s)
{
	sampSize = s;
}

DWORD getSampSize()
{
	return sampSize;
}