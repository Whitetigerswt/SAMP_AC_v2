/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** IMG.cpp
** Class to create or edit GTA IMG archives with efficient algorithms.
** Works with archive versions:
** • version 1 (GTA III, GTA VC, GTA III Mobile)
** • version 2 (GTA SA)
** does not support GTA IV yet.
**
** Version: 1.9
** Author: fastman92
** Site: fastman92-site.tk
** -------------------------------------------------------------------------*/

#include <tchar.h>
#include "IMG.h"
#include "Misc.h"

using namespace std;

int fileChangeSize(FILE *fp, __int64 size)
{
	__int64 position = _ftelli64(fp);
	fseek(fp, 0, SEEK_SET);

	int filedes = _fileno(fp);
	int result = _chsize_s(filedes, size);

	if (position < size)
		_fseeki64(fp, position, SEEK_SET);

	return result;
}

tIMG_FileOrder IMG::fileOrderByExtension[] =
{
	// eFileOrder ID  Extension Name
	{ eIMG_FileOrder::DFF, TEXT("dff"), TEXT("model") },
	{ eIMG_FileOrder::TXD, TEXT("txd"), TEXT("TXD library") },
	{ eIMG_FileOrder::COL, TEXT("col"), TEXT("Collision archive") },
	{ eIMG_FileOrder::IPL, TEXT("ipl"), TEXT("Item placement") },
	{ eIMG_FileOrder::IFP, TEXT("ifp"), TEXT("Item frame projection") },
	{ eIMG_FileOrder::CUT, TEXT("cut"), TEXT("Cutscene") },
	{ eIMG_FileOrder::DAT, TEXT("dat"), TEXT("DAT") },
};

// Default constructor
IMG::IMG()
{
#ifdef IMG_DEBUG
	this->debugFile = fopen("IMG_debug.log", "w");

	if (debugFile)
	{
		const char text[] = "IMG object initialized\n";
		fwrite(text, sizeof(text) - 1, 1, debugFile);
	}
#endif

	this->IMG_Handle = NULL;
	this->DIR_Handle = NULL;
	this->archiveVersion = IMG_VERSION_UNDEFINED;

	this->GTAIV_encryption_key = TEXT("\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce")
		TEXT("\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d");

	this->FilenamesSize = 0;
}

// Destructor
IMG::~IMG()
{
#ifdef IMG_DEBUG
	fclose(this->debugFile);
#endif

	this->CloseArchive();
}

// Opens IMG archive, assumes IMG archive to exist.
// Detects archive type automatically
bool IMG::OpenArchive(const wchar_t* path)
{
	this->CloseArchive();

	if (this->IMG_Handle = _tfopen(path, TEXT("rb")))
	{
		GetFullPathName(path, _countof(this->IMG_fullPath), this->IMG_fullPath, &this->IMG_filename);

		unsigned long int version;

		size_t NumberOfReadBytes = fread(&version, sizeof(char), 4, this->IMG_Handle);

		if (NumberOfReadBytes == 4 && version == '2REV')
		{
			this->SetArchiveFormat(IMG_VERSION_2);

			unsigned long int n_files;

			if (fread(&n_files, 4, 1, this->IMG_Handle))
			{
				while (n_files > 0)
				{
					if (feof(this->IMG_Handle))
					{
						this->CloseArchive();
						return false;
					}
					this->ListOfEntries.push_back(IMG_Entry(this));
					this->ReadEntryOfVersion1And2Header(ListOfEntries.back(), this->IMG_Handle);
					n_files--;
				}

				return true;
			}
		}
		else
		{
			const wchar_t* extension = _tcsrchr(path, '.');

			bool hasExtension = false;

			wchar_t dirPath[_MAX_PATH];

			if (extension)
			{
				extension++;
				hasExtension = true;

				memcpy(dirPath, path, extension - path);
				_tcscpy(&dirPath[extension - path], TEXT("dir"));
			}

			if (hasExtension && (this->DIR_Handle = _tfopen(dirPath, TEXT("rb+"))))
			{
				fseek(this->DIR_Handle, 0, SEEK_END);

				size_t DIR_size = ftell(this->DIR_Handle);

				if (DIR_size % 32 == 0)
				{
					fseek(this->DIR_Handle, 0, SEEK_SET);

					this->SetArchiveFormat(IMG_VERSION_1);

					for (unsigned int i = 0; i < DIR_size / 32; i++)
					{
						this->ListOfEntries.push_back(IMG_Entry(this));
						this->ReadEntryOfVersion1And2Header(ListOfEntries.back(), this->DIR_Handle);
					}

					return true;
				}
			}
			else if (NumberOfReadBytes == 4)
			{
				if (version == GTAIV_MAGIC_ID)
					this->SetArchiveFormat(IMG_VERSION_3_UNENCRYPTED);
				else
					this->SetArchiveFormat(IMG_VERSION_3_ENCRYPTED);

				IMG_version3_header Header;

				if (fread(&Header.Version, 1, sizeof(Header) - 4, this->IMG_Handle) == sizeof(Header) - 4 && Header.Version == 3
					&& Header.SingleTableItemSize == 16)
				{
					for (unsigned int FileNum = 0; FileNum < Header.NumberOfItems; FileNum++)
					{
						IMG_version3_tableItem item;
						if (fread(&item, sizeof(IMG_version3_tableItem), 1, this->IMG_Handle) != 1)
							goto closeArchiveAndReturnFalse;

						this->ListOfEntries.push_back(IMG_Entry(this));
						auto& CurrentEntry = this->ListOfEntries.back();
						CurrentEntry.Position = item.Position;
						CurrentEntry.SizeInBytes = item.SizeInBytes;

						DWORD SizeInBlocks = GET_ALIGNED_SIZE_IN_BLOCKS(item.SizeInBytes, IMG_BLOCK_SIZE);

						if (SizeInBlocks > item.SizeInBlocks)
							goto closeArchiveAndReturnFalse;

						CurrentEntry.ResourceType = (eIMG_file_ResourceTypes)item.ResourceType;
					}

					this->FilenamesSize = Header.TableOfItemsSize - (Header.NumberOfItems * Header.SingleTableItemSize);
					char* Filenames = new char[FilenamesSize];

					if (fread(Filenames, FilenamesSize, 1, this->IMG_Handle) != 1)
					{
						delete[] Filenames;
						return false;
					}

					char* CurrentFilename = Filenames;
					for (auto& entry = this->ListOfEntries.begin(); entry != this->ListOfEntries.end(); entry++)
					{
						unsigned int i;

						for (i = 0; CurrentFilename[i]; i++)
							entry->Name[i] = CurrentFilename[i];

						entry->Name[i] = NULL;

						entry->UpdateFileNameHash();

						CurrentFilename += i + 1;
					}

					delete[] Filenames;

					return true;
				}
			}
		}

	closeArchiveAndReturnFalse:
		this->CloseArchive();
		return false;
	}

	return false;
}

