

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io/MYPFileDatabase.h> 

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

void do_exists(owc::MYPFileDatabase* file_database) {
  char file_name[256];
  printf("Enter file name: \n");
  scanf("%256s",file_name);
  unsigned char* data;
  size_t data_size;
  if(file_database->get_file_data((const char*)file_name, &data, &data_size)) {
    printf("Unexisting file\n");
    return;
  }

  printf("Enter output folder: \n");
  char folder_name[256];
  scanf("%256s",folder_name);
  char final_name[512];
  if(file_database->extract(file_name, folder_name))
    printf("File extracted: %s\n", final_name);
  //free(data);
}

void do_export_files(const char* files_file_name, owc::MYPFileDatabase* file_database) {

  printf("Enter output folder: \n");
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
}

bool is_option(const char* option) {
  return option[0] == '-';
}

int main(int argc, const char** argv) {
    printf("Welcome to OpenWarClient MYP file extractor ...\n");

    owc::MYPFileDatabase myp_file;

    const char* files_file_name = NULL;
    for (int i = 1; i < argc; ) {
        if( (strcmp(argv[i],"-a") == 0) ) {
          i++;
          while( i < argc && !is_option(argv[i]) ) {
            printf("Loading archive %s \n", argv[i]);
            myp_file.load_archive(argv[i]);
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
          do_exists(&myp_file);
          break;
        case 'f':
          do_export_files(files_file_name,&myp_file);
          break;
      }
    } while(option != 'q');
    return 0;
}

