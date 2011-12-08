
#include "http_response.h"
#include "http_request.h"
#include <string>
#include <cstdio>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>

http_response* http_response::create (
    int a_file_descriptor,
    const http_request* a_request,
    int a_status)
{
  http_response* response = new http_response;
  if (!response) return NULL;

  response->fd_ = a_file_descriptor; 

  std::string status_line = a_request->http_version + " ";
  status_line += ('0' + (a_status / 100) % 10);
  status_line += ('0' + (a_status / 10) % 10);
  status_line += ('0' + (a_status / 1) % 10);
  status_line += " OK\r\n";

  status_line += "Content-Type: text/html; charset=UTF-8\r\n";
  status_line += "Transfer-Encoding: chunked\r\n";

  send(response->fd_, status_line.c_str(), status_line.size(), 0);
  printf("responded with [%s]\n", status_line.c_str());

  return response;
}

void http_response::start_body ()
{
  send(fd_, "\r\n", 2, 0);
}

void http_response::body_chunk (const char* a_buffer, size_t a_size)
{
  char chunk_size[8 + 2];

  for (int i = 0; i < 8; ++i) {
    uint8_t nibble = (a_size >> ((8 - 1 - i) * 4)) & 0xF;
    chunk_size[i] = (nibble < 0xA ? '0' : 'A' - 10) + nibble;
  }

  int j = 0;
  while (chunk_size[j] == '0') ++j;

  chunk_size[8] = '\r';
  chunk_size[8+1] = '\n';

  send(fd_, chunk_size + j, sizeof(chunk_size) - j, 0);
  send(fd_, a_buffer, a_size, 0);
  send(fd_, "\r\n", 2, 0);
}

void http_response::finish_body ()
{
  send(fd_, "0\r\n\r\n", 5, 0);
}