// Creates .img archive
// Example: object.CreateArchive("new.img", IMG::IMG_version::VERSION_1);
bool IMG::CreateArchive(const wchar_t* path, eIMG_version version)
{
	this->CloseArchive();

	GetFullPathName(path, _countof(this->IMG_fullPath), this->IMG_fullPath, &this->IMG_filename);

	this->SetArchiveFormat(version);

	switch (version)
	{
	case IMG_VERSION_1:
	{
		const wchar_t* extension;

		if ((extension = _tcsrchr(path, '.'))++)
		{
			wchar_t dirPath[_MAX_PATH];
			memcpy(dirPath, path, extension - path);
			_tcscpy(&dirPath[extension - path], TEXT("dir"));

			if ((this->DIR_Handle = _tfopen(dirPath, TEXT("wb+"))) && (this->IMG_Handle = _tfopen(path, TEXT("wb+"))))
			{
				this->DoModifiedListOfEntriesActions();
				return true;
			}
			else
				this->CloseArchive();
		}
		return false;
	}
	case IMG_VERSION_2:
	{
		if (this->IMG_Handle = _tfopen(path, TEXT("wb+")))
		{
			fwrite("VER2\x0\x0\x0\x0", 1, 8, this->IMG_Handle);

			this->DoModifiedListOfEntriesActions();
			return true;
		}
		return false;
	}
	case IMG_VERSION_3_UNENCRYPTED:
	case IMG_VERSION_3_ENCRYPTED:
	{
		if (this->IMG_Handle = _tfopen(path, TEXT("wb+")))
		{
			this->DoModifiedListOfEntriesActions();
			return true;
		}

		return false;
	}

	default:
		this->archiveVersion = IMG_VERSION_UNDEFINED;
		return false;
	}
}

// Opens .img archive if exists or creates if not exists
// Example: object.OpenOrCreateArchive("new.img", IMG::IMG_version::VERSION_1);
bool IMG::OpenOrCreateArchive(const wchar_t* path, eIMG_version version)
{
	if (PathFileExists(path))
		return this->OpenArchive(path);
	else
		return this->CreateArchive(path, version);
}

// Checks if archive is currently opened.
bool IMG::IsArchiveOpened()
{
	return this->IMG_Handle != NULL;
}

// Sets writing of list of IMG entries to be done on request
void IMG::SetWritingListOfEntriesToBeDoneOnRequest(bool state)
{
	this->bUseManualWritingOfEntries = state;
}

// Writes a list of entries
void IMG::WriteListOfEntriesNow()
{
	if (this->bUseManualWritingOfEntries && this->bWritingOfEntriesPending)
	{
		this->bWritingOfEntriesPending = false;
		this->WriteListOfEntries();
	}
}

// Closes archive
void IMG::CloseArchive()
{
	if (this->IMG_Handle)
	{
		this->WriteListOfEntriesNow();

		fclose(this->IMG_Handle);
		this->IMG_Handle = NULL;
	}

	this->ListOfEntries.clear();

	if (this->DIR_Handle)
	{
		fclose(this->DIR_Handle);
		this->DIR_Handle = NULL;
	}
}

// Rebuilds archive
bool IMG::RebuildArchive()
{
	wchar_t temporaryImgFilepath[_countof(this->IMG_fullPath) + 16];
	_tcscpy(temporaryImgFilepath, this->IMG_fullPath);
	_tcscpy(temporaryImgFilepath, TEXT(".IMGclassTmpFile"));

	FILE* newIMGhandle;

	if (newIMGhandle = _tfopen(temporaryImgFilepath, TEXT("wb+")))
	{
		if (this->archiveVersion == IMG_VERSION_2)
			fprintf(newIMGhandle, "VER2");

		// Seek to offset where first file is written
		fseek(newIMGhandle, GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE), SEEK_SET);

		for (auto entry = ListOfEntries.begin(); entry != ListOfEntries.end(); entry++)
		{
			size_t size = entry->GetFilesizeAllignedToBlocks();
			wchar_t* loadedFile = new wchar_t[size];

			_fseeki64(this->IMG_Handle, (unsigned __int64)entry->Position * IMG_BLOCK_SIZE, SEEK_SET);
			fread(loadedFile, 1, size, this->IMG_Handle);

			entry->Position = (DWORD)(_ftelli64(newIMGhandle) / IMG_BLOCK_SIZE);
			fwrite(loadedFile, 1, size, newIMGhandle);

			delete[] loadedFile;
		}

		this->WriteListOfEntries(newIMGhandle);

		fclose(newIMGhandle);
		fclose(this->IMG_Handle);

		_wremove(this->IMG_fullPath);
		if (_wrename(temporaryImgFilepath, this->IMG_fullPath))
		{
			this->bHasArchiveBeenModifiedAndNotRebuiltYet = false;

			return (this->IMG_Handle = this->IMG_Handle = _tfopen(this->IMG_fullPath, TEXT("rb+"))) != 0;
		}
	}

	return false;
}

// Returns true if archive was modified, but not rebuilt yet.
bool IMG::hasArchiveBeenModifiedAndNotRebuiltYet()
{
	return this->bHasArchiveBeenModifiedAndNotRebuiltYet;
}

// Gets the size of .img archive in bytes
unsigned __int64 IMG::GetImgArchiveSize()
{
	if (this->IMG_Handle)
	{
		auto currentPos = _ftelli64(this->IMG_Handle);
		_fseeki64(this->IMG_Handle, 0, SEEK_END);

		unsigned __int64 sizeOfArchive = _ftelli64(this->IMG_Handle);

		_fseeki64(this->IMG_Handle, currentPos, SEEK_SET);
		return sizeOfArchive;
	}
	else
		return UNDEFINED;
}

