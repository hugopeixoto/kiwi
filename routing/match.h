
#ifndef KIWI_ROUTING_MATCH_H_
#define KIWI_ROUTING_MATCH_H_

#include <string>
#include <map>

namespace kiwi {
  namespace routing {
    struct Match {
      void* action;
      
      std::map<std::string, std::string> parameters; //!< Params, zomg
    };
  }
}

#endif // KIWI_ROUTING_MATCH_H_

