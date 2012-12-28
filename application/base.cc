
#include "application/base.h"
#include "controller/data.h"

#include "http/server.h"
#include "http/request.h"
#include "http/response.h"

#include "routing/base.h"

static kiwi::routing::Method http2routing (const kiwi::http::Method a_method)
{
  static kiwi::routing::Method methods[] = {
    kiwi::routing::Method::GET,
    kiwi::routing::Method::POST,
    kiwi::routing::Method::PUT,
    kiwi::routing::Method::DELETE,
    kiwi::routing::Method::HEAD
  };

  return methods[static_cast<uint32_t>(a_method)];
}

kiwi::application::Base::Base ()
{
  routing_ = NULL;
}

kiwi::application::Base::~Base ()
{
  delete routing_;
}

void kiwi::application::Base::set_routing (kiwi::routing::Base* a_routing)
{
  routing_ = a_routing;
}

void kiwi::application::Base::run (uint16_t a_port)
{
  http::Request*  request;
  http::Response* response;
  http::Server    server;
  kiwi::controller::Data data;

  server.construct(a_port);

  while (server.begin(request, response) == true) {
    if (request && response) {
      fprintf(stderr, "------------------------------------------\n");
      fprintf(stderr, ":::::::::::::: Got Request (%s)\n", request->uri().c_str());

      routing::RouteResult route = routing_->recognize_path(
          http2routing(request->method()),
          request->uri());

      if (!route) {
        fprintf(stderr, "Routing error\n");
      } else {
        data.request = request;
        data.response = response;
        // data.url_params ?
        route.action(&data);
      }

      printf("------------------------------------------\n");
      server.end(request, response);
    }
  }
}

