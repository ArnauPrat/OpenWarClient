

#include "hash.h"
#include "stdio.h"

int main(int argc, char** argv) {
  if( argc > 0 ) {

    unsigned long long result = hash(argv[1], 0xDEADBEEF);
    printf("%lX\n",result);

  }
  return 0;
}

