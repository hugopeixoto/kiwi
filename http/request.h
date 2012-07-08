
#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <string>
#include <map>

namespace kiwi {
  namespace http {
    enum class Method;

    template<typename K, typename V>
    class const_map : public std::map<K, V> {
      public:
      typedef std::map<K, V> super;

      V& operator[](const K& a_key) {
        return super::operator[](a_key);
      }

      bool has_key (const K& a_key) const {
        return find(a_key) != super::end();
      }

      const V& operator[](const K& a_key) const {
        typename super::const_iterator it = find(a_key);
        if (it != super::end()) return it->second;
        return default_;
      }
    
      protected:
      V default_;
    };

    class Request {
      public:
      Request ();

      const std::string& uri () const;
      std::string& uri ();

      const http::Method& method () const;

      void set_method (const http::Method& a_method);

      const_map<std::string, std::string> params;
      const_map<std::string, std::string> headers;

      void clear ();

      protected:
      std::string uri_;
      http::Method method_;
    };

    inline const std::string& Request::uri () const
    {
      return uri_;
    }

    inline const http::Method& Request::method () const
    {
      return method_;
    }

    inline void Request::set_method (const http::Method& a_method)
    {
      method_ = a_method;
    }

    inline std::string& Request::uri ()
    {
      return uri_;
    }
  }
}

#endif // HTTP_REQUEST_H_

