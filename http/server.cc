
#include "http/server.h"
#include "http/response.h"
#include "http/parser.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/tcp.h>
#include <assert.h>

using kiwi::http::Server;

Server::Server (uint16_t a_port)
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

  parser_ = new Parser();
}

bool Server::begin (Request& a_request, Response& a_response)
{
  char buffer[4096];
  int received;
  int client_socket;
  
  client_socket = accept(server_socket_, NULL, NULL);
  if (client_socket < 0) {
    return false;
  }

  // parse request
  parser_->begin(a_request);
  Status status = Status::INCOMPLETE;

  while (status == Status::INCOMPLETE && (received = recv(client_socket, buffer, 4096, 0)) > 0) {
    status = parser_->feed(buffer, received);
  }

  if (status != Status::COMPLETE) {
    // Reply with a 400 Bad Request.
    close(client_socket);
    return false;
  }

  // setup response
  a_response.set_socket(client_socket);
  close(client_socket);

  return true;
}

bool Server::end (Request& a_request, Response& a_response)
{
  return true;
}

