

#ifndef __C_MYPFILE_DATABASE_H_INCLUDED__
#define __C_MYPFILE_DATABASE_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef __IRR_COMPILE_WITH_MYP_ARCHIVE_LOADER_

#include <map>
#include "irrMacros.h"
#include "irrTypes.h"
#include "irrString.h"
#include "irrArray.h"

namespace irr {
  namespace io {

#define MYP_ERROR_EOF               0x01
#define MYP_ERROR_NFILES_MISSMATCH  0x02
#define MYP_ERROR_FILE_NOT_FOUND    0x03

        struct SMYPFileDescriptor {

            u64  table_entry_position;
            u64  starting_position;
            u64  hash;
            u32  header_size;
            u32  compressed_size;
            u32  uncompressed_size;
            u8   compression_method;
            u8   archive_index;

        };

        struct SMYPFileDatabaseStats {
            public:
                s32 num_files_;
        };


        class CMYPFileDatabase {
            IRR_NON_COPYABLE(CMYPFileDatabase);

            friend class CMYPReader;

            public:

            CMYPFileDatabase();
            ~CMYPFileDatabase();

            u32 load_archive( const c8* file_name );

            //u32 load_hash_dictionary( const c8* file_name );

            u32 extract( const c8* path );

            u32 extract( const c8* file_name, const c8* path );

            SMYPFileDatabaseStats get_stats() const;

            u32 get_file_data(const c8* file_name, u8** data, u64* data_size);

            u32 free_file_data( u8* data);


            private:

            const core::array<SMYPFileDescriptor>* get_descriptors() const { return &file_descriptors_; }

            SMYPFileDescriptor get_file_descriptor( u32 index ) const { return file_descriptors_[index]; } 

            u32 get_file_data(const SMYPFileDescriptor* file_descriptor, u8** data, u64* data_size);

            //void add_hash_to_filename_entry( const c8* line );

            u32 extract( const SMYPFileDescriptor* file_descriptor, const c8* path, const c8* output_file_name = NULL );

            private:
            core::array<c8*>                            archive_names_; 
            core::array<FILE*>                          archive_files_;                                       
            core::array<SMYPFileDescriptor>             file_descriptors_;
            //std::map<u64, core::stringc>                      hash_to_file_names_;
        };
}
}

#endif
#endif
