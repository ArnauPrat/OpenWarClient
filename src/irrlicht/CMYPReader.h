// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __MYPREADER_H_INCLUDED__ 
#define __MYPREADER_H_INCLUDED__ 


#include <IFileArchive.h>
#include <IFileSystem.h>
#include <CFileList.h>
#include "CMYPFileDatabase.h"


namespace irr  {

  namespace io {

    //! Archiveloader capable of loading MYP Archives
    class CMYPArchiveLoader : public io::IArchiveLoader
    {
      public:

        //! Constructor
        CMYPArchiveLoader(io::IFileSystem* fs);


        //! returns true if the file maybe is able to be loaded by this class
        //! based on the file extension (e.g. ".zip")
        virtual bool isALoadableFileFormat(const io::path& filename) const;

        //! Check if the file might be loaded by this class
        /** Check might look into the file.
          \param file File handle to check.
          \return True if file seems to be loadable. */
      virtual bool isALoadableFileFormat( io::IReadFile* file) const;

      //! Check to see if the loader can create archives of this type.
      /** Check based on the archive type.
        \param fileType The archive type to check.
        \return True if the archile loader supports this type, false if not */
      virtual bool isALoadableFileFormat( io::E_FILE_ARCHIVE_TYPE fileType) const;

      //! Creates an archive from the filename
      /** \param file File handle to check.
        \return Pointer to newly created archive, or 0 upon error. */
      virtual io::IFileArchive* createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const;

      //! creates/loads an archive from the file.
      //! \return Pointer to the created archive. Returns 0 if loading failed.
      virtual io::IFileArchive* createArchive( io::IReadFile* file, bool ignoreCase, bool ignorePaths) const;

      //! Returns the type of archive created by this loader
      virtual io::E_FILE_ARCHIVE_TYPE getType() const { return io::EFAT_MYP; }

      private:
      io::IFileSystem* FileSystem;
    };


    //! reads from myp 
    class CMYPReader : public virtual io::IFileArchive, virtual io::CFileList
    {
      public:

        CMYPReader( io::IReadFile* file, bool ignoreCase, bool ignorePaths);
        virtual ~CMYPReader();

        //! return the id of the file Archive
        virtual const io::path& getArchiveName() const
        {
          return File->getFileName();
        }

        //! opens a file by file name
        virtual io::IReadFile* createAndOpenFile(const io::path& filename);

        //! opens a file by index
        virtual io::IReadFile* createAndOpenFile(u32 index);

        //! returns the list of files
        virtual const io::IFileList* getFileList() const;

        //! get the class Type
        virtual io::E_FILE_ARCHIVE_TYPE getType() const { return io::EFAT_MYP; }

      private:

        void loadFiles();

        io::IReadFile* File;
        CMYPFileDatabase MYPFileDatabase;

    };
  }
}

#endif 

