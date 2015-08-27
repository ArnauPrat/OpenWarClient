

#ifndef MYPFILE_DATABASE_H
#define MYPFILE_DATABASE_H

#include <map>
#include <string>
#include <vector>
#include "../common/macros.h"

namespace owc {

#define MYP_ERROR_EOF               0x01
#define MYP_ERROR_NFILES_MISSMATCH  0x02



  struct MYPFileDescriptor {

    unsigned long long  table_entry_position;
    unsigned long long  starting_position;
    unsigned long long  hash;
    unsigned int        header_size;
    unsigned int        compressed_size;
    unsigned int        uncompressed_size;
    unsigned char       compression_method;
    unsigned char       archive_index;

  };

  struct MYPFileDatabaseStats {
    public:
      int num_files_;
  };


  class MYPFileDatabase {
    OWC_NON_COPYABLE(MYPFileDatabase);

    friend class MYPReader;

    public:

      MYPFileDatabase();
      ~MYPFileDatabase();

      int load_archive( const char* file_name );

      int load_hash_dictionary( const char* file_name );

      int extract( const char* path );

      MYPFileDatabaseStats get_stats() const;

      int get_file_data(const char* file_name, unsigned char** data);

      int free_file_data( unsigned char* data);


    private:

      const std::vector<MYPFileDescriptor>* get_descriptors() const { return &file_descriptors_; }

      MYPFileDescriptor get_file_descriptor( int index ) const { return file_descriptors_[index]; } 

      int get_file_data(const MYPFileDescriptor* file_descriptor, unsigned char** data);

      void add_hash_to_filename_entry( const char* line );

    private:
      std::vector<char*>                          archive_names_; 
      std::vector<FILE*>                          archive_files_;                                       
      std::vector<MYPFileDescriptor>                 file_descriptors_;
      std::map<unsigned long long, std::string>   hash_to_file_names_;
  };

}

#endif