// Gets size of unused space in .img file
unsigned __int64 IMG::GetSizeOfUnusedSpace()
{
	tIMGentriesContainer sortedListByPosition = this->ListOfEntries;

	this->SortListOfEntriesByPositionAndSize(sortedListByPosition);

	unsigned __int64 UnusedSpace = 0;

	if (sortedListByPosition.size() >= 1)
	{
		size_t headerSize = GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE);
		UnusedSpace = sortedListByPosition.front().Position * IMG_BLOCK_SIZE - headerSize;

		tIMGEntryIterator sortedEntry = sortedListByPosition.begin();

		tIMGEntryIterator lastEntry = sortedListByPosition.end();
		lastEntry--;

		while (sortedEntry != lastEntry)
		{
			tIMGEntryIterator nextEntry = sortedEntry;
			nextEntry++;

			signed __int64 UnusedSpaceItem = (nextEntry->Position - (sortedEntry->Position + sortedEntry->GetFilesizeAllignedToBlocks() / IMG_BLOCK_SIZE))*IMG_BLOCK_SIZE;

			if ((signed int)UnusedSpaceItem > 0)
				UnusedSpace += UnusedSpaceItem;

			sortedEntry = nextEntry;
		}
		UnusedSpace += this->GetImgArchiveSize() - (sortedListByPosition.back().Position * IMG_BLOCK_SIZE + sortedListByPosition.back().GetFilesizeAllignedToBlocks());

		return UnusedSpace;
	}
	else
	{
		size_t headerSize = GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE);
		unsigned __int64 IMG_size = this->GetImgArchiveSize();
		return IMG_size > headerSize ? IMG_size - headerSize : 0;
	}
}

// Retrieves the number of files inside of IMG archive
DWORD IMG::GetFileCount()
{
	return this->ListOfEntries.size();
}

// Adds or replaces file if exists
IMG::tIMGEntryIterator IMG::AddOrReplaceFile(const wchar_t* name, const void* ptr, size_t size)
{
	tIMGEntryIterator it = this->GetFileIteratorByName(name);

	if (it != this->ListOfEntries.end())
	{
		this->ReplaceSingleFile(it, ptr, size);
		return it;
	}
	else
		return this->AddFile(name, ptr, size);
}


// Adds file
IMG::tIMGEntryIterator IMG::AddFile(const wchar_t* name, const void* ptr, size_t size)
{
	if (!this->IsFileNameValid(name))
		return this->ListOfEntries.end();

	tRelatedFilesContainer RelatedFiles;

	this->GetIteratorsOfAssociatedFiles(name, RelatedFiles);

	tIMGEntryIterator currentEntry;

	if (RelatedFiles.size() > 0)
	{
		sort(RelatedFiles.begin(), RelatedFiles.end(),
			[](const tIMGEntryIterator& a, const tIMGEntryIterator& b)
		{
			return _tcscmp(a->Name, b->Name) < 0;
		}
		);

		for (tRelatedFilesContainer::iterator i = RelatedFiles.end() - 1; i >= RelatedFiles.begin(); i--)
		{
			if (_tcscmp(name, (*i)->Name) > 0)
			{
				currentEntry = *i;

				currentEntry++;

				currentEntry = this->ListOfEntries.insert(currentEntry, IMG_Entry(this));

				i++;

				RelatedFiles.insert(i, currentEntry);

				goto InsertNewFile;
			}
		}

		currentEntry = this->ListOfEntries.insert(*RelatedFiles.begin(), IMG_Entry(this));

		RelatedFiles.insert(RelatedFiles.begin(), currentEntry);

	}
	else
	{
		currentEntry = this->ListOfEntries.insert(this->end(), IMG_Entry(this));

		RelatedFiles.push_back(currentEntry);
	}

InsertNewFile:
	_tcscpy(currentEntry->Name, name);

	currentEntry->UpdateFileNameHash();

	this->DoRelatedFilesLoop(RelatedFiles, currentEntry, ptr, size);

	return currentEntry;
}

// Replaces file depending on iterator
void IMG::ReplaceSingleFile(tIMGEntryIterator IMGentryIt, const void* ptr, size_t size)
{
	tRelatedFilesContainer RelatedFiles;

	this->GetIteratorsOfAssociatedFiles(IMGentryIt->Name, RelatedFiles);

	this->DoRelatedFilesLoop(RelatedFiles, IMGentryIt, ptr, size);
}

// Loop used in AddFile and ReplaceSingleFile functions
void IMG::DoRelatedFilesLoop(tRelatedFilesContainer& RelatedFiles, tIMGEntryIterator newIMGentry, const void* ptr, size_t size)
{
	size_t allignedSize = GET_ALIGNED_SIZE(size, IMG_BLOCK_SIZE);

	size_t SizeOfAllReplacedFiles = allignedSize;

	for (tIMGEntryIterator& i : RelatedFiles)
		SizeOfAllReplacedFiles += i->GetFilesizeAllignedToBlocks();

	unsigned __int64 PositionOfNewFiles = this->FindFirstEmptySpaceForFile(SizeOfAllReplacedFiles, &RelatedFiles);
	unsigned __int64 iPositionOfNewFiles = PositionOfNewFiles;

	char* pNewFiles = new char[SizeOfAllReplacedFiles];
	pNewFiles = "";

	char* iNewFiles = pNewFiles;

	for (tIMGEntryIterator& entry : RelatedFiles)
	{
		if (entry == newIMGentry)
		{
			memcpy(iNewFiles, ptr, size);
			memset(iNewFiles + size, NULL, allignedSize - size);

			iNewFiles += allignedSize;

			if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
				entry->SizeInBytes = size;
			else
			{
				if (entry->SizeFirstPriority)
					entry->SizeFirstPriority = allignedSize / IMG_BLOCK_SIZE;
				else
					entry->SizeSecondPriority = allignedSize / IMG_BLOCK_SIZE;
			}

			entry->Position = (DWORD)iPositionOfNewFiles / IMG_BLOCK_SIZE;
			iPositionOfNewFiles += allignedSize;
		}
		else
		{
			size_t processedFileSize = entry->GetFilesize();
			size_t processedAlignedFileSize = GET_ALIGNED_SIZE(processedFileSize, IMG_BLOCK_SIZE);

			entry->ReadEntireFile(iNewFiles);

			memset(iNewFiles + processedFileSize, NULL, processedAlignedFileSize - processedFileSize);

			entry->Position = (DWORD)iPositionOfNewFiles / IMG_BLOCK_SIZE;
			iNewFiles += processedAlignedFileSize;

			iPositionOfNewFiles += processedAlignedFileSize;
		}
	}

	_fseeki64(this->IMG_Handle, PositionOfNewFiles, SEEK_SET);

	fwrite(pNewFiles, 1, SizeOfAllReplacedFiles, this->IMG_Handle);

	delete[] pNewFiles;

	this->DoModifiedArchiveActions();
}

