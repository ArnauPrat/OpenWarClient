

#include <stdio.h>
#include <string.h>
#include "MYPFile.h" 

#define CHECK_ARGUMENT_STRING(index, option, variable) \
	if( strcmp(argv[index],option) == 0 ){ \
			if( (index+1) < argc ) { \
				variable = argv[index+1]; \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}

int main(int argc, const char** argv) {
    printf("Welcome to OpenWarClient MYP file extractor ...\n");

    const char* archive_file_name = NULL;
    const char* hash_file_name = NULL;
    for (int i = 1; i < argc; i++) {
        CHECK_ARGUMENT_STRING(i, "-a", archive_file_name)
        CHECK_ARGUMENT_STRING(i, "-h", hash_file_name)
    }

    if( archive_file_name ) {
        printf("Extracting files of archive %s\n", archive_file_name);
        MYPFile myp_file;
        if(myp_file.Load(archive_file_name)) {
          printf("Error when loading file\n");
        }

        if( hash_file_name ) {
          if( myp_file.LoadHashDictionary(hash_file_name) ) {
            printf("Error when reading hashes file\n");
          }
        }

        if( myp_file.Extract("./extracted_data") ) {
          printf("Error while extracting archive\n");
        }
    }
    return 0;
}

