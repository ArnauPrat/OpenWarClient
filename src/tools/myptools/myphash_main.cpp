

#include <io/hash.h>
#include "stdio.h"

int main(int argc, char** argv) {
  if( argc > 0 ) {

    //unsigned long long result = owc::hash(argv[1], 0xDEADBEEF);
    unsigned int ph = 0;
    unsigned int sh = 0;
    hashlittle2(argv[1], strlen(argv[1]), &sh, &ph); 
    unsigned long long result  = ((unsigned long long)ph << 32) + sh;
    printf("%llX\n",result);

  }
  return 0;
}

