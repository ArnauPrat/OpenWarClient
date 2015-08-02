
#include "names.h"
#include <string>
#include <stdlib.h>


static unsigned long long extract_hash( const char* str) {
  return strtol(str,NULL,16);
}


static std::string search_nif(std::string & file_name) {
  unsigned long long hash = extract_hash(file_name.c_str());

  return std::string();
}



std::string search(std::string & file_name) {
  
  unsigned int last_point_position = file_name.find_last_of(".");
  std::string extension = file_name.substr( last_point_position+1, std::string::npos); 

  if( extension == ".nif" ) {
    return search_nif(file_name);
  }
}

