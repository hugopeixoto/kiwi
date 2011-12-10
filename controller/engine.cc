
#include "controller/engine.h"
#include "controller/base.h"

using kiwi::controller::Engine;

void Engine::add (Base* a_controller) {
  controllers_[a_controller->name()] = a_controller;
}

bool Engine::execute (
    const http::Request&  a_request,
    http::Response&       a_response,
    const std::string&    a_controller,
    const std::string&    a_action)
{
  std::map<std::string, Base*>::const_iterator it = controllers_.find(a_controller);
  if (it == controllers_.end()) {
    fprintf(stderr, "Controller [%s] not found!\n", a_controller.c_str());
    return false;
  }

  return it->second->execute(a_request, a_response, a_action);
}


