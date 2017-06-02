#include "CMemProtect.h"
#include "CMem.h"

std::vector<CMemProtect::stEntry> CMemProtect::_list;

void CMemProtect::Add(BYTE *address, BYTE *content, size_t size)
{
	stEntry entry;

	entry.address = address;
	entry.content = new BYTE[size];
	entry.size = size;

	CMem::Unprotect(address, size, NULL);

	if(content != NULL)
		CMem::Cpy(entry.content, content, size);
	else
		CMem::Cpy(entry.content, address, size);

	_list.push_back(entry);
}

void CMemProtect::Reapply(const stEntry entry)
{
	CMem::Cpy(entry.address, entry.content, entry.size);
}

void CMemProtect::Process()
{
	for (auto& entry : _list)
	{
		if (memcmp(entry.address, entry.content, entry.size) != 0)
		{
			Reapply(entry);
		}
	}
}
