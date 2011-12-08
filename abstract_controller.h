
#ifndef ABSTRACT_CONTROLLER_
#define ABSTRACT_CONTROLLER_

#include "parameters.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <map>
#include <string>

class http_request;
class abstract_controller {
  public:
  abstract_controller (const std::string& a_name)
  {
    name_ = a_name;
  }

  template<typename T>
  void add_action(
    const std::string& a_action_name,
    void (T::*a_action)(const http_request* a_request),
    void (*a_view)(std::ostream&, const parameters& a_params));


  const std::string& name () { return name_; }

  bool execute (const http_request* a_request, const std::string& a_action);

  protected:
  typedef boost::function<void(const http_request*)> action;
  typedef boost::function<void(std::ostream&, const parameters&)> view;
  typedef std::map<std::string, std::pair<action, view>> ActionMapType;

  std::string name_;
  ActionMapType actions_;
  parameters params;
};

template<typename T>
void abstract_controller::add_action(
  const std::string& a_action_name,
  void (T::*a_action)(const http_request*),
  void (*a_view)(std::ostream&, const parameters&))
{
  actions_[a_action_name] = std::make_pair(
    boost::bind(a_action, reinterpret_cast<T*>(this), _1),
    a_view);
}

#endif // ABSTRACT_CONTROLLER_
