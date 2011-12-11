
#include "routing/base.h"
#include "routing/match.h"
#include "http/method.h"

#include <boost/regex.hpp>
#include <iomanip>
#include <vector>

using kiwi::routing::Base;

namespace kiwi { namespace routing {
struct Rule {
  http::Method method;
  boost::regex match;
  std::string  controller;
  std::string  action;
  std::vector<std::string> symbols;
};
} }

std::string regex_escape(const std::string& string_to_escape)
{
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
  std::string escaped = regex_escape(a_match);
  static const boost::regex re_symbols(":([a-zA-Z0-9_]+)");
  static const std::string replace_by("(?<\\1>[^./]+)");

  Rule* r       = new Rule();
  r->method     = a_method;
  r->controller = a_controller;
  r->action     = a_action;
  r->match      = regex_replace(
                    a_match,
                    re_symbols,
                    replace_by,
                    boost::regex_constants::match_default | boost::regex_constants::format_sed);

  std::cerr << "routing rule: " << std::setw(42) << a_match << " generated: " << r->match << std::endl;

  boost::sregex_iterator re_it(a_match.begin(), a_match.end(), re_symbols);
  boost::sregex_iterator end;

  for (; re_it != end; ++re_it) {
    r->symbols.push_back((*re_it)[1].str());
  }

  rules_.push_back(r);
  return *this;
}

// Match finding API

bool Base::match (
    const http::Method& a_method,
    const std::string& a_uri,
    Match& a_match)
{
  boost::match_results<std::string::const_iterator> mr;

  for (Rule* rule : rules_) {
    if (a_method == rule->method && boost::regex_match(a_uri, mr, rule->match)) {
      a_match.controller = rule->controller;
      a_match.action = rule->action;
      for (const std::string& symbol : rule->symbols) {
        a_match.parameters[symbol] = mr[symbol];
      }
      return true;
    }
  }

  return false;
}

