
#ifndef OWC_LOGIN_SEQUENCE_H
#define OWC_LOGIN_SEQUENCE_H

namespace owc {

  int login( const char* user_name, const char* passwd, const char* server_ip, unsigned int port, char** token  );

}

#endif
