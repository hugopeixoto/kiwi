
#ifndef ROUTING_H_
#define ROUTING_H_

#include "http_request.h"

#include <string>
#include <boost/regex.hpp>
#include <list>
#include <map>

namespace kiwi {
  namespace routing {
    class Rule;
    class Match;

    class Base {
      public:
      /**
       * @brief Destructor. Deletes all the rules.
       */
      virtual ~Base ();

      public:
      /**
       * @brief Adds rules for a RESTful resource, identified by the given name.
       */
      Base& resource (const char* a_plural_resource_name);

      /**
       * @brief Maps the entry point of the application to a given controller/action.
       * This is equivalent to calling Base::map(GET, "/", a_controller, a_action)
       */
      Base& root (const char* a_controller, const char* a_action = "index");

      /**
       * @brief Maps the given matching string to a controller and action.
       */
      Base& map (
          const HTTPMethod& a_method,
          const char* a_match,
          const char* a_controller,
          const char* a_action);

      public:
      /**
       * @brief Determines if the given method and URI match any rules.
       * If they do, the a_match structure is filled with the match result.
       */
      bool match (
          const HTTPMethod& a_method,
          const char* a_uri,
          bool a_output);

      protected:
      std::list<Rule*> rules_; //!< List of rules. To be replaced with a magic automata.
    };
  }
}

class routing {
  public:
  struct match_t {
    http_request::http_method          method;
    std::string                        controller;
    std::string                        action;
    std::map<std::string, std::string> params;
  };

  protected:
  std::string extract_query_string (const std::string& a_uri, match_t& a_match);

  struct rule {
    const boost::regex* regex;
    match_t             match;

    rule (const http_request::http_method& m, const boost::regex* r, const std::string& c, const std::string& a);
    ~rule ();
  };

};


#endif // ROUTING_H_
