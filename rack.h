
#ifndef RACK_H_
#define RACK_H_

#include <stdint.h>

class http_request;
class rack {
  public:
  rack (uint16_t a_port);

  const http_request* next ();

  protected:
  int server_socket_;
};

#endif // RACK_H_

