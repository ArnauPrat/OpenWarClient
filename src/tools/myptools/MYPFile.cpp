
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MYPFile.h"
#include "hash.h"

#include <zlib.h>

#include <sys/types.h>
#include <sys/stat.h>


#define MYP_FILE_DESCRIPTOR_SIZE 34
#define MYP_TABLE_BLOCK_HEADER_SIZE 12
#define MYP_ARCHIVE_HEADER_SIZE 16
#define MYP_ARCHIVE_HEADER_START_POSITION 0x0C 
#define MYP_ARCHIVE_HEADER_NUM_FILES_POSITION 0x18
#define MYP_EXPECTED_NUM_FILES_PER_BLOCK 1000

#define LINE_BUFFER_BLOCK_SIZE 8

#define MYP_NUM_PREFIXES 3
static const char* name_prefixes[MYP_NUM_PREFIXES] = {"sk.0.0.", "it.0.0.", "fi.0.0." };


/*
 * Struct definitions
 */

struct FileDescriptor {

  unsigned long long  table_entry_position;
  unsigned long long  starting_position;
  unsigned long long  hash;
  unsigned int        header_size;
  unsigned int        compressed_size;
  unsigned int        uncompressed_size;
  unsigned char       compression_method;

};

struct TableBlockHeader {

  unsigned int        num_files;
  unsigned long long  next_block;

};


/*
 * Static function definitions
 */

static unsigned int parse_uint( const unsigned char* buffer, long offset ) {

    unsigned int result;
    memcpy(&result,buffer+offset,sizeof(unsigned int));
    return result;

}

static unsigned long long parse_ulong( const unsigned char* buffer, long offset ) {

    unsigned long long result;
    memcpy(&result,buffer+offset,sizeof(unsigned long long));
    return result;

}


static void initialize_FileDescriptor(FileDescriptor* descriptor, const unsigned char* buffer, unsigned int offset) {

    descriptor->starting_position   = parse_ulong(buffer, offset + 0);            
    descriptor->header_size         = parse_uint(buffer, offset + 8);
    descriptor->compressed_size     = parse_uint(buffer, offset + 12);
    descriptor->uncompressed_size   = parse_uint(buffer, offset + 16);
    unsigned int secondary_hash      = parse_uint(buffer, offset + 20);
    unsigned long long primary_hash        = parse_uint(buffer, offset + 24);
    descriptor->hash = (primary_hash << 32) + secondary_hash;
    descriptor->compression_method  = buffer[offset + 32];

}

static void initialize_TableBlockHeader( TableBlockHeader* block_header, const unsigned char* buffer ) {

    block_header->num_files  = parse_uint(buffer,0);
    block_header->next_block = parse_ulong(buffer,4);
}

/*
 * Class methods
 */

MYPFile::MYPFile() :
  archive_name_(NULL){
}

MYPFile::~MYPFile() {
  if(archive_name_) {
    free((void*)archive_name_);
  }
}

