
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
  headers.clear();
}

#include <cstdio>
std::string Request::cookie (const char* a_key) const
{
    const std::string& cookies = headers["Cookie"];

    printf("cookies: %s\n", cookies.c_str());

    return cookies.substr(cookies.find("=") + 1);
}

