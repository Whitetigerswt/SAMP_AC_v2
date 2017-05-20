#include "CMem.h"

DWORD CMem::m_dwUnprotectDummy;

void CMem::ApplyJmp(BYTE* pAddress, DWORD dwProxy, DWORD dwLen)
{
	DWORD dwOld, dwRelAddr;
	Unprotect(pAddress, dwLen, &dwOld);
	dwRelAddr = (DWORD)(dwProxy - (DWORD)pAddress) - 5;

	*pAddress = ASM_JMP;
	PutSingle<DWORD>(pAddress + 1, dwRelAddr);

	for (unsigned long i = 5; i < dwLen; ++i)
		PutSingle<BYTE>(pAddress + i, ASM_NOP);

	VirtualProtect((void*)pAddress, dwLen, dwOld, &m_dwUnprotectDummy);
}

#pragma warning(push)
#pragma warning(disable: 4789)
void CMem::Cpy(void* address, const void* src, int size)
{
	CMem::Unprotect(address, size);
	memcpy(address, src, size);
	DWORD dwOldProt = 0;
	VirtualProtect(address, size, m_dwUnprotectDummy, &dwOldProt);
}
#pragma warning(pop)

void CMem::RedirectCall(int address, void *func)
{
	int temp = 0xE8;
	Cpy((void *)address, &temp, 1);
	temp = (int)func - ((int)address + 5);
	Cpy((void *)((int)address + 1), &temp, 4);
}