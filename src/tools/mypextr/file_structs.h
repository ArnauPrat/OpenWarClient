

#ifndef FILE_STRUCTS_H_
#define FILE_STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


#define MYP_FILE_DESCRIPTOR_SIZE 34
#define MYP_TABLE_BLOCK_HEADER_SIZE 12
#define MYP_ARCHIVE_HEADER_SIZE 16
#define MYP_ARCHIVE_HEADER_START_POSITION 0x0C 


/*
 * Struct definitions
 */

struct TableBlockHeader {
    unsigned int num_files;
    unsigned long long next_block;
};

struct FileDescriptor {
    unsigned long long table_entry_position;
    unsigned long long starting_position;
    unsigned int header_size;
    unsigned int compressed_size;
    unsigned int uncompressed_size;
    unsigned int primary_hash;
    unsigned int secondary_hash;
    char compression_method;
};

struct ArchiveFileTable {
    std::vector<FileDescriptor> file_descriptors;
};


/*
 * Creator and Destruction functions
 */

FileDescriptor* create_FileDescriptor() {
    FileDescriptor* file_descriptor = (FileDescriptor*) malloc(sizeof(FileDescriptor));
    memset(file_descriptor,0, sizeof(FileDescriptor));
    return file_descriptor;
}

void delete_FileDescriptor( FileDescriptor* file_descriptor) {
    free(file_descriptor);
}


ArchiveFileTable* create_ArchiveFileTable() {
    return new ArchiveFileTable();
}

void delete_ArchiveFileTable(ArchiveFileTable* file_table) {
    delete file_table;
}


/*
 * Function definitions
 */

unsigned int LE_to_uint( const unsigned char* buffer, long offset ) {
    unsigned int byte1 = (buffer[offset]);
    unsigned int byte2 = (buffer[offset+1]) << 8;
    unsigned int byte3 = (buffer[offset+2]) << 16;
    unsigned int byte4 = (buffer[offset+3]) << 24;
    unsigned int result = byte4 + byte3 + byte2 + byte1;  
    return result;
}

unsigned long long LE_to_ulong( const unsigned char* buffer, long offset ) {
    unsigned long long lower = (unsigned long long)LE_to_uint(buffer,offset);
    unsigned long long upper = ((unsigned long long)(LE_to_uint(buffer,offset+4)) << 32);
    return upper + lower;
}


void initialize_FileDescriptor(FileDescriptor* descriptor, const unsigned char* buffer) {

    descriptor->starting_position = LE_to_ulong(buffer, 0);            
    descriptor->header_size =  LE_to_uint(buffer,8);
    descriptor->compressed_size = LE_to_uint(buffer, 12);
    descriptor->uncompressed_size =  LE_to_uint(buffer, 16);
    descriptor->secondary_hash = LE_to_uint(buffer, 20);
    descriptor->primary_hash = LE_to_uint(buffer, 24);
    descriptor->compression_method = buffer[32];

}

void initialize_TableBlockHeader( TableBlockHeader* block_header, const unsigned char* buffer ) {
    block_header->num_files  = LE_to_uint(buffer,0);
    block_header->next_block = LE_to_ulong(buffer,4);
}

ArchiveFileTable* load_archive( const char* file_name) {
    ArchiveFileTable* archive_file_table = create_ArchiveFileTable();
    FILE* fp = fopen(file_name, "rb");

    unsigned char file_descriptor_buffer[MYP_FILE_DESCRIPTOR_SIZE];
    unsigned char archive_header_buffer[MYP_ARCHIVE_HEADER_SIZE];
    unsigned char table_block_header_buffer[MYP_TABLE_BLOCK_HEADER_SIZE];


    /* 
     * Obtain the position of the first table block  
     */
    if(fseek(fp,MYP_ARCHIVE_HEADER_START_POSITION,SEEK_SET)) {
        fprintf(stderr, "Error when moving file pointer to archive header\n");
        return NULL;
    }

    fread(archive_header_buffer, MYP_ARCHIVE_HEADER_SIZE, 1, fp);
    if( feof(fp) ) {
        fprintf(stderr, "Unexpectedly reached end of file at looking for the archive header\n");
        return NULL;
    }
    unsigned long long next_table_block_header = LE_to_ulong(archive_header_buffer,0);

    /* 
     * Get total number of files in the archive  
     */
    if(fseek(fp,24,SEEK_SET)) {
        fprintf(stderr, "Error when moving file pointer to archive header\n");
        return NULL;
    }

    unsigned char buffer[4];
    fread(buffer,4,1,fp);
    if( feof(fp) ) {
        fprintf(stderr, "Unexpectedly reached end of file when looking for the total number of files in the archive\n");
        return NULL;
    }
    unsigned int num_total_files = LE_to_uint(buffer,0);
    printf("Number of files in the archive %d\n",num_total_files);

    unsigned int num_parsed_files = 0;
    while( next_table_block_header != 0 ) {

        /* We read the first table block header */
        if(fseek(fp,next_table_block_header,SEEK_SET)) {
            fprintf(stderr, "Error placing the cursor to the header block position\n");
            return NULL;
        }
        int bytes_read = fread(table_block_header_buffer, MYP_TABLE_BLOCK_HEADER_SIZE, 1, fp);
        if( feof(fp) && bytes_read != MYP_TABLE_BLOCK_HEADER_SIZE ) {
            fprintf(stderr, "Unexpectedly reached end of file when reading table block header\n");
            return NULL;
        }

        TableBlockHeader table_block_header;
        initialize_TableBlockHeader(&table_block_header, table_block_header_buffer);


        /*Read file descriptors*/

        next_table_block_header = table_block_header.next_block;
        num_parsed_files += table_block_header.num_files;
        printf("Number of files read so far: %d\n", num_parsed_files);
    }

    printf("Number of files read  %d\n", num_total_files);
    fclose(fp);
    return archive_file_table;
}


#endif
