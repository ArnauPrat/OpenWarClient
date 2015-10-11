

#include "CProperties.h"
#include "include/IReadFile.h"
#include "os.h"


namespace irr {
  namespace io {
    CProperties::CProperties() {

    }

    CProperties::~CProperties() {
    }

    u32 CProperties::load(IReadFile* file) {
      file->grab();
      core::stringc line;
      while (file->readLine(&line)) {
        if(line[0] != '[' && line[0] != ';') {
          os::Printer::log("Read Option: ");
          os::Printer::log(line.c_str());
          s32 separator = line.findFirst('=');
          core::stringc option = line.subString(0,separator); 
          core::stringc value = line.subString(separator+1, line.size()); 
          set(option, value);
        }
      }
      file->drop();

      return 0;
    }

    core::stringc CProperties::get( const core::stringc& propertyName) const {
      std::map<core::stringc, core::stringc>::const_iterator it = properties_.find(propertyName);
      if( it != properties_.end() ) return it->second;
      os::Printer::log("OPTION NOT FOUND", ELL_ERROR);
      return core::stringc();
    }

    s32 CProperties::getInt( const core::stringc& propertyName ) const {
      std::map<core::stringc, core::stringc>::const_iterator it = properties_.find(propertyName);
      if( it != properties_.end() ) return atoi(it->second.c_str());
      os::Printer::log("OPTION NOT FOUND", ELL_ERROR);
      return 0;
    }

    void CProperties::set(const core::stringc& property_name, const core::stringc& value) {
      properties_.insert(std::pair<core::stringc, core::stringc>(property_name, value));
    }

    void CProperties::print( FILE* fp ) {
      for ( std::map<core::stringc, core::stringc>::iterator it = properties_.begin(); 
          it != properties_.end(); 
          ++it) {
        fprintf(stdout, "%s %s\n", it->first.c_str(), it->second.c_str());
      }
    }

  }
}
