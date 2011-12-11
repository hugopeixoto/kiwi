
#ifndef KIWI_HTTP_RESPONSE_H_
#define KIWI_HTTP_RESPONSE_H_

#include <stdlib.h>

namespace kiwi {
  namespace http {
    class Response {

      public:
      void set_socket (int a_file_descriptor);

      inline int socket () const;

      void start_body ();
      void finish_body ();
      void body_chunk (const char* a_buffer, size_t a_size);
    
      protected:
      int fd_;
    };

    inline int Response::socket () const
    {
      return fd_; 
    }

    inline void Response::set_socket (int a_file_descriptor)
    {
      fd_ = a_file_descriptor;
    }
  }
}

#endif // KIWI_HTTP_RESPONSE_H_

