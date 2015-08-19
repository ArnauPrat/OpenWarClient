// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef MYPREADER_H 
#define MYPREADER_H 


#include <IFileArchive.h>
#include <IArchiveLoader.h>
#include <CFileList.h>
#include "MYPFileDatabase.h"


namespace owc  {

	//! Archiveloader capable of loading MYP Archives
	class MYPArchiveLoader : public irr::io::IArchiveLoader
	{
	public:

		//! Constructor
		MYPArchiveLoader(irr::io::IFileSystem* fs);

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".zip")
		virtual bool isALoadableFileFormat(const irr::io::path& filename) const;

		//! Check if the file might be loaded by this class
		/** Check might look into the file.
		\param file File handle to check.
		\return True if file seems to be loadable. */
		virtual bool isALoadableFileFormat(irr::io::IReadFile* file) const;

		//! Check to see if the loader can create archives of this type.
		/** Check based on the archive type.
		\param fileType The archive type to check.
		\return True if the archile loader supports this type, false if not */
		virtual bool isALoadableFileFormat(irr::io::E_FILE_ARCHIVE_TYPE fileType) const;

		//! Creates an archive from the filename
		/** \param file File handle to check.
		\return Pointer to newly created archive, or 0 upon error. */
		virtual IFileArchive* createArchive(const irr::io::path& filename, bool ignoreCase, bool ignorePaths) const;

		//! creates/loads an archive from the file.
		//! \return Pointer to the created archive. Returns 0 if loading failed.
		virtual io::IFileArchive* createArchive(irr::io::IReadFile* file, bool ignoreCase, bool ignorePaths) const;

		//! Returns the type of archive created by this loader
		virtual irr::io::E_FILE_ARCHIVE_TYPE getType() const { return irr::io::EFAT_MYP; }

	private:
    irr::io::IFileSystem* FileSystem;
	};


	//! reads from myp 
	class MYPReader : public virtual irr::io::IFileArchive, virtual irr::io::CFileList
	{
	public:

		MYPReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~MYPReader();

		//! return the id of the file Archive
		virtual const irr::io::path& getArchiveName() const
		{
			return File->getFileName();
		}

		//! opens a file by file name
		virtual irr::io::IReadFile* createAndOpenFile(const irr::io::path& filename);

		//! opens a file by index
		virtual irr::io::IReadFile* createAndOpenFile(u32 index);

		//! returns the list of files
		virtual const irr::io::IFileList* getFileList() const;

		//! get the class Type
		virtual irr::io::E_FILE_ARCHIVE_TYPE getType() const { return irr::io::EFAT_MYP; }

	private:

    irr::io::IReadFile* File;
    MYPFileDatabase myp_file_database;

	};
} 

#endif 

