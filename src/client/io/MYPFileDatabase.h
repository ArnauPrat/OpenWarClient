

#ifndef MYPFILE_DATABASE_H
#define MYPFILE_DATABASE_H

#include <map>
#include <string>
#include <vector>
#include "../common/macros.h"

namespace owc {

#define MYP_ERROR_EOF               0x01
#define MYP_ERROR_NFILES_MISSMATCH  0x02
#define MYP_ERROR_FILE_NOT_FOUND    0x03


  struct FileDescriptor;

  struct MYPFileDatabaseStats {
    public:
      int num_files_;
  };


  class MYPFileDatabase {
    OWC_NON_COPYABLE(MYPFileDatabase);

    public:

      MYPFileDatabase();
      ~MYPFileDatabase();

      int load_archive( const char* file_name );

      int load_hash_dictionary( const char* file_name );

      int extract( const char* path );

      int extract( const char* file_name, const char* path );

      MYPFileDatabaseStats get_stats() const;

      int get_file_data(const char* file_name, unsigned char** data, size_t* data_size);

      int free_file_data( unsigned char* data);

    private:

      int get_file_data(const FileDescriptor* file_descriptor, unsigned char** data, size_t* data_size);

      void add_hash_to_filename_entry( const char* line );

      int extract( const FileDescriptor* file_descriptor, const char* path, const char* output_file_name = NULL );

    private:
      std::vector<char*>                          archive_names_; 
      std::vector<FILE*>                          archive_files_;                                       
      std::vector<FileDescriptor>                 file_descriptors_;
      std::map<unsigned long long, std::string>   hash_to_file_names_;
  };

}

#endif
