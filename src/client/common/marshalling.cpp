

#include "marshalling.h"
#include "string.h"



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
