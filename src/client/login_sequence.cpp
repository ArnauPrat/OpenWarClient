

#include "OWClient.h"
#include "core/marshalling.h"
//#include "crypt/sha256.h"
#include "error_codes.h"
#include "login_sequence.h"
#include "network/NetworkSocket.h"
#include <stdio.h>
#include <stdlib.h>

namespace owc {

  int login( const char* user_name, const char* passwd, const char* server_ip, unsigned int port, char** token ) {
/*
    //Log::write("INFO", "Starting login to server");
    TCPSocket login_client;
    if( int ret = login_client.connect(server_ip, port)) {
      return ret;
    }

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

    //Log::write("INFO","Creating SHA256 user/pass string");
    unsigned char sha256_input_buffer[512];
    unsigned char sha256_output_buffer[OWC_SHA256_BUFFER_SIZE];
    sprintf((char*)sha256_input_buffer,"%s:%s",user_name,passwd);
    sha256((char*)sha256_input_buffer, (char*)sha256_output_buffer );

    memcpy( (void*)(buffer+current_position),
        (void*)(sha256_output_buffer), 
        sizeof(unsigned char)*(OWC_SHA256_BUFFER_SIZE));

    current_position+=OWC_SHA256_BUFFER_SIZE;

    write_be(buffer, 0, current_position - 4);

    //Log::write("INFO","Sending user and password");
    size_t num_bytes;
    if( int ret = login_client.write(buffer,current_position, &num_bytes) ) {
      return ret;
    }

    if( int ret = login_client.read(buffer, 1024, &num_bytes) ) {
      return ret;
    }
    //Log::write("INFO","Login server responded");

    unsigned int packet_length = 0;
    current_position = 0;
    read_be(buffer, 0,  &packet_length);
    current_position+= sizeof(unsigned int);

    unsigned char op_code = 0x00;
    read(buffer, current_position, &op_code);
    current_position += sizeof(unsigned char);
    unsigned char val = 0;
    read(buffer, current_position, &val);
    current_position += sizeof(unsigned char);
    if( val == 0 ) {
      //Log::write("INFO","Correct user and password");
      unsigned int token_length = 0;
      read_be(buffer, current_position,  &token_length);
      current_position+=sizeof(unsigned int);
      *token = (char*)malloc(sizeof(char)*(token_length+1));
      read(buffer, current_position,  *token, token_length);
      (*token)[token_length] = '\0';
    } else {
      *token = NULL;
      return OWC_ERROR_WRONG_USER_PASS;
    }
    */
    return 0;
  }

}
