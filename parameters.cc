
#include "parameters.h"

void parameters::clear ()
{
  for (std::map<std::string, parameter*>::iterator it = params_.begin(); it != params_.end(); ++it) {
    delete it->second;
  }

  params_.clear();
}

void parameters::add (const std::string& a_name, parameter* a_param)
{
  std::map<std::string, parameter*>::iterator it = params_.find(a_name);
  if (it != params_.end()) {
    delete it->second;
  }

  params_[a_name] = a_param;
}

