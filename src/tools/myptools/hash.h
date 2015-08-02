
#ifndef MYP_HASH_H
#define MYP_HASH_H

#include <string.h>

unsigned long long hash(const char* str, unsigned int seed) {


  unsigned int edx = 0, eax, esi, ebx = 0;
  unsigned int edi, ecx;
  unsigned long ph, sh;

  int string_length = strlen(str);
  eax = ecx = edx = ebx = esi = edi = 0;
  ebx = edi = esi = strlen(str) + seed;

  int i = 0;
  for ( ; i + 12 < string_length; i += 12)
  {
    unsigned int aux;
    /* edi = (unsigned int)((s[i + 7] << 24) | (s[i + 6] << 16) | (s[i + 5] << 8) | s[i + 4]) + edi; */
    memcpy(&aux, &str[i+4], sizeof(char)*4);
    edi += aux;
    /* esi = (unsigned int)((s[i + 11] << 24) | (s[i + 10] << 16) | (s[i + 9] << 8) | s[i + 8]) + esi; */
    memcpy(&aux, &str[i+8], sizeof(char)*4);
    esi += aux;
    /* edx = (unsigned int)((s[i + 3] << 24) | (s[i + 2] << 16) | (s[i + 1] << 8) | s[i]) - esi; */
    memcpy(&aux, &str[i], sizeof(char)*4);
    edx = aux - esi;


    edx = (edx + ebx) ^ (esi >> 28) ^ (esi << 4);
    esi += edi;
    edi = (edi - edx) ^ (edx >> 26) ^ (edx << 6);
    edx += esi;
    esi = (esi - edi) ^ (edi >> 24) ^ (edi << 8);
    edi += edx;
    ebx = (edx - esi) ^ (esi >> 16) ^ (esi << 16);
    esi += edi;
    edi = (edi - ebx) ^ (ebx >> 13) ^ (ebx << 19);
    ebx += esi;
    esi = (esi - edi) ^ (edi >> 28) ^ (edi << 4);
    edi += ebx;
  }

  if (string_length - i > 0)
  {
    switch (string_length - i)
    {
      case 12:
        esi += ((unsigned int)str[i + 11]) << 24;
      case 11:
        esi += ((unsigned int)str[i + 10]) << 16;
      case 10:
        esi += ((unsigned int)str[i + 9]) << 8;
      case 9:
        esi += ((unsigned int)str[i + 8]);
      case 8:
        edi += ((unsigned int)str[i + 7]) << 24;
      case 7:
        edi += ((unsigned int)str[i + 6]) << 16;
      case 6:
        edi += ((unsigned int)str[i + 5]) << 8;
      case 5:
        edi += ((unsigned int)str[i + 4]);
      case 4:
        ebx += ((unsigned int)str[i + 3]) << 24;
      case 3:
        ebx += ((unsigned int)str[i + 2]) << 16;
      case 2:
        ebx += ((unsigned int)str[i + 1]) << 8;
      case 1:
        ebx += (unsigned int)str[i];
    }

    esi = (esi ^ edi) - ((edi >> 18) ^ (edi << 14));
    ecx = (esi ^ ebx) - ((esi >> 21) ^ (esi << 11));
    edi = (edi ^ ecx) - ((ecx >> 7) ^ (ecx << 25));
    esi = (esi ^ edi) - ((edi >> 16) ^ (edi << 16));
    edx = (esi ^ ecx) - ((esi >> 28) ^ (esi << 4));
    edi = (edi ^ edx) - ((edx >> 18) ^ (edx << 14));
    eax = (esi ^ edi) - ((edi >> 8) ^ (edi << 24));

    ph = edi;
    sh = eax;
    return (ph << 32) + sh;
  }
  ph = esi;
  sh = eax;
  return (ph << 32) + sh;

}


#endif
