
#include "routing/rule.h"

using kiwi::routing::Rule;
using kiwi::routing::MatchResult;

MatchResult::operator bool () const
{
    return matched;
}

std::string MatchResult::operator[] (const char* a_key) const
{
    auto match = parameters.find(a_key);
    if (match == parameters.end()) {
        return "";
    } else {
        return match->second;
    }
}

//std::string Rule::regex_escape (const std::string& a_text)
//{
//  static const boost::regex re_boostRegexEscape("[\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\]");
//  const std::string replace_by("\\\\\\1&");
//  return regex_replace(
//    a_text,
//    re_boostRegexEscape,
//    replace_by,
//    boost::regex_constants::match_default | boost::regex_constants::format_sed);
//}

