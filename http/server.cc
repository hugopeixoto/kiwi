
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

Server::~Server ()
{
  if (server_socket_) {
    close(server_socket_);
  }

  delete parser_;
}

bool Server::begin (Request*& a_request, Response*& a_response)
{
  char buffer[4096];
  int received;
  int client_socket;

  a_request = NULL;
  a_response = NULL;
  
  client_socket = accept(server_socket_, NULL, NULL);
  if (client_socket < 0) {
    close(server_socket_);
    server_socket_ = 0;
    return false;
  }

  while (true) {
    // Not really a select loop
    // oh we've got a read event on client_socket wowie

    received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (received < 0) {
      close(client_socket);
      return true;
    }

    if (parser_->feed(buffer, received) == false) {
      close(client_socket);
      return true;
    }

    if (parser_->pop_request(a_request)) {
      a_response = new Response();
      a_response->set_socket(client_socket);
      return true;
    }
  }

  return true;
}

bool Server::end (Request*& a_request, Response*& a_response)
{
  close(a_response->socket());
  delete a_response;
  return true;
}

