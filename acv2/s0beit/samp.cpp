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

#define SAMP_DLL		"samp.dll"
#define SAMP_CMP		"00E8000085D27408508B"

//randomStuff
extern int						iViewingInfoPlayer;
int								g_iSpectateEnabled = 0, g_iSpectateLock = 0, g_iSpectatePlayerID = -1;
int								g_iCursorEnabled = 0;

// global samp pointers
int								iIsSAMPSupported = 0;
int								g_renderSAMP_initSAMPstructs;
DWORD							g_dwSAMP_Addr;
stSAMP							*g_SAMP = NULL;
stInputInfo						*g_Input = NULL;



//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void Load()
{
	g_dwSAMP_Addr = (DWORD)GetModuleHandle("samp.dll");
}


struct stSAMP *stGetSampInfo(void)
{
	Load();

	if (g_dwSAMP_Addr == NULL)
		return NULL;

	uint32_t	info_ptr;
	info_ptr = (UINT_PTR)* (uint32_t *)((uint8_t *)(void *)((uint8_t *)g_dwSAMP_Addr + SAMP_INFO_OFFSET));
	if (info_ptr == NULL)
		return NULL;

	return (struct stSAMP *)info_ptr;
}

struct stInputInfo *stGetInputInfo(void)
{
	Load();

	if (g_dwSAMP_Addr == NULL)
		return NULL;

	uint32_t	input_ptr;
	input_ptr = (UINT_PTR)* (uint32_t *)((uint8_t *)(void *)((uint8_t *)g_dwSAMP_Addr + SAMP_CHAT_INPUT_INFO_OFFSET));
	if (input_ptr == NULL)
		return NULL;

	return (struct stInputInfo *)input_ptr;
}