// Sets format properties
void IMG::SetArchiveFormat(eIMG_version version)
{
	this->archiveVersion = version;

	this->bHasArchiveBeenModifiedAndNotRebuiltYet = false;

	this->bUseManualWritingOfEntries = false;

	if (this->archiveVersion == IMG_VERSION_3_ENCRYPTED)
		this->IMG_ENTRY_MAX_FILE_NAME_LENGTH = _countof(((IMG_Entry*)0)->Name) - 1;
	else
		this->IMG_ENTRY_MAX_FILE_NAME_LENGTH = _countof(((IMG_version2_tableItem*)0)->Name) - 1;
}

// Renames a file
bool IMG::RenameFile(tIMGEntryIterator fileInfo, const wchar_t* NewName)
{
	if (!this->IsFileNameValid(NewName))
		return false;

	if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
		this->FilenamesSize -= _tcslen(fileInfo->Name) + 1;

	_tcscpy(fileInfo->Name, NewName);

	fileInfo->UpdateFileNameHash();

	if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
		this->FilenamesSize += _tcslen(fileInfo->Name) + 1;

	this->DoModifiedArchiveActions();

	return true;
}

// Remove a file
IMG::tIMGEntryConstIterator IMG::RemoveFile(tIMGEntryConstIterator _Where)
{
	if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
		FilenamesSize -= _tcslen(_Where->Name) + 1;

	tIMGEntryConstIterator result = this->ListOfEntries.erase(_Where);

	this->DoModifiedArchiveActions();

	return result;
}

// Removes files
IMG::tIMGEntryConstIterator IMG::RemoveFiles(
	tIMGEntryConstIterator _First_arg,
	tIMGEntryConstIterator _Last_arg
	)
{
	if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
	{
		for (tIMGEntryConstIterator it = _First_arg; it != _Last_arg; it++)
			FilenamesSize -= _tcslen(it->Name) + 1;
	}

	tIMGEntryConstIterator result = this->ListOfEntries.erase(_First_arg, _Last_arg);

	this->DoModifiedArchiveActions();

	return result;
}

// Gets iterator of file pointing to ListOfFiles
IMG::tIMGEntryIterator IMG::GetFileIteratorByName(const wchar_t* name)
{
	if (ListOfEntries.size())
	{
		if (!this->IsFileNameValid(name))
			return this->ListOfEntries.end();

		unsigned long hash = GTASA_CRC32_fromUpCaseString(Misc::utf8_encode(name).c_str());

		auto& entry = ListOfEntries.begin();

		do
		{
			if (entry->NameHash == hash && !_tcsicmp(entry->Name, name))
				return entry;

			entry++;
		} while (entry != ListOfEntries.end());
	}

	return this->ListOfEntries.end();
}

// Checks if file with specified name exists and returns TRUE/FALSE
bool IMG::FileExists(const wchar_t* name)
{
	return this->GetFileIteratorByName(name) != this->ListOfEntries.end();
}

// Checks if filename's length is appropriate for IMG entry as well as name characters.
bool IMG::IsFileNameValid(const wchar_t* name)
{
	return _tcslen(name) <= IMG_ENTRY_MAX_FILE_NAME_LENGTH;
}

// Gets filename for imported file, filename may be truncated if archive version is 1 or 2.
// Returns true if name is truncated
errno_t IMG::GetFilenameForImportedFile(const wchar_t* lpFileName, wchar_t* lpFilePart, DWORD nBufferLength)
{
	wchar_t FullPath[_MAX_PATH];
	wchar_t* FileName = TEXT("");

	int len = GetFullPathName(lpFileName, _countof(FullPath), FullPath, &FileName);

	if (len != 0 && len != _countof(FullPath))
	{
		size_t ThisSize = nBufferLength > IMG_ENTRY_MAX_FILE_NAME_LENGTH + 1 ? IMG_ENTRY_MAX_FILE_NAME_LENGTH + 1 : nBufferLength;
		return _tcsncpy_s(lpFilePart, ThisSize, FileName, _TRUNCATE);
	}
}

// Access file by name
// Returns a reference to the last element in the vector container.
IMG::tIMGEntryReference IMG::GetFileRefByName(const wchar_t* name)
{
	return *this->GetFileIteratorByName(name);
}

// Return iterator to beginning
IMG::tIMGEntryIterator IMG::begin()
{
	return this->ListOfEntries.begin();
}

// Return iterator to beginning
IMG::tIMGEntryConstIterator IMG::begin() const
{
	return this->ListOfEntries.begin();
}

// Return iterator to end
IMG::tIMGEntryIterator IMG::end()
{
	return this->ListOfEntries.end();
}

// Return iterator to end
IMG::tIMGEntryConstIterator IMG::end() const
{
	return this->ListOfEntries.end();
}

// Return reverse iterator to reverse beginning
IMG::tIMGentriesContainer::reverse_iterator IMG::rbegin()
{
	return this->ListOfEntries.rbegin();
}

// Return reverse iterator to reverse beginning
IMG::tIMGentriesContainer::const_reverse_iterator IMG::rbegin() const
{
	return this->ListOfEntries.rbegin();
}

// Return reverse iterator to reverse end
IMG::tIMGentriesContainer::reverse_iterator IMG::rend()
{
	return this->ListOfEntries.rend();
}

