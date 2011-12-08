
#include "http/parser.h"

using kiwi::http::Parser;

void Parser::begin (Request& a_request)
{
  request_ = &a_request;
}

kiwi::http::Status Parser::feed (const char* a_buffer, size_t a_size)
{
}

