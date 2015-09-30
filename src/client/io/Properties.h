
#ifndef OWC_PROPERTIES_H
#define OWC_PROPERTIES_H

#include <irrTypes.h>
#include <irrMacros.h>
#include <map>
#include <string>


using namespace irr;

namespace owc {


class Properties {
  IRR_NON_COPYABLE(Properties);

  public:
    Properties();
    ~Properties();

    u32 load(const c8* file_name);

    std::string get(const c8* property_name) const; 
    void set(const c8* property_name, const c8* value); 

    void print( FILE* fp );

  private:

    std::map<std::string, std::string>  properties_;

};

}


#endif 
