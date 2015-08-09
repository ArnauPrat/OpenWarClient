

#ifndef OWCLIENT_H
#define OWCLIENT_H

#include "common/macros.h"
#include "io/Properties.h"
#include <string>

namespace owc {

  class OWClient {
    OWC_NON_INSTANTIABLE(OWClient);
    OWC_NON_COPYABLE(OWClient);

    public:
    enum Status {
      E_DISCONNECTED,
      E_LOGGED_IN,
    };

    public:

    static void init();
    static void free();
    static void run( const Properties* properties);

    private:


    static std::string   user_name_;
    static std::string   token_;

    static Status  client_status_;

  };
}

#endif
