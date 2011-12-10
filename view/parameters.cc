
#include "view/parameters.h"

using kiwi::view::Parameters;

void Parameters::clear ()
{
  for (std::map<std::string, Parameter*>::iterator it = params_.begin(); it != params_.end(); ++it) {
    delete it->second;
  }

  params_.clear();
}

void Parameters::add (const std::string& a_name, Parameter* a_param)
{
  std::map<std::string, Parameter*>::iterator it = params_.find(a_name);
  if (it != params_.end()) {
    delete it->second;
  }

  params_[a_name] = a_param;
}

