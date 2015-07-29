

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
#define MYP_EXPECTED_NUM_FILES_PER_BLOCK 1000


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
    unsigned char compression_method;
};

struct ArchiveFileTable {
    std::vector<FileDescriptor> file_descriptors;
};


/*
 * Creator and Destruction functions
 */

ArchiveFileTable* create_ArchiveFileTable() {
    return new ArchiveFileTable();
}

void delete_ArchiveFileTable(ArchiveFileTable* file_table) {
    delete file_table;
}


/*
 * Function definitions
 */

unsigned int parse_uint( const unsigned char* buffer, long offset ) {
    unsigned int result;
    memcpy(&result,buffer+offset,sizeof(unsigned int));
    return result;
}

unsigned long long parse_ulong( const unsigned char* buffer, long offset ) {
    unsigned long long result;
    memcpy(&result,buffer+offset,sizeof(unsigned long long));
    return result;
}


void initialize_FileDescriptor(FileDescriptor* descriptor, const unsigned char* buffer, unsigned int offset) {

    descriptor->starting_position = parse_ulong(buffer, offset + 0);            
    descriptor->header_size =  parse_uint(buffer, offset + 8);
    descriptor->compressed_size = parse_uint(buffer, offset + 12);
    descriptor->uncompressed_size =  parse_uint(buffer, offset + 16);
    descriptor->secondary_hash = parse_uint(buffer, offset + 20);
    descriptor->primary_hash = parse_uint(buffer, offset + 24);
    descriptor->compression_method = buffer[offset + 32];

}

void initialize_TableBlockHeader( TableBlockHeader* block_header, const unsigned char* buffer ) {
    block_header->num_files  = parse_uint(buffer,0);
    block_header->next_block = parse_ulong(buffer,4);
}

ArchiveFileTable* load_archive( const char* file_name) {
    ArchiveFileTable* archive_file_table = create_ArchiveFileTable();
    FILE* fp = fopen(file_name, "rb");

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
    unsigned long long next_table_block_header = parse_ulong(archive_header_buffer,0);

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
    unsigned int num_total_files = parse_uint(buffer,0);
    printf("Number of files in the archive %d\n",num_total_files);

    /* Create buffers to store file descriptors. Initial size to 1000 as it is typically observed in practice.  */
    unsigned int myp_file_descriptor_buffer_size = MYP_EXPECTED_NUM_FILES_PER_BLOCK * MYP_FILE_DESCRIPTOR_SIZE;
    unsigned char* myp_file_descriptor_buffer = (unsigned char*)malloc(sizeof(unsigned char)*myp_file_descriptor_buffer_size);
    unsigned int file_descriptor_buffer_size = MYP_EXPECTED_NUM_FILES_PER_BLOCK;
    FileDescriptor* file_descriptor_buffer = (FileDescriptor*)malloc(sizeof(FileDescriptor)*file_descriptor_buffer_size);
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
        if( table_block_header.num_files*MYP_FILE_DESCRIPTOR_SIZE > myp_file_descriptor_buffer_size ) { // resizing buffer if needed
            free(myp_file_descriptor_buffer);
            myp_file_descriptor_buffer_size = table_block_header.num_files*MYP_FILE_DESCRIPTOR_SIZE;
            myp_file_descriptor_buffer = (unsigned char*)malloc(myp_file_descriptor_buffer_size);
        }

        if( table_block_header.num_files > file_descriptor_buffer_size ) { // resizing buffer if needed
            free(file_descriptor_buffer);
            file_descriptor_buffer_size = table_block_header.num_files ;
            file_descriptor_buffer = (FileDescriptor*)malloc(sizeof(FileDescriptor)*file_descriptor_buffer_size);
        }

        if( fread(myp_file_descriptor_buffer, MYP_FILE_DESCRIPTOR_SIZE, table_block_header.num_files, fp) != table_block_header.num_files ) {
            fprintf(stderr, "Unexpectedly reached end of file when reading file headers\n");
            return NULL;
        }

        for ( int i = 0; i < table_block_header.num_files; ++i) {
            initialize_FileDescriptor(&file_descriptor_buffer[i],myp_file_descriptor_buffer, i*MYP_FILE_DESCRIPTOR_SIZE);
            if( file_descriptor_buffer[i].compression_method == 0 )
                printf("uncompressed size: %d, compressed_size: %d\n", file_descriptor_buffer[i].uncompressed_size, file_descriptor_buffer[i].compressed_size);
            num_parsed_files += file_descriptor_buffer[i].starting_position > 0 ? 1 : 0;
        }

        /*Read files*/
        for ( int i = 0; i < table_block_header.num_files; ++i) {

        }

        next_table_block_header = table_block_header.next_block;
        printf("Number of files read so far: %d\n", num_parsed_files);
    }
    free(myp_file_descriptor_buffer);
    free(file_descriptor_buffer);

    printf("Number of files read  %d\n", num_parsed_files);
    fclose(fp);
    return archive_file_table;
}


#endif
