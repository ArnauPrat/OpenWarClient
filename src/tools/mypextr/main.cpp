

#include <stdio.h>
#include "file_structs.h" 


int main(int argc, const char** argv) {
    printf("Welcome to OpenWarClient MYP file extractor ...\n");

    if( argc > 1 ) {
        printf("Extracting files of archive %s\n", argv[1]);
        ArchiveFileTable* file_table = load_archive(argv[1]);
        if( file_table != NULL)
            delete_ArchiveFileTable(file_table);
    }
    return 0;
}

