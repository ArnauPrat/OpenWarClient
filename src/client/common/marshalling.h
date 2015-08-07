

#ifndef OWC_MARSHALLING_H
#define OWC_MARSHALLING_H


void write(unsigned char* buffer, unsigned int offset, const char*);

void write(unsigned char* buffer, unsigned int offset, unsigned char value);

void write_le(unsigned char* buffer, unsigned int offset, unsigned short value);
void write_le(unsigned char* buffer, unsigned int offset, unsigned int value);
void write_le(unsigned char* buffer, unsigned int offset, unsigned long long value);

void write_le(unsigned char* buffer, unsigned int offset, short value);
void write_le(unsigned char* buffer, unsigned int offset, int value);
void write_le(unsigned char* buffer, unsigned int offset, long long value);

void write_be(unsigned char* buffer, unsigned int offset, unsigned short value);
void write_be(unsigned char* buffer, unsigned int offset, unsigned int value);
void write_be(unsigned char* buffer, unsigned int offset, unsigned long long value);

void write_be(unsigned char* buffer, unsigned int offset, short value);
void write_be(unsigned char* buffer, unsigned int offset, int value);
void write_be(unsigned char* buffer, unsigned int offset, long long value);

#endif 
