

#include <io/hash.h>
#include "stdio.h"

int main(int argc, char** argv) {
  if( argc > 0 ) {

    //unsigned long long result = owc::hash(argv[1], 0xDEADBEEF);
    unsigned int ph = 0;
    unsigned int sh = 0;
    owc::hashlittle2((const void*)argv[1], (unsigned long)strlen(argv[1]), &ph, &sh); 
    unsigned long long result  = ((unsigned long long)sh << 32) + ph;
    printf("%llX\n",result);

  }
  return 0;
}

