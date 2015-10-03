// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Code contributed by skreamz

#include "CMYPReader.h"
#include "CFileSystem.h"
#include "MYPHash.h"
#include <inttypes.h>


#include "os.h"
#include "coreutil.h"
#include <string.h>


namespace irr 
{
  namespace io {

    namespace {

        inline bool isHeaderValid(const char header[4])
        {
            return strcmp(header, "MYP") == 0;
        }
    }

//! Constructor
CMYPArchiveLoader::CMYPArchiveLoader( io::IFileSystem* fs)
: FileSystem(fs)
{
#ifdef _DEBUG
	setDebugName("CMYPArchiveLoader");
#endif
}


//! returns true if the file maybe is able to be loaded by this class
bool CMYPArchiveLoader::isALoadableFileFormat(const io::path& filename) const
{
	return core::hasFileExtension(filename, "myp");
}

//! Check to see if the loader can create archives of this type.
bool CMYPArchiveLoader::isALoadableFileFormat( io::E_FILE_ARCHIVE_TYPE fileType) const
{
	return fileType == io::EFAT_MYP;
}

//! Creates an archive from the filename
/** \param file File handle to check.
\return Pointer to newly created archive, or 0 upon error. */
io::IFileArchive* CMYPArchiveLoader::createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const
{
    io::IFileArchive *archive = 0;
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
io::IFileArchive* CMYPArchiveLoader::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
    io::IFileArchive *archive = 0;
	if ( file )
	{
		file->seek ( 0 );
		archive = new CMYPReader(file, ignoreCase, ignorePaths);
	}
	return archive;
}


//! Check if the file might be loaded by this class
/** Check might look into the file.
\param file File handle to check.
\return True if file seems to be loadable. */
bool CMYPArchiveLoader::isALoadableFileFormat(io::IReadFile* file) const
{
	char header[4];

	file->read(&header, sizeof(header));
    header[3] = '\0';

	return isHeaderValid(header);
}


/*!
	MYP Reader
*/
CMYPReader::CMYPReader( io::IReadFile* file, bool ignoreCase, bool ignorePaths) :
 io::CFileList((file ? file->getFileName() : io::path("")), ignoreCase, ignorePaths), 
 File(file), MYPFileDatabase( file ) 
{
#ifdef _DEBUG
	setDebugName("CMYPReader");
#endif

	if (File)
	{
    /** grab File */
        File->grab();
        loadFiles();
        sort();
	}
}


CMYPReader::~CMYPReader()
{
	if (File)
		File->drop();
}

void CMYPReader::loadFiles() {
  if(MYPFileDatabase.loadArchive()) {
    printf("ERROR LOADING MYP ARCHIVE\n");
  }
  const core::array<SMYPFileDescriptor>* descriptors = MYPFileDatabase.getDescriptors(); 
  u32 index = 0;
  u64 size = descriptors->size();
  for( u64 i = 0; i < size; ++i ) {
    const SMYPFileDescriptor* descriptor = &((*descriptors)[i]);
    c8 fileName[17];

#ifdef _IRR_LINUX_PLATFORM_
    sprintf(fileName , "%lX", descriptor->hash);
#endif

#ifdef _IRR_WINDOWS_
    sprintf(fileName, "%I64X", descriptor->hash);
#endif

    addItem(io::path(fileName), descriptor->startingPosition, descriptor->uncompressedSize, false, index++ );
  }
}


const io::IFileList* CMYPReader::getFileList() const
{
	return this;
}


//! opens a file by file name
io::IReadFile* CMYPReader::createAndOpenFile(const io::path& filename)
{

  unsigned int ph = 0;
  unsigned int sh = 0;
  hashlittle2(filename.c_str(), filename.size(), &sh, &ph); 
  unsigned long long result  = ((unsigned long long)ph << 32) + sh;
  c8 file_name[17];

#ifdef _IRR_LINUX_PLATFORM_
        sprintf(file_name, "%llX", result);
#endif

#ifdef _IRR_WINDOWS_
  sprintf(file_name, "%I64X", result);
#endif

  s32 index = findFile(file_name, false);

  if (index != -1) {
    return createAndOpenFile(index);
  }
  return 0;
}


//! opens a file by index
io::IReadFile* CMYPReader::createAndOpenFile(u32 index)
{
	if (index >= Files.size() )
		return 0;

	const io::SFileListEntry &entry = Files[index];
    // Get additional information regarding MYP files.
    SMYPFileDescriptor desc = MYPFileDatabase.getFileDescriptor(entry.ID);
    if( desc.compressionMethod ) {
        unsigned char* data;
        u64 dataSize;
        MYPFileDatabase.getFileData(desc, &data, &dataSize);
        return io::createMemoryReadFile((void*)data, desc.uncompressedSize, entry.FullName,true);
    } else {
        return io::createLimitReadFile( entry.FullName, File, entry.Offset, entry.Size );
    }
}
} // end namespace io 

} // end namespace irr 

