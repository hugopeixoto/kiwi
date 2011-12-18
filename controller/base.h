
#ifndef KIWI_CONTROLLER_BASE_H_
#define KIWI_CONTROLLER_BASE_H_

// #include "parameters.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <map>
#include <string>

#include "view/parameters.h"

namespace kiwi {
  namespace http { class Request; class Response; }
  namespace view { class Parameters; }

  namespace controller {
    class Base {
      protected:
      typedef void (*View) (std::ostream&, const view::Parameters&);
      typedef boost::function<void(const http::Request&)> Action;
      typedef std::map<std::string, std::pair<Action, View>> ActionMapType;

      public:
      Base (const std::string& a_name);

      virtual ~Base ();

      template<typename T>
      void add_action(
        const std::string& a_action_name,
        void (T::*a_action)(const http::Request& a_request),
        View a_view);

      bool execute (
        const http::Request& a_request,
        http::Response& a_response,
        const std::string& a_action,
        const std::map<std::string, std::string>& a_params);

      /**
       * Getters / Setters
       */
      public:
      const std::string& name ();

      std::string name_;
      ActionMapType actions_;
      view::Parameters params;
    };

    inline const std::string& Base::name ()
    {
      return name_;
    }

    template<typename T>
    void Base::add_action (
        const std::string& a_action_name,
        void (T::*a_action)(const http::Request&),
        View a_view)
    {
      actions_[a_action_name] = std::make_pair(
        boost::bind(a_action, reinterpret_cast<T*>(this), _1),
        a_view);
    }
  }
}

#endif // KIWI_CONTROLLER_BASE_H_
