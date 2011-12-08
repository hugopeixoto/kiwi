
#include "abstract_controller.h"
#include "http_response.h"
#include "http_request.h"
#include <sstream>

bool abstract_controller::execute (const http_request* a_request, const std::string& a_action)
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
    http_response* response = http_response::create(a_request->socket, a_request, 200);
    response->start_body();

    std::string response_text(sout.str());
    response->body_chunk(response_text.c_str(), response_text.size());

    response->finish_body();
    delete response;

    // clear parameters
    params.clear();

    return true;
  }
}


