
#include "routing/base.h"

#include <boost/regex.hpp>
#include <iomanip>
#include <vector>

using kiwi::routing::Base;
using kiwi::routing::RouteResult;

//Base::~Base ()
//{
//  for (std::list<Rule*>::iterator it = rules_.begin(); it != rules_.end(); ++it) {
//    delete *it;
//  }
//}

RouteResult Base::recognize_path (
    const Method& a_method,
    const std::string& a_uri)
{
    RouteResult result;
    result.matched = false;

    for (auto rule : rules_[a_method]) {
        MatchResult r = rule.first->matches(a_uri);
        if (r) {
            result.matched = true;
            result.parameters = r.parameters;
            result.action = rule.second;
            break;
        }
    }

    return result;
}

RouteResult::operator bool () const
{
    return matched;
}

std::string RouteResult::operator[] (const char* a_key) const
{
    auto match = parameters.find(a_key);
    if (match == parameters.end()) {
        return "";
    } else {
        return match->second;
    }
}