// Return reverse iterator to reverse end
IMG::tIMGentriesContainer::const_reverse_iterator IMG::rend() const
{
	return this->ListOfEntries.rend();
}

// Access last element
// Returns a reference to the last element in the vector container.
IMG::tIMGEntryReference IMG::back()
{
	return this->ListOfEntries.back();
}

// Access last element
// Returns a reference to the last element in the vector container.
IMG::tIMGEntryConstReference IMG::back() const
{
	return this->ListOfEntries.back();
}

// Access first element
// Returns a reference to the first element in the vector container.
IMG::tIMGEntryReference IMG::front()
{
	return this->ListOfEntries.front();
}

// Access first element
// Returns a reference to the first element in the vector container.
IMG::tIMGEntryConstReference IMG::front() const
{
	return this->ListOfEntries.front();
}

// Get end of list entries and header
DWORD IMG::GetEndOfHeaderDataPastTheListOfFiles()
{
	switch (this->archiveVersion)
	{
	case IMG_VERSION_1:
		return 32;	// watermark
	case IMG_VERSION_2:
		return
			8	// version, number of files
			+ this->ListOfEntries.size() * sizeof(IMG_version2_tableItem)
			+ 32;	// watermark

	case IMG_VERSION_3_UNENCRYPTED:
	case IMG_VERSION_3_ENCRYPTED:
		return
			20	// header
			+ this->ListOfEntries.size() * 16
			+ FilenamesSize
			+ 32;	// watermark
	default:
		return UNDEFINED;
	}
}

// Gets IPL filename and num, returns true on success and false on failure.
bool IMG::GetIPLfilenameAndNum(const wchar_t* fullFilename, wchar_t* IPL_name, DWORD* IPL_num)
{
	size_t nameLen = _tcslen(fullFilename);

	const wchar_t* DotAndExtension = fullFilename + nameLen - 4;

	if (nameLen >= 5 && !_tcsicmp(DotAndExtension, TEXT(".ipl")))
	{
		const wchar_t* StartOfNum = DotAndExtension - 1;

		while (isdigit(*StartOfNum))
			StartOfNum--;

		StartOfNum++;

		if (StartOfNum < DotAndExtension)
		{
			if (IPL_num)
				*IPL_num = _tstoi(StartOfNum);

			if (IPL_name)
			{
				memcpy(IPL_name, fullFilename, StartOfNum - fullFilename);
				IPL_name[StartOfNum - fullFilename] = NULL;
			}

			return true;
		}
	}

	if (IPL_num)
		*IPL_num = UNDEFINED;

	return false;
}

// Gets ID of this file OR IDs of all .ipl files with this name
void IMG::GetIteratorsOfAssociatedFiles(const wchar_t* SearchedName, tRelatedFilesContainer& list)
{
	list.clear();

	wchar_t Cur_IPL_Name[256];
	DWORD Cur_IPL_Num;

	if (GetIPLfilenameAndNum(SearchedName, Cur_IPL_Name, &Cur_IPL_Num))
	{
		auto listEnd = this->ListOfEntries.end();

		for (auto entry = this->ListOfEntries.begin(); entry != listEnd; entry++)
		{
			wchar_t tested_IPL_Name[256];
			DWORD tested_IPL_Num;

			if (GetIPLfilenameAndNum(entry->Name, tested_IPL_Name, &tested_IPL_Num))
			{
				// if(!_stricmp(Cur_IPL_Name, tested_IPL_Name))

				list.push_back(entry);
			}
		}
	}
	else
	{
		tIMGEntryIterator it = this->GetFileIteratorByName(SearchedName);

		if (it != this->end())
			list.push_back(it);
	}
}

// Loads files identified by DWORD indexes into continous aligned memory
// Returned is size of all files loaded from list
// Remember to free memory when it's no longer neccessary! Use delete!
// None of addresses may be NULL
void IMG::LoadFilesInCountinousAlignedMemory(tRelatedFilesContainer& FileIterators, void** retAddress, size_t* retSize, FILE* imgHandle)
{
	if (!imgHandle)
		imgHandle = this->IMG_Handle;

	size_t SizeOfFiles = 0;

	for (tIMGEntryIterator& i : FileIterators)
		SizeOfFiles += i->GetFilesizeAllignedToBlocks();

	char* pFiles = new char[SizeOfFiles];
	char* iFiles = pFiles;

	for (tIMGEntryIterator& entry : FileIterators)
	{
		_fseeki64(imgHandle, (unsigned __int64)entry->Position * IMG_BLOCK_SIZE, SEEK_SET);
		size_t Filesize = entry->GetFilesizeAllignedToBlocks();
		fread(iFiles, 1, Filesize, imgHandle);

		iFiles += Filesize;
	}

	*retAddress = pFiles;
	*retSize = SizeOfFiles;
}

// Moves files located in position before list of files to suitable position
void IMG::MoveFilesLocatedBeforeListOfFilesToSuitablePosition(FILE* imgHandle)
{
	DWORD treshold = this->GetEndOfHeaderDataPastTheListOfFiles();

	auto listEnd = ListOfEntries.end();

	for (auto entry = ListOfEntries.begin(); entry != listEnd; entry++)
	{
		if (entry->Position * IMG_BLOCK_SIZE < treshold)
		{
			tRelatedFilesContainer RelatedFiles;

			this->GetIteratorsOfAssociatedFiles(entry->Name, RelatedFiles);

			char* MovedFiles;
			size_t MovedFilesSize;

			LoadFilesInCountinousAlignedMemory(
				RelatedFiles,
				(void**)&MovedFiles,
				&MovedFilesSize,
				imgHandle);

			DWORD PositionOfFiles = (DWORD)(this->FindFirstEmptySpaceForFile(MovedFilesSize, &RelatedFiles)) / IMG_BLOCK_SIZE;

			DWORD CurrentPosition = PositionOfFiles;
			for (tIMGEntryIterator& m : RelatedFiles)
			{
				m->Position = CurrentPosition;
				CurrentPosition += m->GetFilesizeAllignedToBlocks() / IMG_BLOCK_SIZE;
			}

			_fseeki64(imgHandle, PositionOfFiles * IMG_BLOCK_SIZE, SEEK_SET);
			fwrite(MovedFiles, 1, MovedFilesSize, imgHandle);
			delete[] MovedFiles;
		}
	}
}

