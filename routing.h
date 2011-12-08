
#ifndef ROUTING_H_
#define ROUTING_H_

#include "http_request.h"

#include <string>
#include <boost/regex.hpp>
#include <list>
#include <map>

class routing {
  public:
  struct match_t {
    http_request::http_method          method;
    std::string                        controller;
    std::string                        action;
    std::map<std::string, std::string> params;
  };

  virtual ~routing ();
  virtual void setup() = 0;

  routing& resource (const std::string& a_plural_resource_name);
  routing& map (
      const http_request::http_method& a_method,
      const boost::regex* a_regex,
      const std::string& a_controller,
      const std::string& a_action);

  routing& root (const std::string& a_controller, const std::string& a_action="index");

  bool match (const http_request::http_method& a_method, const std::string& a_uri, match_t& a_match);

  protected:
  std::string extract_query_string (const std::string& a_uri, match_t& a_match);

  struct rule {
    const boost::regex* regex;
    match_t             match;

    rule (const http_request::http_method& m, const boost::regex* r, const std::string& c, const std::string& a);
    ~rule ();
  };
  std::list<routing::rule*> rules_;
};


#endif // ROUTING_H_
