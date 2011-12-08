
#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <string>
#include <map>

namespace kiwi {
  namespace http {
    enum class Method;

    class Request {
      public:


      const std::string& uri () const;

      const http::Method& method () const;

      protected:
      std::string uri_;
      http::Method method_;
    };
  }
}

template<typename K, typename V>
class const_map : public std::map<K, V> {
  public:
  typedef std::map<K, V> super;
  const V& operator[](const V& a_value) const {
    typename super::const_iterator it = find(a_value);
    if (it != super::end()) return it->second;
    return default_;
  }

  protected:
  V default_;
};


struct http_request {

  static http_request* parse (int a_file_descriptor);

  protected:
  http_request () { state_ = 0; }
  bool feed (const char* a_buffer, size_t a_size);
  size_t extract_method (const char* a_buffer, size_t a_size);
  size_t extract_request_uri (const char* a_buffer, size_t a_size);
  size_t extract_http_version (const char* a_buffer, size_t a_size);
  size_t ignore_headers (const char* a_buffer, size_t a_size);

  public:
  const_map<std::string, std::string> headers;
  const_map<std::string, std::string> params;
  http_method method;
  std::string request_uri;
  std::string http_version;
  std::string body;
  int socket;

  protected:
  int state_;
  std::string tmp_method_;
};

#endif // HTTP_REQUEST_H_

