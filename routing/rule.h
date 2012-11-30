
#ifndef KIWI_ROUTING_MATCH_H_
#define KIWI_ROUTING_MATCH_H_

#include <string>
#include <vector>
#include <boost/regex.hpp>

#include "http/method.h"

namespace kiwi {
  namespace routing {
    template<typename Target>
    class Rule {
      public:
      Rule (const http::Method& a_method, const std::string& a_text)
      {
        method = a_method;
        match  = a_text;
      }

      bool matches (const http::Method& a_method, const std::string& a_text) const
      {
        return a_method == method && boost::regex_match(a_text, match);
      }

      Target target;
      http::Method method;
      boost::regex match;
      std::vector<std::string> symbols;
    };
  }
}

#endif // KIWI_ROUTING_MATCH_H_

