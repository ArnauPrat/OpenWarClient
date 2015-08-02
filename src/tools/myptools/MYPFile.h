

#ifndef MYPFILE_H
#define MYPFILE_H

#include <map>
#include <string>
#include <vector>

#define MYP_ERROR_EOF               0x01
#define MYP_ERROR_NFILES_MISSMATCH  0x02


struct FileDescriptor;

struct MYPFileStats {
  public:
    int num_files_;
};

class MYPFile {

  public:

    MYPFile();
    ~MYPFile();

    int load( const char* file_name );

    int load_hash_dictionary( const char* file_name );

    int extract( const char* path );

    MYPFileStats get_stats() const;

  private:

    MYPFile(const MYPFile& file);
    MYPFile& operator=(const MYPFile& file);

    void add_hash_to_filename_entry( const char* line );


  private:

    const char*                                 archive_name_; 
    std::vector<FileDescriptor>                 file_descriptors_;
    std::map<unsigned long long, std::string>   hash_to_file_names_;

};

#endif
