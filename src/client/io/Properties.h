
#ifndef OWC_PROPERTIES_H
#define OWC_PROPERTIES_H

#include "common/macros.h"
#include <map>
#include <string>


namespace owc {

class Properties {
  OWC_NON_COPYABLE(Properties);

  public:
    Properties();
    ~Properties();

    int load(const char* file_name);

    std::string get(const char* property_name) const; 
    void set(const char* property_name, const char* value); 

    void print( FILE* fp );

  private:

    std::map<std::string, std::string>  properties_;

};

}


#endif 
