
#include "http/parser.h"
#include "http_parser.h"

#include <boost/regex.hpp>

using kiwi::http::Parser;

/**
 * Not-so-wonderful hack to avoid header tainting
 */
#define SETTINGS() static_cast<http_parser_settings*>(settings_)
#define PARSER() static_cast<http_parser*>(parser_)

static std::string extract_query_string (
  const std::string& a_uri, 
  std::map<std::string, std::string>& a_params);

typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
typedef int (*http_cb) (http_parser*);

int on_message_begin (http_parser* a_parser)
{
  return static_cast<Parser*>(a_parser->data)->on_message_begin();
}
int on_message_complete (http_parser* a_parser)
{
  return static_cast<Parser*>(a_parser->data)->on_message_complete();
}
int on_headers_complete (http_parser* a_parser)
{
  return static_cast<Parser*>(a_parser->data)->on_headers_complete();
}

int on_url (http_parser* a_parser, const char* a_buffer, size_t a_length)
{
  return static_cast<Parser*>(a_parser->data)->on_url(a_buffer, a_length);
}
int on_header_field (http_parser* a_parser, const char* a_buffer, size_t a_length)
{
  return static_cast<Parser*>(a_parser->data)->on_header_field(a_buffer, a_length);
}
int on_header_value (http_parser* a_parser, const char* a_buffer, size_t a_length)
{
  return static_cast<Parser*>(a_parser->data)->on_header_value(a_buffer, a_length);
}
int on_body (http_parser* a_parser, const char* a_buffer, size_t a_length)
{
  return static_cast<Parser*>(a_parser->data)->on_body(a_buffer, a_length);
}

Parser::Parser ()
{
  total_requests_ = 0;
  first_request_ = 0;
  completed_requests_ = 0;

  settings_ = static_cast<http_parser_settings_ptr>(new http_parser_settings());
  parser_ = static_cast<http_parser_ptr>(new http_parser());

  SETTINGS()->on_message_begin    = ::on_message_begin;
  SETTINGS()->on_message_complete = ::on_message_complete;
  SETTINGS()->on_headers_complete = ::on_headers_complete;

  SETTINGS()->on_url = ::on_url;
  SETTINGS()->on_header_field = ::on_header_field;
  SETTINGS()->on_header_value = ::on_header_value;
  SETTINGS()->on_body = ::on_body;

  http_parser_init(PARSER(), HTTP_REQUEST);
  PARSER()->data = this;
}

bool Parser::feed (const char* a_buffer, size_t a_size)
{
  return http_parser_execute(PARSER(), SETTINGS(), a_buffer, a_size) == a_size;
}

bool Parser::pop_request (Request*& a_request)
{
  if (completed_requests_ == 0) {
    return false;
  }

  a_request = &requests_[first_request_];
  first_request_ = (first_request_ + 1) % 10;
  --completed_requests_;
  --total_requests_;
  return true;
}

kiwi::http::Request& Parser::current_request ()
{
  return requests_[(first_request_ + total_requests_ - 1) % 10];
}

int Parser::on_message_begin ()
{
  if (total_requests_ == 10) {
    return -1;
  }

  ++total_requests_;
  current_request().clear();
  return 0;
}

int Parser::on_message_complete ()
{
  ++completed_requests_;
  return 0;
}

int Parser::on_headers_complete ()
{
  // extract query string from URL
  current_request().uri().assign(extract_query_string(current_request().uri(), current_request().params));
  return 0;
}

int Parser::on_url (const char* a_buffer, size_t a_length)
{
  current_request().uri().append(a_buffer, a_length);
  return 0;
}

int Parser::on_header_field (const char* a_buffer, size_t a_length)
{
  return 0;
}

int Parser::on_header_value (const char* a_buffer, size_t a_length)
{
  return 0;
}

int Parser::on_body (const char* a_buffer, size_t a_length)
{
  return 0;
}

std::string extract_query_string (
    const std::string& a_uri,
    std::map<std::string, std::string>& a_params)
{
  static const boost::regex qse_re("^(?<ABS_PATH>[^?]*)(\\?(?<QS>.*))?$");
  boost::match_results<std::string::const_iterator> results;
  boost::regex_match(a_uri, results, qse_re);
  
  boost::sub_match<std::string::const_iterator> qs_match = results["QS"];
  if (qs_match.matched) {
    std::string qs = qs_match.str();
    for (size_t i = 0; i < qs.size();) {
      size_t j = qs.find_first_of("&=", i);
      if (j == std::string::npos) {
        a_params[qs.substr(i)] = "";
        i = qs.size();
      } else {
        if (qs[j] == '&') {
          a_params[qs.substr(i, j - i)] = "";
          i = j + 1;
        } else {
          size_t k = qs.find_first_of("&", j + 1);
          if (k == std::string::npos) {
            a_params[qs.substr(i, j - i)] = qs.substr(j + 1);
            i = qs.size();
          } else {
            a_params[qs.substr(i, j - i)] = qs.substr(j + 1, k - (j + 1));
            i = k + 1;
          }
        }
      }
    }
  }

  return results["ABS_PATH"].str();
}


