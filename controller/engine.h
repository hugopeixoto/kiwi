
#ifndef KIWI_CONTROLLER_ENGINE_H_
#define KIWI_CONTROLLER_ENGINE_H_

#include <string>
#include <map>

namespace kiwi {
  namespace http {
    class Request;
    class Response;
  }

  namespace controller {
    class Base;

    class Engine {
      public:

      virtual ~Engine ();

      void add (Base* a_base);
      bool execute (
          const http::Request& a_request,
          http::Response& a_response,
          const std::string& a_controller,
          const std::string& a_action,
          const std::map<std::string, std::string>& a_params);

      protected:
      std::map<std::string, Base*> controllers_;
    };  
  }
}

#endif // KIWI_CONTROLLER_ENGINE_H_

