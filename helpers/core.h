
#ifndef HELPERS_CORE_H_
#define HELPERS_CORE_H_

#include <string>
#include <sstream>

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

