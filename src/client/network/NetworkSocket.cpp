

#ifdef LINUX
/*#include "NetworkSocket.h"
#include "network_error_codes.h"
#include "string.h"


namespace owc {

  template <typename Protocol>
  NetworkSocket<Protocol>::NetworkSocket() : 
    port_(0),
    socket_(io_service_) {
    memset(ip_, 0, sizeof(char)*OWC_IP_STRING_LENGTH );
  }

  template <typename Protocol>
  NetworkSocket<Protocol>::~NetworkSocket() {
    socket_.close();
  }

  template <typename Protocol>
  int NetworkSocket<Protocol>::connect( const char* ip, unsigned short port ) {
    strcpy(ip_, ip);
    endpoint_.address(asio::ip::address_v4().from_string(ip_));
    endpoint_.port(port);
    try {
      socket_.connect(endpoint_);
      return 0;
    } catch (...) {
      return OWC_NETWORK_ERROR_CONNECTION_REFUSED;
    }
  }

  template <typename Protocol>
  int NetworkSocket<Protocol>::write( unsigned char* buffer, size_t length, size_t* bytes_written ) {
    try {
      *bytes_written = socket_.write_some(asio::buffer(buffer, length));
      return 0;
    } catch(...) {
      *bytes_written = 0;
      return OWC_NETWORK_ERROR_WRITE_ERROR;
    }
  }

  template <typename Protocol>
  int NetworkSocket<Protocol>::read( unsigned char* buffer, size_t length, size_t* bytes_read ) {
    try {
      *bytes_read = socket_.read_some(asio::buffer(buffer, length));
      return 0;
    } catch (...) {
      *bytes_read = 0;
      return OWC_NETWORK_ERROR_READ_ERROR;
    }
  }


  template class NetworkSocket<tcp>;
  template class NetworkSocket<udp>;

}

*/
#endif
