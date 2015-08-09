
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
  printf("%10s -u user_name \n","");
  printf("%10s -p password \n","");
  printf("%10s -f config_file_name \n","");
}


int main( int argc, char** argv){

  const char* user_name = NULL;
  const char* passwd = NULL;
  const char* config_file_name = NULL;


  for ( int i = 1; i < argc; i++) {
    CHECK_ARGUMENT_STRING(i,"-u", user_name );
    CHECK_ARGUMENT_STRING(i,"-p", passwd );
    CHECK_ARGUMENT_STRING(i,"-f", config_file_name );
  }

  if( !(user_name && passwd && config_file_name) ) {
    print_usage();
  }

  Properties properties;
  if( properties.load(config_file_name) ) {
    fprintf(stderr, "Error when loading config file\n");
    return 0;
  }

  properties.print(stdin);

  properties.set("user_name", user_name);
  properties.set("passwd", passwd);
  owc::OWClient::init();
  owc::OWClient::run(&properties);
  owc::OWClient::free();
  return 0;
}
