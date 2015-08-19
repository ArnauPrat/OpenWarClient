

#include "OWClient.h"
#include "io/Log.h"
#include "login_sequence.h"
#include "network/NetworkSocket.h"
#include <irrlicht.h>
#include <string.h>
#include <thread>

namespace owc {

  OWClient::Status OWClient::client_status_ = E_DISCONNECTED;
  std::string OWClient::user_name_;
  std::string OWClient::token_;

  void OWClient::init() {
    Log::open("./log.txt");
  }

  void OWClient::free() {
    Log::close();
  }

  void OWClient::run( const Properties* properties) {

    std::thread t([ properties ] () mutable {
        char* token;
        if( login( properties->get("user_name").c_str(), 
            properties->get("passwd").c_str(), 
            properties->get("server_ip").c_str(), 
            atoi(properties->get("server_port").c_str()), &token ) ) {
          Log::write("ERROR", "Error logging in");
          return;
        }
        token_ = token;
        ::free(token);
        Log::write("INFO",token_.c_str());
        client_status_ = E_LOGGED_IN;
        });
    t.join();

  }
  

}
