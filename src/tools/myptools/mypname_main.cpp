

#include <stdio.h>
#include <string.h>
#include <string>
#include <set>
#include "MYPFile.h" 

#define CHECK_ARGUMENT_STRING(index, option, variable) \
	if( strcmp(argv[index],option) == 0 ){ \
			if( (index+1) < argc ) { \
				variable = argv[index+1]; \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}

int main(int argc, const char** argv) {

  std::set<std::string> file_list;
  for (int i = 1; i < argc; ++i) {
   file_list.insert(std::string(argv[i]));
  }

  return 0;
}

