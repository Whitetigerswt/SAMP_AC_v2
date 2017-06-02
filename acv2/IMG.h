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

#include <windows.h>
#include <vector>
#include <algorithm>
#include <io.h>
#include <iostream>
#include <list>
#include <utility>
#include "Shlwapi.h"
#pragma comment( lib, "shlwapi.lib")

#include "GTASA_CRC32.h"
// #include "Rijndael.h"

#undef min
#undef max

#define GET_ALIGNED_SIZE(requested_size, block_size) (requested_size % block_size == 0 ? requested_size : ((requested_size / block_size)+1)*block_size)
#define GET_ALIGNED_SIZE_IN_BLOCKS(requested_size, block_size) (requested_size % block_size == 0 ? requested_size / block_size : (requested_size / block_size) + 1)

#define _countOfAndPointer(Array) _countof(Array), Array

// #define IMG_DEBUG

// Function to get number of digits
// Example: number 1000 has 4 digits.
template <class T>
int GetNumberOfDigits(T number, unsigned int base = 10)
{
	int digits = 0;
	if (number < 0) digits = 1; // remove this line if '-' counts as a digit
	while (number) {
		number /= base;
		digits++;
	}
	return digits;
}

enum eIMG_file_ResourceTypes : DWORD
{
	RESOURCE_UNDEFINED = 0x00,
	RESOURCE_GENERIC = 0x01,
	RESOURCE_TEXTURE_ARCHIVE = 0x08,
	RESOURCE_BOUNDS = 0x20,
	RESOURCE_MODEL_FILE = 0x6E,
	RESOURCE_XPFL = 0x24
};

enum class eIMG_FileOrder
{
	INVALID,

	DFF,
	IFP,
	TXD,
	COL,
	IPL,
	CUT,
	DAT
};

enum eIMG_version
{
	IMG_VERSION_UNDEFINED,
	IMG_VERSION_1,	// GTA III, GTA VC, GTA III Mobile
	IMG_VERSION_2,	// GTA SA
	IMG_VERSION_3_UNENCRYPTED,	// GTA IV
	IMG_VERSION_3_ENCRYPTED		// GTA IV
};

class IMG;

class IMG_Entry
{
	friend class IMG;

private:
	wchar_t Name[256];				// NULL terminated
	DWORD NameHash;

	// In IMG archive
	DWORD Position;				// In blocks
	size_t SizeSecondPriority;	// In blocks
	size_t SizeFirstPriority;		// In blocks

	// Version 3
	DWORD SizeInBytes;
	// DWORD SizeInBlocks;
	eIMG_file_ResourceTypes ResourceType;

	// Updates hash of file name 
	void IMG_Entry::UpdateFileNameHash();

	// Get file size aligned to blocks
	size_t IMG_Entry::GetFilesizeAllignedToBlocks();

	// Gets file size in blocks
	size_t IMG_Entry::GetFilesizeInBlocks();

public:
	IMG* IMG_Instance;

	// For reading/writing
	unsigned int curPos;

	/////// Methods

	// Constructor
	IMG_Entry::IMG_Entry(IMG * IMG_Instance);

	// Returns pointer to file name.
	const wchar_t* IMG_Entry::GetFilename();

	// Writes a filename without extension to sufficiently long buffer
	void IMG_Entry::GetFilenameWithoutExtension(wchar_t* filenameWithoutExtension, int size);

	// Returns file order ID
	eIMG_FileOrder IMG_Entry::GetFileOrderIDbyExtension();

	// Reads whole file, doesn't affect current file position (curPos)
	// Returns number of bytes read.
	size_t IMG_Entry::ReadEntireFile(void* ptr);

	// Get file size in bytes
	size_t IMG_Entry::GetFilesize();

	//////// File operations that may affect curPos
	// Reads to memory
	size_t IMG_Entry::Read(void* Ptr, size_t Size);

	// Reads one byte
	char IMG_Entry::ReadC();

