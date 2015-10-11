
#ifndef __CPROPERTIES_H_INCLUDED__
#define __CPROPERTIES_H_INCLUDED__

#include <irrTypes.h>
#include <irrMacros.h>
#include <map>
#include <irrString.h>



namespace irr {


  namespace io {
    class IReadFile;

    class CProperties {
      IRR_NON_COPYABLE(CProperties);

      public:
      CProperties();
      ~CProperties();

      u32 load( io::IReadFile* file );

      core::stringc get(const core::stringc& propertyName) const; 

      s32 getInt( const core::stringc& propertyName ) const;

      void set(const core::stringc& propertyName, const core::stringc& value); 

      void print( FILE* fp );

      private:

      std::map<core::stringc, core::stringc>  properties_;

    };
  }

}


#endif 
