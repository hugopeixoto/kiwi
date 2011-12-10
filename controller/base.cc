
#include "controller/base.h"
#include "http/response.h"
#include <sstream>

using kiwi::controller::Base;

Base::Base (const std::string& a_name)
{
  name_ = a_name;
}

bool Base::execute (
    const http::Request& a_request,
    http::Response& a_response,
    const std::string& a_action)
{
  ActionMapType::iterator it = actions_.find(a_action);
  if (it == actions_.end()) {
    return false;
  } else {
    (it->second.first)(a_request);

    // render view
    std::ostringstream sout;
    (it->second.second)(sout, params);

    // send it
    a_response.start_body();

    std::string response_text(sout.str());
    a_response.body_chunk(response_text.c_str(), response_text.size());

    a_response.finish_body();

    // clear parameters
    params.clear();

    return true;
  }
}

