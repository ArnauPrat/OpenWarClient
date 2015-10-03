

#ifndef __C_MYPFILE_DATABASE_H_INCLUDED__
#define __C_MYPFILE_DATABASE_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef __IRR_COMPILE_WITH_MYP_ARCHIVE_LOADER_

#include <map>
#include "irrMacros.h"
#include "irrTypes.h"
#include "irrString.h"
#include "irrArray.h"
#include "IReadFile.h"

namespace irr {
  namespace io {

#define MYP_ERROR_EOF               0x01
#define MYP_ERROR_NFILES_MISSMATCH  0x02
#define MYP_ERROR_FILE_NOT_FOUND    0x03

    struct SMYPFileDescriptor {

      u64  tableEntryPosition;
      u64  startingPosition;
      u64  hash;
      u32  headerSize;
      u32  compressedSize;
      u32  uncompressedSize;
      u8   compressionMethod;
    };

    struct SMYPFileDatabaseStats {
      public:
        s32 numFiles;
    };


    class CMYPFileDatabase {
      IRR_NON_COPYABLE(CMYPFileDatabase);

      friend class CMYPReader;

      public:

      CMYPFileDatabase( IReadFile* file );
      ~CMYPFileDatabase();

      u32 loadArchive();

      u32 extract( const io::path& fileName );

      u32 extract( const io::path& fileName, const io::path& outputFolderName );

      SMYPFileDatabaseStats getStats() const;

      u32 getFileData(const io::path& fileName, u8** data, u64* dataSize);

      u32 freeFileData( u8* data);


      private:

      const core::array<SMYPFileDescriptor>* getDescriptors() const { return &FileDescriptors; }

      SMYPFileDescriptor getFileDescriptor( u32 index ) const { return FileDescriptors[index]; } 

      u32 getFileData(const SMYPFileDescriptor& fileDescriptor, u8** data, u64* dataSize);

      //void add_hash_to_filename_entry( const c8* line );

      u32 extract( const SMYPFileDescriptor& file_descriptor, const io::path& outputFolderName, const io::path& fileName );

      private:
      io::IReadFile*                              File;
      core::array<SMYPFileDescriptor>             FileDescriptors;
    };
  }
}

#endif
#endif
