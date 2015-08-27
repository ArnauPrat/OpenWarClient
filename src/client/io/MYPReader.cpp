// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Code contributed by skreamz

#include "MYPReader.h"
#include "CFileSystem.h"
#include "hash.h"


#include "os.h"
#include "coreutil.h"
#include <string.h>

using namespace irr;

namespace owc 
{

    namespace {

        inline bool isHeaderValid(const char header[4])
        {
            return strcmp(header, "MYP") == 0;
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
bool MYPArchiveLoader::isALoadableFileFormat( io::E_FILE_ARCHIVE_TYPE fileType) const
{
	return fileType == io::EFAT_MYP;
}

//! Creates an archive from the filename
/** \param file File handle to check.
\return Pointer to newly created archive, or 0 upon error. */
io::IFileArchive* MYPArchiveLoader::createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const
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
io::IFileArchive* MYPArchiveLoader::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
    io::IFileArchive *archive = 0;
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
	char header[4];

	file->read(&header, sizeof(header));
    header[3] = '\0';

	return isHeaderValid(header);
}


/*!
	MYP Reader
*/
MYPReader::MYPReader( io::IReadFile* file, bool ignoreCase, bool ignorePaths) :
 io::CFileList((file ? file->getFileName() : io::path("")), ignoreCase, ignorePaths), 
 File(file)
{
#ifdef _DEBUG
	setDebugName("MYPReader");
#endif

	if (File)
	{
    /** grab File */
        File->grab();
        load_files();
        sort();
	}
}


MYPReader::~MYPReader()
{
	if (File)
		File->drop();
}

void MYPReader::load_files() {
    myp_file_database.load_archive(File->getFileName().c_str());
    const std::vector<MYPFileDescriptor>* descriptors = myp_file_database.get_descriptors(); 
    u32 index = 0;
    for( std::vector<MYPFileDescriptor>::const_iterator it = descriptors->begin(); it != descriptors->end(); ++it) {
        c8 file_name[9];
        sprintf(file_name, "%llu", it->hash);
		addItem(io::path(file_name), it->starting_position, it->uncompressed_size, false, index++ );
    }
}


const io::IFileList* MYPReader::getFileList() const
{
	return this;
}


//! opens a file by file name
io::IReadFile* MYPReader::createAndOpenFile(const io::path& filename)
{

    unsigned int ph = 0;
    unsigned int sh = 0;
    hashlittle2(filename.c_str(), filename.size(), &sh, &ph); 
    unsigned long long result  = ((unsigned long long)ph << 32) + sh;
    c8 file_name[9];
    sprintf(file_name, "%llu", result);
	s32 index = findFile(file_name, false);

	if (index != -1)
		return createAndOpenFile(index);

	return 0;
}


//! opens a file by index
io::IReadFile* MYPReader::createAndOpenFile(u32 index)
{
	if (index >= Files.size() )
		return 0;

	const io::SFileListEntry &entry = Files[index];
    // Get additional information regarding MYP files.
    MYPFileDescriptor desc = myp_file_database.get_file_descriptor(entry.ID);
    if( desc.compression_method ) {
        unsigned char* data;
        myp_file_database.get_file_data(&desc,&data);
        return io::createMemoryReadFile((void*)data, desc.uncompressed_size, entry.FullName,true);
    } else {
        return io::createLimitReadFile( entry.FullName, File, entry.Offset, entry.Size );
    }
}

} // end namespace owc 