	// Write data
	size_t IMG_Entry::Write(void* ptr, size_t size);

	// Check End-of-File indicator
	bool IMG_Entry::isEOF();

	// Seek to certain position
	bool IMG_Entry::Seek(unsigned int offset, int origin);

	// Returns current position
	unsigned int IMG_Entry::Tell();

	// Outputs entry informations
	void IMG_Entry::DebugEntry(int fileID);
};

struct tIMG_FileOrder {
	eIMG_FileOrder orderID;
	const wchar_t* orderExtension;
	const wchar_t* orderName;
};

class IMG
{
	friend class IMG_Entry;

public:
	const static unsigned int UNDEFINED = 0xFFFFFFFF;

	struct AssociatedFilesMatch
	{
		AssociatedFilesMatch(DWORD FileID, DWORD IPL_num)
		{
			this->FileID = FileID;
			this->IPL_num = IPL_num;
		}

		DWORD FileID;
		DWORD IPL_num;
	};

	typedef std::list<IMG_Entry> tIMGentriesContainer;

	typedef tIMGentriesContainer::iterator tIMGEntryIterator;
	typedef tIMGentriesContainer::const_iterator tIMGEntryConstIterator;
	typedef tIMGentriesContainer::reference tIMGEntryReference;
	typedef tIMGentriesContainer::const_reference tIMGEntryConstReference;

private:
#ifdef IMG_DEBUG
	FILE* debugFile;
#endif

	// File entries
	tIMGentriesContainer ListOfEntries;
private:

	typedef std::vector<tIMGEntryIterator> tRelatedFilesContainer;

	FILE* IMG_Handle;
	FILE* DIR_Handle;

	wchar_t IMG_fullPath[MAX_PATH];
	wchar_t* IMG_filename;

	// Size of all filenames, only usable, when IMG version 3
	DWORD FilenamesSize;

	// Get end of list entries and header
	DWORD IMG::GetEndOfHeaderDataPastTheListOfFiles();

	// Gets IPL filename and num, returns true on success and false on failure.
	static bool IMG::GetIPLfilenameAndNum(const wchar_t* fullFilename, wchar_t* IPL_name, DWORD* IPL_num);

	// Gets ID of this file OR IDs of all .ipl files with this name
	void IMG::GetIteratorsOfAssociatedFiles(const wchar_t* SearchedName, tRelatedFilesContainer& list);

	// Loads files identified by DWORD indexes into continous aligned memory
	// Returned is size of all files loaded from list
	// Remember to free memory when it's no longer neccessary! Use delete!
	// None of addresses may be NULL
	void IMG::LoadFilesInCountinousAlignedMemory(tRelatedFilesContainer& FileIterators, void** retAddress, size_t* retSize, FILE* imgHandle = NULL);

	// Moves files located in position before list of files to suitable position
	void IMG::MoveFilesLocatedBeforeListOfFilesToSuitablePosition(FILE* imgHandle);

	// Returns pattern of IMG validity
	void IMG::GetPatternToCheckIfPositionIsValid(wchar_t* str);

	// Tests if file position is valid
	void IMG::TestIfPositionIsValid(FILE* imgHandle);

	// reads version 1 or 2 header
	void IMG::ReadEntryOfVersion1And2Header(IMG_Entry& entry, FILE* file);

	// writes version 1 or 2 header
	void IMG::WriteListOfEntriesVersion1And2(FILE* file);

	// Does action when the list of entries needs to be saved
	void IMG::DoModifiedListOfEntriesActions();

	// Does action when the archive has been modified
	void IMG::DoModifiedArchiveActions();

	// Updates list of entries in IMG archive/DIR file
	void IMG::WriteListOfEntries(FILE* imgHandle = nullptr);

	// Writes file content alligned to IMG_BLOCK_SIZE
	// Returned value is number of blocks written ( realSize / IMG_BLOCK_SIZE )
	size_t IMG::WriteAllignedFileToIMGblocks(const void* ptr, size_t size);

