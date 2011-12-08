
#include "routing.h"
#include <iostream>

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

routing::~routing ()
{
  for (std::list<rule*>::iterator it = rules_.begin(); it != rules_.end(); ++it) {
    delete *it;
  }
}
routing::rule::~rule ()
{
  delete regex;
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

routing& routing::resource (const std::string& a_plural_resource_name)
{
  std::string escaped = regex_escape(a_plural_resource_name);
  std::cout << escaped << std::endl;
  map(http_request::GET, new boost::regex("^\\/" + escaped + "\\/?"), a_plural_resource_name, "index");
  map(http_request::GET, new boost::regex("^\\/" + escaped + "\\/(?<id>[^?/]+)"), a_plural_resource_name, "show");
  map(http_request::GET, new boost::regex("^\\/" + escaped + "\\/[^?/]+/edit"), a_plural_resource_name, "edit");

  return *this;
}

routing& routing::map (
    const http_request::http_method& a_method,
    const boost::regex*              a_regex,
    const std::string&               a_controller,
    const std::string&               a_action)
{
  rules_.push_back(new rule(a_method, a_regex, a_controller, a_action));
  return *this;
}

routing& routing::root (const std::string& a_controller, const std::string& a_action)
{
  return map(http_request::GET, new boost::regex("^\\/"), a_controller, a_action);
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

