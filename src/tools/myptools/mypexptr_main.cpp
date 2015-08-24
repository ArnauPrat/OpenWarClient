

#include <stdio.h>
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


void print_options() {
  printf("e - Export file\n");
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
}

int main(int argc, const char** argv) {
    printf("Welcome to OpenWarClient MYP file extractor ...\n");

    owc::MYPFileDatabase myp_file;

    const char* archive_file_name = NULL;
    const char* hash_file_name = NULL;
    for (int i = 1; i < argc; ++i) {
        if( (strcmp(argv[i],"-a") == 0) ) {
          i++;
          while( i < argc ) {
            printf("Loading archive %s \n", argv[i]);
            myp_file.load_archive(argv[i]);
            i++;
          }
        }
    }

    char option;
    do {
      print_options();
      scanf("\n%c",&option);
      switch(option) {
        case 'e':
          do_exists(&myp_file);
          break;
      }
    } while(option != 'q');
    return 0;
}

