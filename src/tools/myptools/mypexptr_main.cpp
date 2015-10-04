

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CFileSystem.h>
#include <CMYPReader.h>
#include <IReadFile.h>
#include <IWriteFile.h>

using namespace irr;

#define CHECK_ARGUMENT_STRING(index, option, variable) \
	if( strcmp(argv[index],option) == 0 ){ \
			if( (index+1) < argc ) { \
				variable = argv[index+1]; \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}


void print_options( const char* file_name ) {
  printf("e - Export file\n");
  if(file_name != NULL){
    printf("f - Export files in %s\n", file_name);
  }
  printf("q - Quit\n");
}

void extract(io::CFileSystem* file_system, io::IReadFile* read_file, const io::path& real_file_name, const io::path& output_folder ) {
  io::path final_path;
  final_path.append(output_folder);
  final_path.append("\\").append(real_file_name);
  io::IWriteFile* write_file = file_system->createAndWriteFile(final_path);
  u8 buffer[1024];
  u32 byte_count = 0;
  while( (byte_count = read_file->read(buffer, 1024)) != 0 ) {
    write_file->write(buffer,byte_count);
  }
  write_file->drop();
}

void do_exists(io::CFileSystem* file_system ) {
  char file_name[256];
  printf("Enter file name: \n");
  scanf("%256s",file_name);
  size_t data_size;
  io::IReadFile* file = file_system->createAndOpenFile(io::path(file_name));
  if(file) {
    printf("Enter output folder: \n");
    char folder_name[256];
    scanf("%256s",folder_name);
    io::path folder_path(folder_name);
    extract(file_system, file, io::path(file_name), io::path(folder_path));
    file->drop();
  }
}

void do_export_files(const char* files_file_name, io::CFileSystem* file_system ) {

  /*printf("Enter output folder: \n");
  char folder_name[256];
  scanf("%256s",folder_name);

  FILE* fp = fopen(files_file_name, "r");

  int LINE_BUFFER_BLOCK_SIZE = 256;
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
    line_buffer[strlen(line_buffer)-1] = '\0';
    if(file_database->extract(line_buffer, folder_name))
      printf("File extracted: %s\n", line_buffer);
  }
  free(line_buffer);
  fclose(fp);
  */
}

bool is_option(const char* option) {
  return option[0] == '-';
}




int main(int argc, const char** argv) {
    printf("Welcome to OpenWarClient MYP file extractor ...\n");

    io::CFileSystem* file_system = new irr::io::CFileSystem();
    io::IArchiveLoader* myp_loader = new irr::io::CMYPArchiveLoader(file_system);
    file_system->addArchiveLoader(myp_loader);


    const char* files_file_name = NULL;
    for (int i = 1; i < argc; ) {
        if( (strcmp(argv[i],"-a") == 0) ) {
          i++;
          while( i < argc && !is_option(argv[i]) ) {
            printf("Loading archive %s \n", argv[i]);
            if(!file_system->addFileArchive( io::path(argv[i]), false, false, io::EFAT_MYP)) {
              printf("ERROR loading %s\n",argv[i]);
            }
            i++;
          }
        } else if( (strcmp(argv[i],"-f") == 0) ) {
          ++i;
          if( i < argc ) { 
            files_file_name = argv[i];
            ++i;
          }
        }
    }

    char option;
    do {
      print_options( files_file_name );
      scanf("\n%c",&option);
      switch(option) {
        case 'e':
          do_exists(file_system);
          break;
        case 'f':
          do_export_files(files_file_name, file_system);
          break;
      }
    } while(option != 'q');
    delete file_system;
    delete myp_loader;
    return 0;
}