	// Sorts an array by
	// 1. Position
	// 2. Size
	void IMG::SortListOfEntriesByPositionAndSize(IMG::tIMGentriesContainer& list);

#ifdef IMG_DEBUG
public:
#endif
	// Finds a first free space for target file
	unsigned __int64 IMG::FindFirstEmptySpaceForFile(size_t filesize, tRelatedFilesContainer* overwrittenFileIndexes = NULL);

public:
	const static unsigned int IMG_BLOCK_SIZE = 2048;		// 2 KB
	const static unsigned int MAX_FILESIZE = 0xFFFF * IMG_BLOCK_SIZE;		// 128 MB - IMG_BLOCK_SIZE bytes
	const static unsigned int GTAIV_MAGIC_ID = 0xA94E2A52;

	wchar_t* GTAIV_encryption_key;

#pragma pack(push, 1)
	struct IMG_version2_tableItem		// size: 32 bytes
	{
		DWORD Position;				// In blocks
		size_t SizeSecondPriority;	// In blocks
		size_t SizeFirstPriority;		// In blocks
		char Name[24];				// NULL terminated
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct IMG_version3_header		// size: 20 bytes
	{
		DWORD MagicID;
		DWORD Version;				// always should be 3
		DWORD NumberOfItems;
		DWORD TableOfItemsSize;
		WORD SingleTableItemSize;	// should be 0x10 (16)
		WORD Unknown;
	};
#pragma pack(pop)

	struct IMG_version3_tableItem		// size: 20 bytes
	{
		DWORD SizeInBytes;
		eIMG_file_ResourceTypes ResourceType;
		DWORD Position;		// in blocks
		WORD SizeInBlocks;
		WORD Unknown;
	};

	eIMG_version archiveVersion;

	// true if archive was modified in any way, but rebuilding didn't happen
	bool bHasArchiveBeenModifiedAndNotRebuiltYet;

	// delays writing of list of entries
	bool bWritingOfEntriesPending;

	bool bUseManualWritingOfEntries;

	static tIMG_FileOrder fileOrderByExtension[];

	// Returns pointer to file order struct by ID or NULL
	static tIMG_FileOrder* IMG::getFileOrderStructByID(eIMG_FileOrder orderID);

	DWORD IMG_ENTRY_MAX_FILE_NAME_LENGTH;

private:
	// Writes IMG archive version 3 header to memory.
	void IMG::WriteVersion3HeaderToMemory(IMG_version3_header* header);

public:

	// Default constructor
	IMG::IMG();

	// Destructor
	IMG::~IMG();

	// Copy constructor
	// IMG::IMG(const IMG &cSource);

	// Opens IMG archive, assumes IMG archive to exist.
	// Detects archive type automatically
	bool IMG::OpenArchive(const wchar_t* path);

	// Creates .img archive
	// Example: object.CreateArchive("new.img", IMG::IMG_version::VERSION_1);
	bool IMG::CreateArchive(const wchar_t* path, eIMG_version version);

	// Opens .img archive if exists or creates if not exists
	// Example: object.OpenOrCreateArchive("new.img", IMG::IMG_version::VERSION_1);
	bool IMG::OpenOrCreateArchive(const wchar_t* path, eIMG_version version);

	// Checks if archive is currently opened.
	bool IMG::IsArchiveOpened();

	// Sets writing of list of IMG entries to be done on request
	void IMG::SetWritingListOfEntriesToBeDoneOnRequest(bool state);

	// Writes a list of entries
	void IMG::WriteListOfEntriesNow();

	// Closes archive
	void IMG::CloseArchive();

	// Rebuilds archive
	bool IMG::RebuildArchive();

	// Returns true if archive was modified, but not rebuilt yet.
	bool IMG::hasArchiveBeenModifiedAndNotRebuiltYet();

