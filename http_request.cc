
#include "http_request.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdio>
#include <boost/regex.hpp>

static std::string extract_query_string (const std::string& a_uri, std::map<std::string, std::string>& a_params)
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

static std::string decode_uri (const std::string& a_uri)
{
  std::string decoded;
  
  for (size_t i = 0; i < a_uri.size(); ++i) {
    if (a_uri[i] == '%' && i + 2 < a_uri.size()) {
      char c[3] = { a_uri[i + 1], a_uri[i + 2], 0 };
      decoded += (char)strtol(c, NULL, 16);
      i += 2;
    } else if (a_uri[i] == '+') {
      decoded += ' ';
    } else {
      decoded += a_uri[i];
    }
  }

  return decoded;
}

bool http_request::feed (const char* a_buffer, size_t a_size) {
  while (a_size) {
    size_t r;
    if (state_ == 0) { r = extract_method(a_buffer, a_size); }
    else if (state_ == 1) { r = extract_request_uri(a_buffer, a_size); }
    else if (state_ == 2) { r = extract_http_version(a_buffer, a_size); }
    else { r = ignore_headers(a_buffer, a_size); }

    a_buffer += r;
    a_size -= r;
  }

  return true;
}

size_t read_space_parameter (const char* a_buffer, size_t a_size, std::string& a_output)
{
  size_t r = 0;
  const char* end = a_buffer + a_size;
  while (a_buffer < end && !isspace(*a_buffer)) {
    a_output += *a_buffer;
    ++r;
    ++a_buffer;
  }

  return r;
}

size_t http_request::ignore_headers (const char* a_buffer, size_t a_size)
{
  return a_size;
}

size_t http_request::extract_http_version (const char* a_buffer, size_t a_size)
{
  size_t r = read_space_parameter(a_buffer, a_size, http_version);
  if (r < a_size) { ++r; ++state_; }
  return r;
}

size_t http_request::extract_request_uri (const char* a_buffer, size_t a_size)
{
  size_t r = read_space_parameter(a_buffer, a_size, request_uri);
  if (r < a_size) { ++r; ++state_; }
  return r;
}

size_t http_request::extract_method (const char* a_buffer, size_t a_size)
{
  size_t r = read_space_parameter(a_buffer, a_size, tmp_method_);
  if (r < a_size) {
    ++r;
    ++state_;
    if (tmp_method_ == "GET") method = GET;
    if (tmp_method_ == "POST") method = POST;
    if (tmp_method_ == "PUT") method = PUT;
    if (tmp_method_ == "DELETE") method = DELETE;
  }
  return r;
}

http_request* http_request::parse (int a_file_descriptor)
{
  http_request* req = new http_request();
  req->socket = a_file_descriptor;

  char buffer[4096];
  int received = 0;
  while ((received = recv(a_file_descriptor, buffer, 4096, 0)) > 0) {
    printf("yes data\n");
    if (req->feed(buffer, received)) {
      req->request_uri = decode_uri(req->request_uri);
      req->request_uri = extract_query_string(req->request_uri, req->params);
      printf("method: [%s]\n", req->tmp_method_.c_str());
      printf("requri: [%s]\n", req->request_uri.c_str());
      printf("http v: [%s]\n", req->http_version.c_str());

      break;
    }
  }

  return req;
}


