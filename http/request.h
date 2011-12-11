
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
      const V& operator[](const V& a_value) const {
        typename super::const_iterator it = find(a_value);
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

      const_map<std::string, std::string> params;

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

    inline std::string& Request::uri ()
    {
      return uri_;
    }
  }
}

#endif // HTTP_REQUEST_H_

