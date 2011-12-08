
#ifndef ABSTRACT_BAG_H_
#define ABSTRACT_BAG_H_

#include <string>
#include <map>

class http_request;
class abstract_controller;
namespace kiwi {
  namespace controllers {
    class Engine {
      public:
      void add (abstract_controller*);
      bool execute (const http_request*, const std::string&, const std::string&);

      protected:
      std::map<std::string, abstract_controller*> controllers_;
    };  
  }
}

#endif // ABSTRACT_BAG_H_

