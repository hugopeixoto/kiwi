
#include "routing/base.h"
#include "routing/match.h"
#include "http/method.h"

using kiwi::routing::Base;

namespace kiwi { namespace routing {
struct Rule {
  http::Method method;
  std::string  match;
  std::string  controller;
  std::string  action;
};
} }

Base::~Base ()
{
  for (std::list<Rule*>::iterator it = rules_.begin(); it != rules_.end(); ++it) {
    delete *it;
  }
}

// Public rule-adding API

Base& Base::resource (const std::string& a_plural_resource_name)
{
  std::string escaped = a_plural_resource_name;//escape(a_plural_resource_name);

  map(http::Method::GET, "/" + escaped, a_plural_resource_name, "index");
  map(http::Method::GET, "/" + escaped + "/:id", a_plural_resource_name, "show");

  map(http::Method::GET, "/" + escaped + "/:id/edit", a_plural_resource_name, "edit");
  map(http::Method::PUT, "/" + escaped + "/:id", a_plural_resource_name, "update");

  map(http::Method::GET, "/" + escaped + "/:id/new", a_plural_resource_name, "new");
  map(http::Method::POST, "/" + escaped + "/:id", a_plural_resource_name, "create");

  // map(http::Method::GET, "/" + escaped + "/:id/delete", a_plural_resource_name, "delete");
  map(http::Method::DELETE, "/" + escaped + "/:id", a_plural_resource_name, "destroy");

  return *this;
}

Base& Base::root (const std::string& a_controller, const std::string& a_action)
{
  return this->map(http::Method::GET, "/", a_controller, a_action);
}

Base& Base::map (
    const http::Method& a_method,
    const std::string& a_match,
    const std::string& a_controller,
    const std::string& a_action)
{
  Rule* r = new Rule();
  r->method = a_method;
  r->match = a_match;
  r->controller = a_controller;
  r->action = a_action;

  rules_.push_back(r);
  return *this;
}

// Match finding API

bool Base::match (
    const http::Method& a_method,
    const std::string& a_uri,
    Match& a_match)
{
  for (Rule* rule : rules_) {
    if (a_method == rule->method && a_uri == rule->match) {
      a_match.controller = rule->controller;
      a_match.action = rule->action;
      return true;
    }
  }
#if 0
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
#endif

  return false;
}

