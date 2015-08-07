

#ifndef OWCLIENT_H
#define OWCLIENT_H

#include "common/macros.h"
#include <string>
#include "asio.hpp"

class OWClient {
  OWC_NON_INSTANTIABLE(OWClient);
  OWC_NON_COPYABLE(OWClient);

  public:

    static OWClient* instance(); 
    static void destroy_instance();


    void login( const char* user_name, const char* passwd, const char* server_ip, unsigned int port );



  private:

    static OWClient* instance_;

    std::string   user_name_;
    std::string   token_;

    asio::io_service          io_service_;
    asio::ip::tcp::socket     login_socket_;
    asio::ip::tcp::endpoint   login_endpoint_; 

};

#endif
