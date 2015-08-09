

#ifndef OWC_LOG_H
#define OWC_LOG_H

#include "../common/macros.h"
#include "stdio.h"

namespace owc {

  class Log {
    OWC_NON_INSTANTIABLE(Log);
    OWC_NON_COPYABLE(Log);
    
    public:
    static void open( const char* log_file_name);
    static void close();
    static void write( const char* prefix, const char* message );

    private:
    static FILE* fp_;

  };
}

#endif