int MYPFile::load( const char* file_name) {

    FILE* fp = fopen(file_name, "rb");

    unsigned char archive_header_buffer[MYP_ARCHIVE_HEADER_SIZE];

    /* Obtain the position of the first table block */ 
    if(fseek(fp,MYP_ARCHIVE_HEADER_START_POSITION,SEEK_SET)) 
        return MYP_ERROR_EOF;

    fread(archive_header_buffer, MYP_ARCHIVE_HEADER_SIZE, 1, fp);
    if( feof(fp) )
        return MYP_ERROR_EOF;

    unsigned long long next_table_block_header = parse_ulong(archive_header_buffer,0);

     
    /* Get total number of files in the archive */
    if(fseek(fp,MYP_ARCHIVE_HEADER_NUM_FILES_POSITION,SEEK_SET)) 
        return MYP_ERROR_EOF;

    unsigned char buffer[4];
    fread(buffer,4,1,fp);
    if( feof(fp) ) 
        return MYP_ERROR_EOF;

    unsigned int num_total_files = parse_uint(buffer,0);

    /* Buffer to store myp table block header  */
    unsigned char myp_table_block_header_buffer[MYP_TABLE_BLOCK_HEADER_SIZE];

    /* Create buffers to store file descriptors. Initial size to 1000 as it is typically observed in practice.  */
    unsigned int myp_file_descriptor_buffer_size = MYP_EXPECTED_NUM_FILES_PER_BLOCK * MYP_FILE_DESCRIPTOR_SIZE;
    unsigned char* myp_file_descriptor_buffer = (unsigned char*)malloc(sizeof(unsigned char)*myp_file_descriptor_buffer_size);
    FileDescriptor aux_file_descriptor;

    while( next_table_block_header != 0 ) {

        /* We read the next table block header */
        if(fseek(fp,next_table_block_header,SEEK_SET)) 
          return MYP_ERROR_EOF;

        int bytes_read = fread(myp_table_block_header_buffer, MYP_TABLE_BLOCK_HEADER_SIZE, 1, fp);
        if( feof(fp) && bytes_read != MYP_TABLE_BLOCK_HEADER_SIZE ) 
          return MYP_ERROR_EOF;

        TableBlockHeader table_block_header;
        initialize_TableBlockHeader(&table_block_header, myp_table_block_header_buffer);

        /* Read file descriptors*/
        if( table_block_header.num_files*MYP_FILE_DESCRIPTOR_SIZE > myp_file_descriptor_buffer_size ) { // resizing buffer if needed
            free(myp_file_descriptor_buffer);
            myp_file_descriptor_buffer_size = table_block_header.num_files*MYP_FILE_DESCRIPTOR_SIZE;
            myp_file_descriptor_buffer = (unsigned char*)malloc(myp_file_descriptor_buffer_size);
        }

        if( fread(myp_file_descriptor_buffer, MYP_FILE_DESCRIPTOR_SIZE, table_block_header.num_files, fp) != table_block_header.num_files ) 
            return MYP_ERROR_EOF;

        for ( int i = 0; i < table_block_header.num_files; ++i) {
            initialize_FileDescriptor(&aux_file_descriptor,myp_file_descriptor_buffer, i*MYP_FILE_DESCRIPTOR_SIZE);
            if( aux_file_descriptor.starting_position > 0 ) 
              file_descriptors_.push_back(aux_file_descriptor); 
        }

        next_table_block_header = table_block_header.next_block;

    }
    free(myp_file_descriptor_buffer);
    fclose(fp);

    if ( num_total_files != file_descriptors_.size() )
      return MYP_ERROR_NFILES_MISSMATCH;
    archive_name_ = (const char*)malloc(sizeof(char)*(strlen(file_name)+1));
    strcpy((char *)archive_name_, file_name);
    return 0;

}


int MYPFile::load_hash_dictionary( const char* file_name ) {

  FILE* fp = fopen(file_name, "r");

  int line_buffer_size = LINE_BUFFER_BLOCK_SIZE;
  char* line_buffer = (char*)malloc(sizeof(char)*LINE_BUFFER_BLOCK_SIZE);

  while( (line_buffer = fgets(line_buffer, line_buffer_size, fp)) ) {
    int line_size = strlen(line_buffer);
    while( line_size == (line_buffer_size-1) &&
           line_buffer[line_buffer_size - 2] != '\n' && 
           !feof(fp) ) { //resizing the buffer and filling with more data in case we do not found an eof
      line_buffer_size += LINE_BUFFER_BLOCK_SIZE;
      line_buffer = (char*) realloc(line_buffer, line_buffer_size );
      fgets(&line_buffer[line_buffer_size - LINE_BUFFER_BLOCK_SIZE - 1], LINE_BUFFER_BLOCK_SIZE + 1, fp);  
      line_size = strlen(line_buffer);
    }
    add_hash_to_filename_entry(line_buffer);
  }
  free(line_buffer);
  fclose(fp);
  return 0;
}

void MYPFile::add_hash_to_filename_entry( const char* line ) {
  char* next_sharp; 
  long int ph = strtol(line,&next_sharp,16);
  long int sh = strtol(next_sharp+1,&next_sharp,16);
  long long int full_hash = (ph << 32) + sh;
  char* last_sharp = strchr(next_sharp+1,'#');
  hash_to_file_names_.insert(std::pair<unsigned long long, std::string > ( full_hash,
                                                                           std::string(next_sharp+1,last_sharp) ) );
}

