

#ifndef FILE_STRUCTS_H_
#define FILE_STRUCTS_H_


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

unsigned int LE_to_int( char* buffer, long offset ) {
    unsigned int result = (buffer[offset+3] << 24) + (buffer[offset+2] << 16) + (buffer[offset+1] << 8) + (buffer[offset+1]); 
}


void initialize_file_descriptor(FileDescriptor* descriptor, char* buffer) {

    descriptor->starting_position = LE_to_int(buffer, 0);            
    descriptor->starting_position += (LE_to_int(buffer,4) << 32);   
    descriptor->header_size =  LE_to_int(buffer,8);
    descriptor->compressed_size = LE_to_int(buffer, 12);
    descriptor->uncompressed_size =  LE_to_int(buffer, 16);
    descriptor->secondary_hash = LE_to_Int(buffer, 20);
    descriptor->primary_hash = LE_to_Int(buffer, 24);
    descriptor->compression_method = buffer[32];

}


#endif
