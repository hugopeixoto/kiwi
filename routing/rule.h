
#ifndef KIWI_ROUTING_RULE_H_
#define KIWI_ROUTING_RULE_H_

#include <string>
#include <map>
#include <boost/regex.hpp>

namespace kiwi {
  namespace routing {
    class MatchResult {
        public:
        virtual operator bool () const;
        virtual std::string operator[] (const char*) const;

        public:
        bool matched;
        std::map<std::string, std::string> parameters;
    };

    class Rule {
      public:
      Rule (const std::string& a_text)
      {
        static const boost::regex re_symbols(":([a-zA-Z0-9_]+)");
        static const std::string replace_by("(?<\\1>[^./]+)");

        match = regex_replace(
          a_text,
          re_symbols, replace_by,
          boost::regex_constants::match_default | boost::regex_constants::format_sed);

        boost::sregex_iterator re_it(a_text.begin(), a_text.end(), re_symbols);
        boost::sregex_iterator re_end;

        for (; re_it != re_end; ++re_it) {
            keywords.push_back((*re_it)[1].str());
        }
      }

      virtual MatchResult matches (const std::string& a_text) const
      {
        boost::match_results<std::string::const_iterator> mr;
        MatchResult result;

        result.matched = boost::regex_match(a_text, mr, match);
        if (result.matched) {
          for (const auto& keyword : keywords) {
            result.parameters[keyword] = mr[keyword];
          }
        }

        return result;
      }

      boost::regex match;
      std::list<std::string> keywords;

      protected:
      //static std::string regex_escape (const std::string& a_text);
    };
  }
}

#endif // KIWI_ROUTING_RULE_H_

