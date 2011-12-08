
#include "application/base.h"
#include "controller/engine.h"

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
  controller_engine_->add(a_controller)
}

void kiwi::application::Base::run (uint16_t a_port)
{
  const http_request* request;
  rack server(a_port);

  while ((request = server.next())) {
    routing::match_t rm;
    printf("------------------------------------------\n");
    printf(":::::::::::::: Got Request (%s)\n", request->request_uri.c_str());

    if (!routing_->match(request->method, request->request_uri, rm))
    { 
      printf("Routing error\n");
    } else {
      printf(":::::::::::::: Routing (%s / %s)\n", rm.controller.c_str(), rm.action.c_str());

      for (std::map<std::string, std::string>::iterator it = rm.params.begin(); it != rm.params.end(); ++it) {
        printf("::: QS [%s]=[%s]\n", it->first.c_str(), it->second.c_str());
      }

      if (!controller_engine_->execute(request, rm.controller, rm.action)) {
        printf("Controller/action missing\n");
      }
    }

    close(request->socket);
    delete request;
    printf("------------------------------------------\n");
  }
}

