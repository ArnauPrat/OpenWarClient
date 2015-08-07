

#include "OWClient.h"
#include "common/marshalling.h"
#include "crypt/sha256.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



OWClient* OWClient::instance_ = NULL;

OWClient::OWClient() : login_socket_(io_service_) {
}

OWClient* OWClient::instance() {
  if (!instance_) {
    instance_ = new OWClient();
  }
  return instance_;
}

void OWClient::destroy_instance(){
  if(instance_) {
    delete instance_;
    instance_ = NULL;
  }
}

void OWClient::login( const char* user_name, const char* passwd, const char* server_ip, unsigned int port ) {

  login_endpoint_.address(asio::ip::address_v4().from_string(server_ip));
  login_endpoint_.port(port);
  login_socket_.connect(login_endpoint_);

  unsigned char buffer[1024];
  unsigned int current_position = 4;
  write(buffer, current_position, (unsigned char)(0x03));
  current_position += sizeof(unsigned char);
  int user_name_length = strlen(user_name);
  write_be(buffer, current_position, user_name_length);
  current_position += sizeof(unsigned int);
  write(buffer, current_position, user_name);
  current_position += sizeof(unsigned char)*user_name_length;

  write_be(buffer,current_position,OWC_SHA256_BUFFER_SIZE);
  current_position+=4;
  unsigned char sha256_input_buffer[512];
  unsigned char sha256_output_buffer[OWC_SHA256_BUFFER_SIZE];
  sprintf((char*)sha256_input_buffer,"%s:%s",user_name,passwd);
  sha256((char*)sha256_input_buffer, (char*)sha256_output_buffer );

  memcpy((void*)(buffer+current_position),
         (void*)(sha256_output_buffer), 
         sizeof(unsigned char)*(OWC_SHA256_BUFFER_SIZE));

  current_position+=OWC_SHA256_BUFFER_SIZE;
  write_be(buffer, 0, current_position - 4);
  login_socket_.write_some(asio::buffer(buffer,current_position));

}
