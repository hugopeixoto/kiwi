
#include "application/base.h"
#include "controller/engine.h"

#include "http/server.h"
#include "http/request.h"
#include "http/response.h"

#include "routing/match.h"
#include "routing/base.h"

kiwi::application::Base::Base ()
{
  routing_ = NULL;
  controller_engine_ = new kiwi::controller::Engine();
}

void kiwi::application::Base::set_routing (kiwi::routing::Base* a_routing)
{
  routing_ = a_routing;
}

void kiwi::application::Base::add_controller (kiwi::controller::Base* a_controller)
{
  controller_engine_->add(a_controller);
}

void kiwi::application::Base::run (uint16_t a_port)
{
  http::Request*  request;
  http::Response* response;
  http::Server    server(a_port);

  while (server.begin(request, response) == true) {
    if (request && response) {
      routing::Match routing_match;
      fprintf(stderr, "------------------------------------------\n");
      fprintf(stderr, ":::::::::::::: Got Request (%s)\n", request->uri().c_str());

      if (routing_->match(request->method(), request->uri(), routing_match) == false) { 
        fprintf(stderr, "Routing error\n");
      } else {
        printf(
          ":::::::::::::: Routing (%s / %s)\n",
          routing_match.controller.c_str(),
          routing_match.action.c_str());

        for (std::map<std::string, std::string>::const_iterator it = routing_match.parameters.begin(); it != routing_match.parameters.end(); ++it) {
          printf("::: QS [%s]=[%s]\n", it->first.c_str(), it->second.c_str());
        }

        if (controller_engine_->execute(
              *request,
              *response,
              routing_match.controller,
              routing_match.action,
              routing_match.parameters) == false) {
          printf("Controller/action missing\n");
        }
      }

      printf("------------------------------------------\n");
      server.end(request, response);
    }
  }
}