static void find_extension(const unsigned char* data, char* ext) {

  sprintf(ext,"%s","txt"); 

  char buff[4];
  memcpy((unsigned char*)buff, data, sizeof(unsigned char)*4);

  if (buff[0] == 0 && buff[1] == 1 && buff[2] == 0)
  {
    sprintf(ext,"%s","ttf"); 
  }
  else if (buff[0] == 0x0a && buff[1] == 0x05 && buff[2] == 0x01 && buff[3] == 0x08)
  {
    sprintf(ext,"%s","pcx"); 
  }
  else if (strstr(buff,"PK") )
  {
    sprintf(ext,"%s","zip"); 
  }
  else if (strstr(buff,"SCPT") )
  {
    sprintf(ext,"%s","scpt"); 
  }
  else if (strstr(buff,"<") )
  {
    sprintf(ext,"%s","xml"); 
  }
  else if ( strstr((char*)data, "lua") && ( strstr((char*)data, "lua") - (const char*)data) < 50)
  {
    sprintf(ext,"%s","lua"); 
  }
  else if (strstr(buff,"DDS") )
  {
    sprintf(ext,"%s","dds"); 
  }
  else if (strstr(buff,"XSM") )
  {
    sprintf(ext,"%s","xsm"); 
  }
  else if (strstr(buff,"XAC") )
  {
    sprintf(ext,"%s","xac"); 
  }
  else if (strstr(buff,"8BPS") )
  {
    sprintf(ext,"%s","8bps"); 
  }
  else if (strstr(buff,"bdLF") )
  {
    sprintf(ext,"%s","db"); 
  }
  else if (strstr(buff,"gsLF") )
  {
    sprintf(ext,"%s","geom"); 
  }
  else if (strstr(buff,"idLF") )
  {
    sprintf(ext,"%s","diffuse"); 
  }
  else if (strstr(buff,"psLF") )
  {
    sprintf(ext,"%s","specular"); 
  }
  else if (strstr(buff,"amLF") )
  {
    sprintf(ext,"%s","mask"); 
  }
  else if (strstr(buff,"ntLF") )
  {
    sprintf(ext,"%s","tint"); 
  }
  else if (strstr(buff,"lgLF") )
  {
    sprintf(ext,"%s","glow"); 
  }
  else if (strstr((char*)data, "Gamebry") )
  {
    sprintf(ext,"%s","nif"); 
  }
  else if (strstr((char*)data,"WMPHOTO") )
  {
    sprintf(ext,"%s","lmp"); 
  }
  else if (strstr(buff,"RIFF") )
  {
    char buff2[4];
    memcpy(buff2, &data[8], sizeof(unsigned char)*4);
    if (strstr(buff2,"WAVE") )
    {
      sprintf(ext,"%s","wav"); 
    }
    else
    {
      sprintf(ext,"%s","riff"); 
    }
  }
  else if (strstr(buff,"; Zo") )
  {
    sprintf(ext,"%s","zone.txt"); 
  }
  else if (strstr(buff,"\0\0\0\0") )
  {
    sprintf(ext,"%s","zero.txt"); 
  }
  else if (strstr(buff,"PNG") )
  {
    sprintf(ext,"%s","png"); 
  }
  else if (strstr(buff,"AMX") )
  {
    sprintf(ext,"%s","amx"); 
  }
  else if (strstr(buff,"SIDS") )
  {
    sprintf(ext,"%s","sids"); 
  }
 /* else if (commaNum >= 10)
  {
    ext = "csv";
  }
  */
  
}