// Returns pattern of IMG validity
void IMG::GetPatternToCheckIfPositionIsValid(wchar_t* str)
{
	const wchar_t a[] = TEXT("\x5f\x5b\x6e\x70\x6a\x5f\x6d\x39\x33\x22\x4c\x51\x4c\x26\x4a\x33\x34\x2a\x6e\x78\x6e\x81\x82\x30\x42\x40\x4c\x14\x15\x16\x17\x18");

	wchar_t c;

	DWORD d = 0;

	do
	{
		c = a[d] - (wchar_t)d + 7;
		str[d] = c;

		d++;
	} while (d < 32);
}

// Tests if file position is valid
void IMG::TestIfPositionIsValid(FILE* imgHandle)
{
	wchar_t b[32];
	GetPatternToCheckIfPositionIsValid(b);

	fwrite(b, 1, 32, imgHandle);
}

// reads version 1 or 2 header
void IMG::ReadEntryOfVersion1And2Header(IMG_Entry& entry, FILE* file)
{
	fread(&entry.Position, 4, 1, file);
	fread(&entry.SizeSecondPriority, 2, 1, file);
	fread(&entry.SizeFirstPriority, 2, 1, file);
	fread(&entry.Name, 24, 1, file);
	entry.UpdateFileNameHash();
}

// writes version 1 or 2 header
void IMG::WriteListOfEntriesVersion1And2(FILE* file)
{
	IMG_version2_tableItem* pBuffer = new IMG_version2_tableItem[this->ListOfEntries.size()];

	IMG_version2_tableItem* iBuffer = pBuffer;

	for_each(ListOfEntries.begin(), ListOfEntries.end(),
		[&](IMG_Entry entry)
	{
		iBuffer->Position = entry.Position;
		iBuffer->SizeSecondPriority = entry.SizeSecondPriority;
		iBuffer->SizeFirstPriority = entry.SizeFirstPriority;

		strncpy_s(iBuffer->Name, Misc::utf8_encode(entry.Name).c_str(), _countof(((IMG_version2_tableItem*)0)->Name) - 1);
		iBuffer++;
	}
	);

	fwrite(pBuffer, sizeof(IMG_version2_tableItem), this->ListOfEntries.size(), file);
	delete[] pBuffer;
}

// Does action when the list of entries needs to be saved
void IMG::DoModifiedListOfEntriesActions()
{
	if (this->bUseManualWritingOfEntries)
		this->bWritingOfEntriesPending = true;
	else
		this->WriteListOfEntries();
}

// Does action when the archive has been modified
void IMG::DoModifiedArchiveActions()
{
	this->bHasArchiveBeenModifiedAndNotRebuiltYet = true;

	this->DoModifiedListOfEntriesActions();
}

// Updates list of entries in IMG archive/DIR file
void IMG::WriteListOfEntries(FILE* imgHandle)
{
	if (!imgHandle)
		imgHandle = this->IMG_Handle;

	if (this->ListOfEntries.size() > 0)
	{
		tIMGEntryIterator FileWithHighestPosition = max_element(ListOfEntries.begin(), ListOfEntries.end(),
			[](IMG_Entry a, IMG_Entry b) { return a.Position < b.Position; });

		fileChangeSize(imgHandle, (unsigned __int64)FileWithHighestPosition->Position * IMG_BLOCK_SIZE + FileWithHighestPosition->GetFilesizeAllignedToBlocks());
	}
	else
		fileChangeSize(imgHandle, GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE));

	this->MoveFilesLocatedBeforeListOfFilesToSuitablePosition(imgHandle);

	if (archiveVersion == IMG_VERSION_1)
	{
		fseek(imgHandle, 0, SEEK_SET);
		this->TestIfPositionIsValid(imgHandle);

		fileChangeSize(this->DIR_Handle, 0);
		_fseeki64(this->DIR_Handle, 0, SEEK_SET);

		this->WriteListOfEntriesVersion1And2(this->DIR_Handle);
	}
	else if (archiveVersion == IMG_VERSION_2)
	{
		fseek(imgHandle, 4, SEEK_SET);		// number of files

		DWORD numberOfFiles = this->ListOfEntries.size();
		fwrite(&numberOfFiles, 4, 1, imgHandle);

		this->WriteListOfEntriesVersion1And2(imgHandle);

		this->TestIfPositionIsValid(imgHandle);
	}
	else if (archiveVersion == IMG_VERSION_3_UNENCRYPTED)
	{
		fseek(imgHandle, 0, SEEK_SET);

		size_t BeginningSizeAlligned = GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE);
		wchar_t* pBeginning = new wchar_t[BeginningSizeAlligned];

		wchar_t* iBeginning = pBeginning;
		this->WriteVersion3HeaderToMemory((IMG_version3_header*)iBeginning);

		iBeginning += sizeof(IMG_version3_header);

		IMG_version3_tableItem* ItemDescription = (IMG_version3_tableItem*)iBeginning;

		for (auto& entry = this->ListOfEntries.begin(); entry != this->ListOfEntries.end(); entry++, ItemDescription++)
		{
			ItemDescription->SizeInBytes = entry->SizeInBytes;
			ItemDescription->ResourceType = entry->ResourceType;
			ItemDescription->Position = entry->Position;
			ItemDescription->SizeInBlocks = (WORD)GET_ALIGNED_SIZE_IN_BLOCKS(entry->SizeInBytes, IMG_BLOCK_SIZE);
			ItemDescription->Unknown = NULL;
		}

		wchar_t* Filenames = (wchar_t*)ItemDescription;
		for (auto& entry = this->ListOfEntries.begin(); entry != this->ListOfEntries.end(); entry++)
		{
			unsigned int i;

			for (i = 0; entry->Name[i]; i++)
				*(Filenames++) = entry->Name[i];

			*(Filenames++) = NULL;
		}

		GetPatternToCheckIfPositionIsValid(Filenames);

		fwrite(pBeginning, BeginningSizeAlligned, 1, imgHandle);

		delete[] pBeginning;
	}
}

