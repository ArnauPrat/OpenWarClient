

#include "marshalling.h"
#include "string.h"


namespace owc {

  void write(unsigned char* buffer, unsigned int offset, const char* str) {
    strcpy((char*)&buffer[offset],str); 
  }

  void write(unsigned char* buffer, unsigned int offset, unsigned char value) {
    buffer[offset] = value;
  }

  void write_le(unsigned char* buffer, unsigned int offset,  unsigned short value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned short) );
  }

  void write_le(unsigned char* buffer, unsigned int offset, unsigned int value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned int) );
  }

  void write_le(unsigned char* buffer, unsigned int offset, unsigned long long value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned long long) );
  }

  void write_le(unsigned char* buffer, unsigned int offset, short value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned short) );
  }

  void write_le(unsigned char* buffer, unsigned int offset, int value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned int) );
  }

  void write_le(unsigned char* buffer, unsigned int offset, long long value) {
    memcpy( &buffer[offset], &value, sizeof(unsigned long long) );
  }


  void write_be(unsigned char* buffer, unsigned int offset, unsigned short value) {
    buffer[offset] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)(value & 0x00ff); 
  }

  void write_be(unsigned char* buffer, unsigned int offset, unsigned int value) {

    buffer[offset] = (unsigned char)((value >> 24) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)((value >> 16) & 0x00ff); 
    buffer[offset + 2] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 3] = (unsigned char)(value & 0x00ff); 

  }

  void write_be(unsigned char* buffer, unsigned int offset, unsigned long long value) {

    buffer[offset] = (unsigned char)((value >> 56) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)((value >> 48) & 0x00ff); 
    buffer[offset + 2] = (unsigned char)((value >> 40) & 0x00ff); 
    buffer[offset + 3] = (unsigned char)((value >> 32) & 0x00ff); 
    buffer[offset + 4] = (unsigned char)((value >> 24) & 0x00ff); 
    buffer[offset + 5] = (unsigned char)((value >> 16) & 0x00ff); 
    buffer[offset + 6] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 7] = (unsigned char)(value & 0x00ff); 
  }

  void write_be(unsigned char* buffer, unsigned int offset, short value) {
    buffer[offset] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)(value & 0x00ff); 
  }

  void write_be(unsigned char* buffer, unsigned int offset, int value) {

    buffer[offset] = (unsigned char)((value >> 24) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)((value >> 16) & 0x00ff); 
    buffer[offset + 2] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 3] = (unsigned char)(value & 0x00ff); 

  }

  void write_be(unsigned char* buffer, unsigned int offset, long long value) {

    buffer[offset] = (unsigned char)((value >> 56) & 0x00ff); 
    buffer[offset + 1] = (unsigned char)((value >> 48) & 0x00ff); 
    buffer[offset + 2] = (unsigned char)((value >> 40) & 0x00ff); 
    buffer[offset + 3] = (unsigned char)((value >> 32) & 0x00ff); 
    buffer[offset + 4] = (unsigned char)((value >> 24) & 0x00ff); 
    buffer[offset + 5] = (unsigned char)((value >> 16) & 0x00ff); 
    buffer[offset + 6] = (unsigned char)((value >> 8) & 0x00ff); 
    buffer[offset + 7] = (unsigned char)(value & 0x00ff); 

  }

  void read(unsigned char* buffer, unsigned int offset, const char* str, unsigned int length) {
    strncpy((char*)str, (const char*)&buffer[offset], length);

  }
  void read(unsigned char* buffer, unsigned int offset, unsigned char* value) {
    (*value) = buffer[offset];
  }

  void read_le(unsigned char* buffer, unsigned int offset, unsigned short* value) {
    memcpy(value, &buffer[offset], sizeof(unsigned short));
  }

  void read_le(unsigned char* buffer, unsigned int offset, unsigned int* value) {
    memcpy(value, &buffer[offset], sizeof(unsigned int));
  }

  void read_le(unsigned char* buffer, unsigned int offset, unsigned long long* value) {
    memcpy(value, &buffer[offset], sizeof(unsigned long long ));
  }

  void read_le(unsigned char* buffer, unsigned int offset, short* value) {
    memcpy(value, &buffer[offset], sizeof(short ));
  }

  void read_le(unsigned char* buffer, unsigned int offset, int* value) {
    memcpy(value, &buffer[offset], sizeof(int));
  }

  void read_le(unsigned char* buffer, unsigned int offset, long long* value) {
    memcpy(value, &buffer[offset], sizeof(long long));
  }

  void read_be(unsigned char* buffer, unsigned int offset, unsigned short* value) {
    (*value) = (((unsigned short)buffer[offset]) << 8) + (unsigned short)(buffer[offset+1]) ; 
  }

  void read_be(unsigned char* buffer, unsigned int offset, unsigned int* value) {
    (*value) = (((unsigned int)buffer[offset]) << 24) + 
               (((unsigned int)buffer[offset+1]) << 16) + 
               (((unsigned int)buffer[offset+2]) << 8) + 
                 (unsigned int)(buffer[offset+3]); 
  }

  void read_be(unsigned char* buffer, unsigned int offset, unsigned long long* value) {

    (*value) = (((unsigned long long)buffer[offset]) << 56) + 
               (((unsigned long long)buffer[offset+1]) << 48) + 
               (((unsigned long long)buffer[offset+2]) << 40) + 
               (((unsigned long long)buffer[offset+3]) << 32) + 
               (((unsigned long long)buffer[offset+4]) << 24) + 
               (((unsigned long long)buffer[offset+5]) << 16) + 
               (((unsigned long long)buffer[offset+6]) << 8) + 
                 (unsigned long long)(buffer[offset+7]); 

  }

  void read_be(unsigned char* buffer, unsigned int offset, short* value) {
    (*value) = (((unsigned int)buffer[offset]) << 24) + 
               (((unsigned int)buffer[offset+1]) << 16) + 
               (((unsigned int)buffer[offset+2]) << 8) + 
                 (unsigned int)(buffer[offset+3]); 
  }

  void read_be(unsigned char* buffer, unsigned int offset, int* value) {

    (*value) = (((unsigned int)buffer[offset]) << 24) + 
               (((unsigned int)buffer[offset+1]) << 16) + 
               (((unsigned int)buffer[offset+2]) << 8) + 
                 (unsigned int)(buffer[offset+3]); 
  }
  void read_be(unsigned char* buffer, unsigned int offset, long long* value) {

    (*value) = (((unsigned long long)buffer[offset]) << 56) + 
               (((unsigned long long)buffer[offset+1]) << 48) + 
               (((unsigned long long)buffer[offset+2]) << 40) + 
               (((unsigned long long)buffer[offset+3]) << 32) + 
               (((unsigned long long)buffer[offset+4]) << 24) + 
               (((unsigned long long)buffer[offset+5]) << 16) + 
               (((unsigned long long)buffer[offset+6]) << 8) + 
                 (unsigned long long)(buffer[offset+7]); 
  }
}
