

#ifndef OWC_SHA256_H
#define OWC_SHA256_H

#include <openssl/sha.h>

#define OWC_SHA256_BUFFER_SIZE SHA256_DIGEST_LENGTH

void sha256(const char *string, char output_buffer[OWC_SHA256_BUFFER_SIZE] ) {
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, string, strlen(string));
  SHA256_Final((unsigned char*)output_buffer, &sha256);
}

#endif
