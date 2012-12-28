
#ifndef KIWI_CONTROLLER_BASE_H_
#define KIWI_CONTROLLER_BASE_H_

#include <functional>

#include <map>
#include <string>

namespace kiwi {
  namespace http {
    class Request;
    class Response;
  }

  namespace controller {
    class Base {
      public:
      typedef std::map<std::string, std::string> Parameters;

      public:
      Base (void* a_data);

      virtual ~Base ();

      /**
       * Getters / Setters
       */
      public:
      const http::Request* request;
      http::Response* response;

      Parameters params;
    };
  }
}

#endif // KIWI_CONTROLLER_BASE_H_
