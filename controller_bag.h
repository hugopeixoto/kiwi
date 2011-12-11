
#ifndef KIWI_CONTROLLER_ENGINE_H_
#define KIWI_CONTROLLER_ENGINE_H_

#include <string>
#include <map>

namespace kiwi {
  namespace http {
    class Request;
  }

  namespace controller {
    class Base;

    class Engine {
      public:
      void add (Base* a_base);
      bool execute (
          const http::Request&,
          const std::string& a_controller,
          const std::string& a_action);

      protected:
      std::map<std::string, Base*> controllers_;
    };  
  }
}

#endif // KIWI_CONTROLLER_ENGINE_H_

