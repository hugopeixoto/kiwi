
#ifndef KIWI_HTTP_RESPONSE_H_
#define KIWI_HTTP_RESPONSE_H_

#include <stdlib.h>
#include <sstream>

namespace kiwi {
  namespace http {
    class Response : private std::streambuf, public std::ostream {

      public:
      Response ();
      ~Response ();

      void set_socket (int a_file_descriptor);

      inline int socket () const;

      void start_body ();
      void finish_body ();
      int body_chunk (const char* a_buffer, size_t a_size);

      protected:
      // std::streambuf implementation
      std::streambuf::int_type overflow (std::streambuf::int_type c);
      std::streambuf::int_type sync ();
      char buffer_[4096];

      int fd_;
    };

    inline Response::Response () : std::ostream(static_cast<std::streambuf*>(this)), std::ios(0)
    {
    }

    inline Response::~Response ()
    {
      sync();
    }

    inline int Response::socket () const
    {
      return fd_; 
    }

    inline void Response::set_socket (int a_file_descriptor)
    {
      fd_ = a_file_descriptor;
    }

    inline std::streambuf::int_type Response::overflow (std::streambuf::int_type ch)
    {
      if (pbase() == NULL) {
        setp(buffer_, buffer_ + 4095);
        if (ch != std::streambuf::traits_type::eof()) {
          ch = sputc(ch);
        } else {
          ch = 0;
        }
      } else {
        char* end = pptr();
        if (ch != std::streambuf::traits_type::eof()) {
          *end++ = ch;
        }

        if (body_chunk(pbase(), end - pbase()) == -1) {
          ch = std::streambuf::traits_type::eof();
        } else {
          ch = 0;
        }

        setp(buffer_, buffer_ + 4095);
      }
      return ch;
    }

    inline std::streambuf::int_type Response::sync ()
    {
      return (pptr() == pbase() || body_chunk(pbase(), pptr() - pbase()) != -1) ? 0 : -1;
    }
  }
}

#endif // KIWI_HTTP_RESPONSE_H_

