
#include "controller_bag.h"

#include "abstract_controller.h"

void controller_bag::add(abstract_controller* a_controller) {
  controllers_[a_controller->name()] = a_controller;
}

bool controller_bag::execute (
    const http_request* a_request,
    const std::string& a_controller,
    const std::string& a_action)
{
  std::map<std::string, abstract_controller*>::iterator it = controllers_.find(a_controller);
  if (it == controllers_.end()) {
    printf("Controller not found!\n");
    return false;
  }

  return it->second->execute(a_request, a_action);
}


