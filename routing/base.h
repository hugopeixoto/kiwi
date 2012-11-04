
#ifndef ROUTING_H_
#define ROUTING_H_

#include <string>
#include <list>

namespace kiwi {
  namespace http {
    enum class Method;
  }

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
      Base& resource (const std::string& a_plural_resource_name);

      /**
       * @brief Maps the entry point of the application to a given controller/action.
       * This is equivalent to calling Base::map(GET, "/", a_controller, a_action)
       */
      Base& root (const std::string& a_controller, const std::string& a_action = "index");

      /**
       * @brief Maps the given matching string to a controller and action.
       */
      Base& map (
          const http::Method& a_method,
          const std::string& a_match,
          const std::string& a_controller,
          const std::string& a_action);

      public:
      /**
       * @brief Determines if the given method and URI match any rules.
       * If they do, the a_match structure is filled with the match result.
       */
      bool match (
          const http::Method& a_method,
          const std::string& a_uri,
          Match& a_match);

      protected:
      std::list<Rule*> rules_; //!< List of rules. To be replaced with a magic automata.
    };
  }
}

#endif // ROUTING_H_

