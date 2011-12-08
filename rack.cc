
#include "rack.h"

#include "http_request.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/tcp.h>
#include <assert.h>


rack::rack (uint16_t a_port)
{
  sockaddr_in server;
  int derp = 1;

  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  assert(server_socket_ > 0);
  setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &derp, sizeof(derp));

  server.sin_family = AF_INET;
  server.sin_port = htons(a_port);
  server.sin_addr.s_addr = INADDR_ANY;

  assert(0 == bind(server_socket_, (sockaddr*)&server, sizeof(server)));
  assert(0 == listen(server_socket_, 10));
}

const http_request* rack::next ()
{
  int client_socket = accept(server_socket_, NULL, NULL);
  if (client_socket < 0) { return NULL; }
  return http_request::parse(client_socket);
}

