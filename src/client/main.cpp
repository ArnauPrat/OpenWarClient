
#include "io/Properties.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "OWClient.h"

#define CHECK_ARGUMENT_STRING(index, option, variable) \
	if( strcmp(argv[index],option) == 0 ){ \
			if( (index+1) < argc ) { \
				variable = argv[index+1]; \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}

#define CHECK_ARGUMENT_FLOAT(index, option,variable,setVariable) \
	if( strcmp(argv[index],option) == 0 ){ \
			setVariable = true; \
			if( (index+1) < argc ) { \
				variable = atof(argv[index+1]); \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}

#define CHECK_ARGUMENT_INT(index, option,variable,setVariable) \
	if( strcmp(argv[index],option) == 0 ){ \
			setVariable = true; \
			if( (index+1) < argc ) { \
				variable = atoi(argv[index+1]); \
			} else { \
				printf( "Invalid options.\n" ); \
				return 1;\
			}\
		}

#define CHECK_FLAG(index, option,setVariable) \
	if( strcmp(argv[index],option) == 0 ){ \
			setVariable = true; \
		}


void print_usage() {
  printf("OpenWarClient usage:\n");
  printf("%10s -f config_file_name \n","");
}


int main( int argc, char** argv){

  const char* config_file_name = NULL;


  for ( int i = 1; i < argc; i++) {
    CHECK_ARGUMENT_STRING(i,"-f", config_file_name );
  }

  if( !config_file_name ) {
    print_usage();
  }


  if(owc::OWClient::init( config_file_name )) {
    printf("ERROR when initializing engine");
  }
  owc::OWClient::run();
  owc::OWClient::free();
  return 0;
}
