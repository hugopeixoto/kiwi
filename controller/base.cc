
#include "controller/base.h"
#include "controller/data.h"

#include <sstream>

using kiwi::controller::Base;

Base::Base (void* a_data)
{
  controller::Data* data = static_cast<controller::Data*>(a_data);

  request  = data->request;
  response = data->response;
}

Base::~Base ()
{
}