	// Gets the size of .img archive in bytes
	unsigned __int64 IMG::GetImgArchiveSize();

	// Gets size of unused space in .img file
	unsigned __int64 IMG::GetSizeOfUnusedSpace();

	// Retrieves the number of files inside of IMG archive
	DWORD IMG::GetFileCount();

	// Adds or replaces file if exists
	IMG::tIMGEntryIterator IMG::AddOrReplaceFile(const wchar_t* name, const void* ptr, size_t size);

	// Adds file
	IMG::tIMGEntryIterator IMG::AddFile(const wchar_t* name, const void* ptr, size_t size);

	// Replaces file depending on iterator
	void IMG::ReplaceSingleFile(tIMGEntryIterator IMGentryIt, const void* ptr, size_t size);

private:
	// Loop used in AddFile and ReplaceSingleFile functions
	void IMG::DoRelatedFilesLoop(tRelatedFilesContainer& RelatedFiles, tIMGEntryIterator newIMGentry, const void* ptr, size_t size);

	// Sets format properties
	void IMG::SetArchiveFormat(eIMG_version version);

public:

	// Renames a file
	bool IMG::RenameFile(tIMGEntryIterator fileInfo, const wchar_t* NewName);

	// Remove a file
	IMG::tIMGEntryConstIterator IMG::RemoveFile(tIMGEntryConstIterator _Where);

	// Removes files
	IMG::tIMGEntryConstIterator IMG::RemoveFiles(
		tIMGEntryConstIterator _First_arg,
		tIMGEntryConstIterator _Last_arg
		);

	// Gets iterator of file pointing to ListOfFiles
	IMG::tIMGEntryIterator IMG::GetFileIteratorByName(const wchar_t* name);

	// Checks if file with specified name exists and returns TRUE/FALSE
	bool IMG::FileExists(const wchar_t* name);

	// Checks if filename's length is appropriate for IMG entry as well as name characters.
	bool IMG::IsFileNameValid(const wchar_t* name);

	// Gets filename for imported file, filename may be truncated if archive version is 1 or 2.
	errno_t IMG::GetFilenameForImportedFile(const wchar_t* lpFileName, wchar_t* lpFilePart, DWORD nBufferLength);

	// Access file by name
	// Returns a reference to the last element in the vector container.
	IMG::tIMGEntryReference IMG::GetFileRefByName(const wchar_t* name);

	// Return iterator to beginning
	IMG::tIMGEntryIterator IMG::begin();

	// Return iterator to beginning
	IMG::tIMGEntryConstIterator IMG::begin() const;

	// Return iterator to end
	IMG::tIMGEntryIterator IMG::end();

	// Return iterator to end
	IMG::tIMGEntryConstIterator IMG::end() const;

	// Return reverse iterator to reverse beginning
	IMG::tIMGentriesContainer::reverse_iterator IMG::rbegin();

	// Return reverse iterator to reverse beginning
	IMG::tIMGentriesContainer::const_reverse_iterator IMG::rbegin() const;

	// Return reverse iterator to reverse end
	IMG::tIMGentriesContainer::reverse_iterator IMG::rend();

	// Return reverse iterator to reverse end
	IMG::tIMGentriesContainer::const_reverse_iterator IMG::rend() const;

	// Access last element
	// Returns a reference to the last element in the vector container.
	IMG::tIMGEntryReference IMG::back();

	// Access last element
	// Returns a reference to the last element in the vector container.
	IMG::tIMGEntryConstReference IMG::back() const;

	// Access first element
	// Returns a reference to the first element in the vector container.
	IMG::tIMGEntryReference IMG::front();

	// Access first element
	// Returns a reference to the first element in the vector container.
	IMG::tIMGEntryConstReference IMG::front() const;

	// Outputs list of entries specified as argument
	void IMG::DebugListOfEntries(tIMGentriesContainer& list);

	// Outputs ListOfEntries
	void IMG::DebugListOfEntries();
};