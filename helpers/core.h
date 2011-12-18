
#ifndef HELPERS_CORE_H_
#define HELPERS_CORE_H_

#include <string>
#include <sstream>

#include "model/base.h"

namespace kiwi {
  namespace helpers {
    template<typename Model>
    std::string path (const Model* a_object)
    {
      std::ostringstream sout;
      sout << "/" << Model::model_name_ << "s/" << a_object->id();
      return sout.str();
    }

    template<typename Model>
    std::string link_to (const std::string& a_text, const Model* a_object) {
      std::ostringstream sout;
      sout << "<a href='" << path(a_object) << "'>" << a_text << "</a>";
      return sout.str();
    }
  }
}

template<typename T>
std::string html_escape (const T& a_value)
{
  std::ostringstream sout;
  std::string original, escaped;

  sout << a_value;
  original = sout.str();
  for (size_t i = 0; i < original.size(); ++i) {
    switch (original[i]) {
      case '&':
        escaped += "&amp;";
        break;
      case '>':
        escaped += "&gt;";
        break;
      case '<':
        escaped += "&lt;";
        break;
      case '"':
        escaped += "&quot;";
        break;
      default:
        escaped += original[i];
        break;
    }
  }

  return escaped;
}

template<typename T> std::string h (const T& a_value) { return html_escape(a_value); }

#endif // HELPERS_CORE_H_

