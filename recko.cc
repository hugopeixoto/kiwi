
#include "recko.h"

#include "routing.h"
#include "controller_bag.h"
#include "rack.h"

void recko (routing& routes, controller_bag& controllers, rack& server)
{
  const http_request* request;
  while ((request = server.next())) {
    routing::match_t rm;
    printf("------------------------------------------\n");
    printf(":::::::::::::: Got Request (%s)\n", request->request_uri.c_str());

    if (!routes.match(request->method, request->request_uri, rm))
    { 
      printf("Routing error\n");
    } else {
      printf(":::::::::::::: Routing (%s / %s)\n", rm.controller.c_str(), rm.action.c_str());

      for (std::map<std::string, std::string>::iterator it = rm.params.begin(); it != rm.params.end(); ++it) {
        printf("::: QS [%s]=[%s]\n", it->first.c_str(), it->second.c_str());
      }

      if (!controllers.execute(request, rm.controller, rm.action)) {
        printf("Controller/action missing\n");
      }
    }

    close(request->socket);
    delete request;
    printf("------------------------------------------\n");
  }
}


