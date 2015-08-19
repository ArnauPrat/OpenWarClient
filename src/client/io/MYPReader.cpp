// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Code contributed by skreamz

#include "MYPReader.h"


#include "os.h"
#include "coreutil.h"

namespace owc 
{

namespace {
inline bool isHeaderValid(const SPAKFileHeader& header)
{
  return false;
}
}

//! Constructor
MYPArchiveLoader::MYPArchiveLoader( io::IFileSystem* fs)
: FileSystem(fs)
{
#ifdef _DEBUG
	setDebugName("MYPArchiveLoader");
#endif
}


//! returns true if the file maybe is able to be loaded by this class
bool MYPArchiveLoader::isALoadableFileFormat(const io::path& filename) const
{
	return core::hasFileExtension(filename, "myp");
}

//! Check to see if the loader can create archives of this type.
bool MYPArchiveLoader::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
{
	return fileType == EFAT_MYP;
}

//! Creates an archive from the filename
/** \param file File handle to check.
\return Pointer to newly created archive, or 0 upon error. */
IFileArchive* MYPArchiveLoader::createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	io::IReadFile* file = FileSystem->createAndOpenFile(filename);

	if (file)
	{
		archive = createArchive(file, ignoreCase, ignorePaths);
		file->drop ();
	}

	return archive;
}

//! creates/loads an archive from the file.
//! \return Pointer to the created archive. Returns 0 if loading failed.
IFileArchive* MYPArchiveLoader::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	if ( file )
	{
		file->seek ( 0 );
		archive = new MYPReader(file, ignoreCase, ignorePaths);
	}
	return archive;
}


//! Check if the file might be loaded by this class
/** Check might look into the file.
\param file File handle to check.
\return True if file seems to be loadable. */
bool MYPArchiveLoader::isALoadableFileFormat(io::IReadFile* file) const
{
	SPAKFileHeader header;

	file->read(&header, sizeof(header));

	return isHeaderValid(header);
}


/*!
	MYP Reader
*/
MYPReader::MYPReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
: CFileList((file ? file->getFileName() : io::path("")), ignoreCase, ignorePaths), File(file)
{
#ifdef _DEBUG
	setDebugName("MYPReader");
#endif

	if (File)
	{
    /** grab File */
	}
}


MYPReader::~MYPReader()
{
	if (File)
		File->drop();
}


const IFileList* MYPReader::getFileList() const
{
	return this;
}


//! opens a file by file name
IReadFile* MYPReader::createAndOpenFile(const io::path& filename)
{
	s32 index = findFile(filename, false);

	if (index != -1)
		return createAndOpenFile(index);

	return 0;
}


//! opens a file by index
IReadFile* MYPReader::createAndOpenFile(u32 index)
{
	if (index >= Files.size() )
		return 0;

	const SFileListEntry &entry = Files[index];
	return createLimitReadFile( entry.FullName, File, entry.Offset, entry.Size );
}

} // end namespace owc 

