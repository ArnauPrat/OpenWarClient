
#ifndef OWC_NETWORK_CLIENT_H
#define OWC_NETWORK_CLIENT_H

#ifdef LINUX
/*#include "core/macros.h"
#include <asio.hpp>

#define OWC_IP_STRING_LENGTH 16

namespace owc {


  typedef asio::ip::tcp tcp;
  typedef asio::ip::udp udp;
  
  template<typename Protocol>
  class NetworkSocket {
    OWC_NON_COPYABLE(NetworkSocket);

    public:

    NetworkSocket();
    ~NetworkSocket();

    int connect( const char* ip, unsigned short port ); 
    int write( unsigned char* buffer, size_t length, size_t* bytes_written ); 
    int read( unsigned char* buffer, size_t length, size_t* bytes_read ); 


    protected:

    char                      ip_[OWC_IP_STRING_LENGTH];
    unsigned short            port_;

    asio::io_service                             io_service_;
    asio::basic_stream_socket<Protocol>          socket_;
    asio::ip::basic_endpoint<Protocol>           endpoint_; 
    
  };

  typedef NetworkSocket<tcp> TCPSocket;
  typedef NetworkSocket<udp> UDPSocket;

}
*/
#endif


#endif
