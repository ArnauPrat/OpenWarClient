


#include "Log.h"

namespace owc {

  FILE* Log::fp_ = NULL;

  void Log::open( const char* log_file_name ) {
    fp_ = fopen(log_file_name, "w");
  }

  void Log::close() {
    fclose(fp_);
  }


  void Log::write( const char* prefix, const char* message ) {
    fprintf(stdout,"%s: %s\n", prefix, message);
    fprintf(fp_,"%s: %s\n", prefix, message);
  }

}
