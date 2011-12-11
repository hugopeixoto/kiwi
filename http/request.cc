
#include "http/request.h"

using kiwi::http::Request;

Request::Request ()
{
  clear();
}

void Request::clear ()
{
  uri_ = "";
  params.clear();
}