int MYPFile::extract( const char* path ) {
  
  if( archive_name_ ) {
    FILE* fp = fopen(archive_name_,"rb");
    int num_files = file_descriptors_.size();
    for ( int i = 0; i < num_files; ++i ) {

      FileDescriptor* file_descriptor = &file_descriptors_[i];

      unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*file_descriptor->uncompressed_size);;
      if( file_descriptor->compression_method != 0 ) {
        unsigned char* compressed_buffer = (unsigned char*)malloc(sizeof(unsigned char)*file_descriptor->compressed_size);
        fseek(fp,file_descriptor->starting_position + file_descriptor->header_size, SEEK_SET);
        fread(compressed_buffer, sizeof(unsigned char), file_descriptor->compressed_size, fp);
        z_stream zstr;
        zstr.next_in = compressed_buffer;
        zstr.avail_in = file_descriptor->compressed_size;
        zstr.next_out = data;
        zstr.avail_out = file_descriptor->uncompressed_size;
        zstr.zalloc = Z_NULL;
        zstr.zfree = Z_NULL;
        zstr.opaque = Z_NULL;
        if ( inflateInit(&zstr) != Z_OK ) fprintf(stderr, "Error when initializing zlib\n");
        int ret = inflate(&zstr,Z_FINISH) ; 
        if ( ret != Z_OK && ret != Z_STREAM_END ) {
          fprintf(stderr, "Error when inflating compressed buffer: %d\n", ret);
        }
        if ( inflateEnd(&zstr) != Z_OK ) fprintf(stderr, "Error when ending zlib\n");
        free(compressed_buffer);
      } else {
        fseek(fp,file_descriptor->starting_position + file_descriptor->header_size, SEEK_SET);
        fread(data, sizeof(unsigned char), file_descriptor->uncompressed_size, fp);
      }

      char* file_name = NULL;
      std::map<unsigned long long, std::string>::iterator it = hash_to_file_names_.find( file_descriptor->hash );
      if( it != hash_to_file_names_.end() ) {
        
        const char* str  = it->second.c_str();
        int str_length = strlen(str);
        file_name= (char*)malloc(sizeof(char)*(str_length+1));
        strcpy((char*)file_name, str);

        if( str_length > 0 ) {
          const char* actual_file_name_start = strrchr((char*)file_name, '/');
          if( actual_file_name_start != NULL ) {
            actual_file_name_start++;
          } else {
            actual_file_name_start = file_name;
          }
          int pre_length = 0;
          int pre = 0;
          for (int j = 0; j < MYP_NUM_PREFIXES; ++j ) {
            int length = strlen(name_prefixes[j]);
            if( strncmp(actual_file_name_start, name_prefixes[j], length) == 0 ) {
              pre_length = length;
              pre = j;
              break;
            }
          }

          if ( pre < MYP_NUM_PREFIXES ) {
            strcpy((char*)(actual_file_name_start),(char*)(str+(actual_file_name_start - file_name) + pre_length));

            const char* last_dot_pos = strrchr(file_name, '.');
            if( last_dot_pos && (strcmp((char*)(last_dot_pos+1), "stx") == 0)) {
              file_name[str_length- 3 - pre_length] = 'd';
              file_name[str_length- 3 - pre_length + 1] = 'd';
              file_name[str_length- 3 - pre_length + 2] = 's';
            }

          }
        }

      }

      
      if( !file_name || strlen(file_name) == 0 ) {

        char ext[16];
        find_extension(data,ext);

        file_name = (char*)malloc(sizeof(char)*(8+16+1)); // hash_name + ext (16) + null_char     
        sprintf((char*)file_name,"%llX.%s", file_descriptor->hash, ext);

      }

      char current_folder[3] = {'.','/', '\0'};
      if( path == NULL || (strlen(path) == 0) ) {
        path = current_folder;
      }

      int path_size = strlen(path);
      int file_name_size = strlen(file_name);
      int final_file_name_length = path_size+file_name_size;
      char* final_file_name = (char*)malloc(sizeof(char*)*final_file_name_length+1);
      sprintf(final_file_name, "%s/%s", path, file_name);

      /* Creating folders from path */
      char* sub_path = (char*)malloc(sizeof(char)*final_file_name_length+1);
      char* next_token = strchr((char*)final_file_name,'/');
      while(next_token) {
        next_token++;
        int end = next_token - final_file_name;
        strncpy(sub_path, final_file_name, end );
        sub_path[end] = '\0';
        mkdir((char*)sub_path, S_IRWXU);
        next_token = strchr(next_token,'/');
      }

      FILE* wfp = fopen(final_file_name, "wb");
      if( wfp ) {
        fwrite(data, file_descriptor->uncompressed_size, 1, wfp);
        fclose(wfp);
      } else {
        fprintf(stderr, "ERROR creating file %s\n", final_file_name );
      }

      free((char*)file_name);
      free(data);
      if( (i+1) % 1000  == 0 ) 
        printf("Extracted %d out of %d files\n",i+1, num_files);
    }
    fclose(fp);
  }

  return 0;
}

MYPFileStats MYPFile::get_stats() const {

  MYPFileStats stats;
  stats.num_files_ = file_descriptors_.size();
  return stats;

}
