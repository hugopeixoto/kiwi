
#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <stdlib.h>

struct http_request;
struct http_response {
  
  static http_response* create (int a_file_descriptor, const http_request* a_request, int a_status);
  void start_body ();
  void finish_body ();
  void body_chunk (const char* a_buffer, size_t a_size);

  protected:
  int fd_;
  const http_request* request_;
};

#endif // HTTP_RESPONSE_H_

