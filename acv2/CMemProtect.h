#pragma once
#include <Windows.h>
#include <vector>


class CMemProtect
{
public:
	struct stEntry {
		BYTE *address;
		BYTE *content;
		size_t size;
	};

	static void Add(BYTE *address, BYTE *content, size_t size);
	static void Reapply(const stEntry entry);
	static void Process();
private:

	static std::vector<stEntry> _list;
};