// Writes file content alligned to IMG_BLOCK_SIZE
// Returned value is number of blocks written ( realSize / IMG_BLOCK_SIZE )
size_t IMG::WriteAllignedFileToIMGblocks(const void* ptr, size_t size)
{
	fwrite(ptr, 1, size, this->IMG_Handle);
	size_t allignedSize = GET_ALIGNED_SIZE(size, IMG_BLOCK_SIZE);

	size_t sizeToAllign = allignedSize - size;
	char contentToAllign[2048];
	memset(contentToAllign, NULL, sizeToAllign);

	fwrite(contentToAllign, 1, sizeToAllign, this->IMG_Handle);

	return allignedSize / IMG_BLOCK_SIZE;
}

// Returns pointer to file order struct by ID or NULL
tIMG_FileOrder* IMG::getFileOrderStructByID(eIMG_FileOrder orderID)
{
	tIMG_FileOrder* result = find_if(IMG::fileOrderByExtension, fileOrderByExtension + _countof(fileOrderByExtension),
		[orderID](tIMG_FileOrder& fileOrder)
	{
		return fileOrder.orderID == orderID;
	});

	return result != fileOrderByExtension + _countof(fileOrderByExtension) ? result : NULL;
}

// Writes IMG archive version 3 header to memory.
void IMG::WriteVersion3HeaderToMemory(IMG_version3_header* header)
{
	header->MagicID = GTAIV_MAGIC_ID;
	header->Version = 3;
	header->NumberOfItems = this->ListOfEntries.size();
	header->TableOfItemsSize = header->NumberOfItems * sizeof(IMG_version3_tableItem) + this->FilenamesSize;
	header->SingleTableItemSize = sizeof(IMG_version3_tableItem);
	header->Unknown = NULL;
}

// Sorts an array by
// 1. Position
// 2. Size
void IMG::SortListOfEntriesByPositionAndSize(IMG::tIMGentriesContainer& list)
{
	list.sort(
		[](IMG_Entry& a, IMG_Entry& b) -> bool
	{
		size_t a_filesize = a.GetFilesizeAllignedToBlocks();
		size_t b_filesize = b.GetFilesizeAllignedToBlocks();

		return
			a.Position < b.Position ? true :
			a.Position > b.Position ? false :

			a_filesize < b_filesize ? true :
			b_filesize > b_filesize;
	}
	);
}

// Finds a first free space for target file
unsigned __int64 IMG::FindFirstEmptySpaceForFile(size_t filesize, tRelatedFilesContainer* overwrittenFileIterators)
{
	filesize = GET_ALIGNED_SIZE_IN_BLOCKS(filesize, IMG_BLOCK_SIZE);

	tIMGentriesContainer sortedListByPosition;

	if (overwrittenFileIterators)
	{
		// cout << "test: " << overwrittenFileIterators -> size() << endl;

		// Remove overwritten file indexes from sortedOverwrittenFileIndexes
		for (tIMGEntryIterator OriginalEntry = this->ListOfEntries.begin(); OriginalEntry != this->ListOfEntries.end(); OriginalEntry++)
		{
			if (find(overwrittenFileIterators->begin(), overwrittenFileIterators->end(), OriginalEntry) == overwrittenFileIterators->end())
				sortedListByPosition.push_back(*OriginalEntry);
		}
	}

	this->SortListOfEntriesByPositionAndSize(sortedListByPosition);

	unsigned __int64 BeginningOfFileSpace = GET_ALIGNED_SIZE(GetEndOfHeaderDataPastTheListOfFiles(), IMG_BLOCK_SIZE);

	if (sortedListByPosition.size() >= 1 && ((signed __int64)sortedListByPosition.front().Position
		- BeginningOfFileSpace / IMG_BLOCK_SIZE
		<= filesize || BeginningOfFileSpace / IMG_BLOCK_SIZE > sortedListByPosition.front().Position))
	{
		tIMGEntryIterator sortedEntry = sortedListByPosition.begin();

		tIMGEntryIterator lastEntry = sortedListByPosition.end();
		lastEntry--;

		while (sortedEntry != lastEntry)
		{
			tIMGEntryIterator nextEntry = sortedEntry;
			nextEntry++;

			unsigned long int sortedEntryFileSizeInBlocks = sortedEntry->GetFilesizeInBlocks();

			__int64 FreeSpace = nextEntry->Position - (sortedEntry->Position + sortedEntryFileSizeInBlocks);

			if (FreeSpace >= filesize)
				return (unsigned __int64)sortedEntry->Position * IMG_BLOCK_SIZE + sortedEntryFileSizeInBlocks * IMG_BLOCK_SIZE;

			sortedEntry = nextEntry;
		}

		return (unsigned __int64)lastEntry->Position * IMG_BLOCK_SIZE + lastEntry->GetFilesizeAllignedToBlocks();
	}
	else
		return BeginningOfFileSpace;
}

// Outputs list of entries specified as argument
void IMG::DebugListOfEntries(tIMGentriesContainer& list)
{

	size_t countOfListEntries = this->ListOfEntries.size();

	if (countOfListEntries > 0)
	{
		unsigned int i = 0;

		char pattern[256];

		strcpy(pattern, "\n%");

		_itoa(GetNumberOfDigits(countOfListEntries - 1, 10), pattern + strlen(pattern), 10);

		strcat(pattern, "d %08X");
		/*
		unsigned __int64 maxOffset = max(list.begin(), list.end(),
		[](const tIMGEntryConstIterator& a, const tIMGEntryConstIterator& b)
		{
		return a -> Position < b -> Position;
		}
		) -> Position * IMG_BLOCK_SIZE;

		cout << hex << maxOffset;
		*/
		// cout << pattern;

		for (IMG_Entry& entry : list)
		{
			entry.DebugEntry(i);

			i++;

			/*
			if(i==50)
			break;
			*/
		}
	}
}

// Outputs ListOfEntries
void IMG::DebugListOfEntries()
{
	this->DebugListOfEntries(this->ListOfEntries);
}

// Constructor
IMG_Entry::IMG_Entry(IMG * IMG_Instance)
{
	this->IMG_Instance = IMG_Instance;

	this->Position = 0;
	this->SizeSecondPriority = 0;
	this->SizeFirstPriority = 0;
	this->SizeInBytes = 0;
	this->ResourceType = RESOURCE_UNDEFINED;
	this->Name[0] = NULL;

	this->curPos = 0;
}

