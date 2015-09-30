

#include "Properties.h"
#include "io_error_codes.h"
#include <fstream>
#include <iostream>
#include <sstream>


namespace owc {
Properties::Properties() {

}

Properties::~Properties() {
}

u32 Properties::load(const char* file_name) {

  std::ifstream input_file;
  input_file.open(file_name);

  if (!input_file) 
    return OWC_IO_ERROR_FILE_NOT_FOUND;

  std::string line;
  std::string option;
  std::string value;
  while (std::getline(input_file, line)) {
    std::stringstream ss(line);
    ss >> option;
    if( !(ss >> value) ) {
      fprintf(stderr, "Error when reading input file");
      return OWC_IO_ERROR_WRONG_FORMAT;
    }
    properties_.insert(std::pair<std::string, std::string>(option, value));
  }

  input_file.close();
  return 0;
}

std::string Properties::get(const char* property_name) const {
  std::map<std::string, std::string>::const_iterator it = properties_.find(property_name);
  if( it != properties_.end() ) return it->second;
  return std::string();
}

void Properties::set(const char* property_name, const char* value) {
  properties_.insert(std::pair<std::string, std::string>(property_name, value));
}

void Properties::print( FILE* fp ) {
  for ( std::map<std::string, std::string>::iterator it = properties_.begin(); 
        it != properties_.end(); 
        ++it) {
    fprintf(stdout, "%s %s\n", it->first.c_str(), it->second.c_str());
  }
}

}
