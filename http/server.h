
#ifndef KIWI_HTTP_SERVER_H_
#define KIWI_HTTP_SERVER_H_

#include <stdint.h>

namespace kiwi {
  namespace http {
    class Parser;
    class Request;
    class Response;

    class Server {
      public:
      /**
       * @brief Initializes the server on the given port.
       */
      Server (uint16_t a_port);

      virtual ~Server ();

      /**
       * @brief Blocks until there is a request to serve, or the server fails.
       */
      bool begin (Request*& a_request, Response*& a_response);

      /**
       * @brief Finishes the request/response workflow.
       * This flushes the output and frees all request associated resources.
       */
      bool end (Request*& a_request, Response*& a_response);

      protected:
      int server_socket_; //!< file descriptor of the listener socket.
      Parser* parser_;
    };
  }
}

#endif // KIWI_HTTP_SERVER_H_

