
#include "controller/base.h"
#include "http/response.h"
#include "view/base.h"

#include <sstream>

using kiwi::controller::Base;

Base::Base (const std::string& a_name)
{
  name_ = a_name;
}

Base::~Base ()
{
}

bool Base::execute (
    const http::Request& a_request,
    http::Response& a_response,
    const std::string& a_action,
    const Parameters& a_params)
{
  ActionMapType::iterator it = actions_.find(a_action);
  if (it == actions_.end()) {
    return false;
  }

  params = a_params;
  view.clear();

  request = &a_request;
  (it->second.first)();
  request = NULL;

  // render view
  render(it->second.second, a_response);

  // clear parameters
  params.clear();

  return true;
}

bool Base::render (View& a_view, http::Response& a_response)
{
  a_response.start_body();
  a_view();
  a_response.finish_body();
  return true;
}

