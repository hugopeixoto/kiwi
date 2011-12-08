
#include "routing/base.h"

using kiwi::routin::Base;

static std::string qs_regex("(?:\\?(?<QS>.*))?$");

std::string regex_escape(const std::string& string_to_escape) {
  static const boost::regex re_boostRegexEscape("[\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\]");
  const std::string replace_by("\\\\\\1&");
  return regex_replace(
      string_to_escape,
      re_boostRegexEscape,
      replace_by,
      boost::regex_constants::match_default | boost::regex_constants::format_sed);
}

Base::~Base ()
{
  for (std::list<rule*>::iterator it = rules_.begin(); it != rules_.end(); ++it) {
    delete *it;
  }
}

routing& routing::resource (const char* a_plural_resource_name)
{
  std::string escaped = escape(a_plural_resource_name);

  map(GET, "/" + escaped, a_plural_resource_name, "index");
  map(GET, "/" + escaped + "/:id", a_plural_resource_name, "show");

  map(GET, "/" + escaped + "/:id/edit", a_plural_resource_name, "edit");
  map(PUT, "/" + escaped + "/:id", a_plural_resource_name, "update");

  map(GET, "/" + escaped + "/:id/new", a_plural_resource_name, "new");
  map(POST, "/" + escaped + "/:id", a_plural_resource_name, "create");

  // map(GET, "/" + escaped + "/:id/delete", a_plural_resource_name, "delete");
  map(DELETE, "/" + escaped + "/:id", a_plural_resource_name, "destroy");

  return *this;
}

Base& Base::root (const char* a_controller, const char* a_action)
{
  return this->map(http_request::GET, "/", a_controller, a_action);
}

Base& Base::map (
    const HTTPMethod& a_method,
    const char* a_match,
    const char* a_controller,
    const char* a_action)
{
  rules_.push_back(new rule(a_method, a_regex, a_controller, a_action));
  return *this;
}

bool routing::match (
    const http_request::http_method& a_method,
    const std::string& a_uri,
    routing::match_t& a_match)
{
  boost::match_results<std::string::const_iterator> mr;
  for (std::list<rule*>::iterator it = rules_.begin(); it != rules_.end(); ++it) {
    const rule& r = **it;
    if (a_method == r.match.method && boost::regex_match(a_uri, mr, *r.regex)) {
      a_match.controller = r.match.controller;
      a_match.action     = r.match.action;
      a_match.method     = r.match.method;
      std::cout << "matches " << mr.size() << std::endl;

      return true;
    }
  }

  return false;
}

routing::rule::rule (
    const http_request::http_method& m,
    const boost::regex* r,
    const std::string& c,
    const std::string& a)
{
  regex = r;
  match.method     = m;
  match.controller = c;
  match.action     = a;
}

