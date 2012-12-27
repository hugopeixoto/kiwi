
#ifndef KIWI_HTTP_SERVER_H_
#define KIWI_HTTP_SERVER_H_

#include <stdint.h>

namespace kiwi {
  namespace http {
    class Request;
    class Response;

    class Server {
      public:
      /**
       * @brief Initializes the server on the given port.
       */
      Server ();

      virtual ~Server ();

      bool construct (uint16_t a_port);

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
      bool accept ();
      bool receive (uint32_t a_idx);
      bool close (uint32_t a_idx);

      protected:
      /**
       * @brief Opaque structure.
       */
      struct Implementation;

      Implementation* data; //!< Opaque pointer.
    };
  }
}

#endif // KIWI_HTTP_SERVER_H_