void IMG_Entry::UpdateFileNameHash()
{
	this->NameHash = GTASA_CRC32_fromUpCaseString(Misc::utf8_encode(this->Name).c_str());
}

// Returns pointer to file name.
const wchar_t* IMG_Entry::GetFilename()
{
	return this->Name;
}

// Writes a filename without extension to sufficiently long buffer
void IMG_Entry::GetFilenameWithoutExtension(wchar_t* filenameWithoutExtension, int size)
{
	if (const wchar_t* extension = _tcsrchr(this->Name, '.'))
	{
		memcpy(filenameWithoutExtension, this->Name, extension - this->Name);
		filenameWithoutExtension[extension - this->Name] = NULL;
	}
	else
		_tcscpy_s(filenameWithoutExtension, size, this->Name);
}

// Returns file order ID
eIMG_FileOrder IMG_Entry::GetFileOrderIDbyExtension()
{
	if (const wchar_t* extension = _tcsrchr(this->Name, '.'))
	{
		extension++;

		tIMG_FileOrder* fileOrderInfo = find_if(
			IMG::fileOrderByExtension,
			IMG::fileOrderByExtension + _countof(IMG::fileOrderByExtension),
			[extension](tIMG_FileOrder& fileOrder)
		{
			return !_tcsicmp(fileOrder.orderExtension, extension);
		});

		if (fileOrderInfo != IMG::fileOrderByExtension + _countof(IMG::fileOrderByExtension))
			return fileOrderInfo->orderID;
	}

	return eIMG_FileOrder::INVALID;
}

// Reads whole file, doesn't affect current file position (curPos)
// Returns number of bytes read.
size_t IMG_Entry::ReadEntireFile(void* ptr)
{
	_fseeki64(this->IMG_Instance->IMG_Handle, (unsigned __int64) this->Position * IMG::IMG_BLOCK_SIZE, SEEK_SET);

	size_t Filesize = this->GetFilesize();
	fread(ptr, 1, Filesize, this->IMG_Instance->IMG_Handle);
	return Filesize;
}

// Reads to memory
size_t IMG_Entry::Read(void* Ptr, size_t Size)
{
	if (this->curPos < this->GetFilesize())
	{
		size_t SizeToRead = this->curPos + Size > this->GetFilesize() ? this->GetFilesize() - this->curPos : Size;

		this->Seek(0, SEEK_CUR);

		size_t n_readBytes = fread(Ptr, 1, SizeToRead, this->IMG_Instance->IMG_Handle);
		this->curPos += n_readBytes;

		return n_readBytes;
	}
	else
		return false;
}

// Reads one byte
char IMG_Entry::ReadC()
{
	if (this->curPos < this->GetFilesize())
	{
		this->Seek(0, SEEK_CUR);
		char result;
		this->curPos += fread(&result, 1, 1, this->IMG_Instance->IMG_Handle);

		return result;
	}
	else
		return -1;
}

// Get file size in bytes
size_t IMG_Entry::GetFilesize()
{
	if (this->IMG_Instance->archiveVersion == IMG_VERSION_3_ENCRYPTED
		|| this->IMG_Instance->archiveVersion == IMG_VERSION_3_UNENCRYPTED)
		return this->SizeInBytes;
	else
	{
		size_t realSize = this->SizeFirstPriority ? this->SizeFirstPriority : this->SizeSecondPriority;
		realSize *= IMG::IMG_BLOCK_SIZE;
		return realSize;
	}
}

// Get file size aligned to blocks
size_t IMG_Entry::GetFilesizeAllignedToBlocks()
{
	if (this->IMG_Instance->archiveVersion == IMG_VERSION_3_ENCRYPTED
		|| this->IMG_Instance->archiveVersion == IMG_VERSION_3_UNENCRYPTED)
		return GET_ALIGNED_SIZE(this->SizeInBytes, IMG::IMG_BLOCK_SIZE);
	else
	{
		size_t realSize = this->SizeFirstPriority ? this->SizeFirstPriority : this->SizeSecondPriority;
		realSize *= IMG::IMG_BLOCK_SIZE;
		return realSize;
	}
}

// Gets file size in blocks
size_t IMG_Entry::GetFilesizeInBlocks()
{
	return this->GetFilesizeAllignedToBlocks() / IMG::IMG_BLOCK_SIZE;
}

// Check End-of-File indicator
bool IMG_Entry::isEOF()
{
	return this->curPos >= this->GetFilesize();
}

// Seek to certain position
bool IMG_Entry::Seek(unsigned int offset, int origin)
{
	bool result = false;

	if (origin == SEEK_SET)
	{
		if (offset <= this->GetFilesize())
		{
			this->curPos = offset;
			result = true;
		}
	}
	else if (origin == SEEK_CUR)
	{
		if (this->curPos + offset <= this->GetFilesize())
		{
			this->curPos += offset;
			result = true;
		}
	}
	else if (origin == SEEK_END)
	{
		if (this->curPos + this->GetFilesize() + offset <= this->GetFilesize())
		{
			this->curPos += this->GetFilesize() + offset;
			result = true;
		}
	}

	return _fseeki64(this->IMG_Instance->IMG_Handle, (unsigned __int64)this->Position * IMG::IMG_BLOCK_SIZE + this->curPos, SEEK_SET) != 0;
}

// Returns current position
unsigned int IMG_Entry::Tell()
{
	return this->curPos;
}

// Outputs entry informations
void IMG_Entry::DebugEntry(int fileID)
{
	printf("\n%d Offset: %llX", fileID, (unsigned __int64)this->Position * IMG::IMG_BLOCK_SIZE);

	if (this->IMG_Instance->archiveVersion == IMG_VERSION_1 || this->IMG_Instance->archiveVersion == IMG_VERSION_2)
		printf(" 2nd size: %d 1st size: %d", this->SizeSecondPriority * IMG::IMG_BLOCK_SIZE, this->SizeFirstPriority * IMG::IMG_BLOCK_SIZE);
	else
		printf(" Size in bytes: %d Resource type: %d", this->SizeInBytes, this->ResourceType);

	printf(" Name: %ls", this->Name);
}