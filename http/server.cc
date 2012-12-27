
#include "http/server.h"
#include "http/response.h"
#include "http/parser.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/tcp.h>
#include <assert.h>

#include <list>

using kiwi::http::Server;

struct Connection {
    int fd;
    kiwi::http::Parser parser;
    kiwi::http::Response response;

    Connection () {
        fd = -1;
    }

    bool in_use () const { return fd != -1; }
    bool ready () const { return false; }

    void close ()
    {
        ::close(fd);
        parser.reset();
        fd = -1;
    }
};

struct Server::Implementation {
    const static int MAX_CONNECTIONS = 16;

    int server_fd;
    Connection connections[MAX_CONNECTIONS];
    int nconnections;
    int maxfd;
    char* buffer;

    void recalc ()
    {
        maxfd = server_fd;
        for (uint32_t i = 0; i < MAX_CONNECTIONS; ++i) {
            if (connections[i].in_use()) {
                maxfd = std::max(maxfd, connections[i].fd);
            }
        }
    }
};

Server::Server ()
{
    data = new Implementation();
    data->server_fd = -1;
    data->nconnections = 0;
    data->buffer = new char[1<<12];
}

bool Server::construct (uint16_t a_port)
{
    int reuseaddr = 1;
    sockaddr_in server;

    data->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (data->server_fd < 0) return false;

    data->recalc();

    setsockopt(data->server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    server.sin_family = AF_INET;
    server.sin_port = htons(a_port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(data->server_fd, (sockaddr*)&server, sizeof(server)) != 0) return false;
    if (listen(data->server_fd, 10) != 0) return false;
    return true;
}

Server::~Server ()
{
    close(data->server_fd);
    delete[] data->buffer;
}

bool Server::close (uint32_t a_idx)
{
    data->connections[a_idx].close();
    --data->nconnections;
    return true;
}

bool Server::accept ()
{
    if (data->nconnections == Implementation::MAX_CONNECTIONS) {
        return true;
    }

    int fd = ::accept(data->server_fd, NULL, NULL);
    if (fd < 0) {
        return false;
    }

    for (uint32_t i = 0; i < Implementation::MAX_CONNECTIONS; ++i) {
        if (!data->connections[i].in_use()) {
            //printf("pos %d\n", i);
            data->connections[i].fd = fd;
            data->connections[i].response.set_socket(fd);
            ++data->nconnections;
            if (fd > data->maxfd) data->maxfd = fd;
            break;
        }
    }

    return true;
}

bool Server::begin (Request*& a_request, Response*& a_response)
{ // TODO(hpeixoto): this should go into a C file or something.
    a_request = NULL;
    a_response = NULL;

    for (uint32_t i = 0; i < Implementation::MAX_CONNECTIONS; ++i) {
        if (data->connections[i].parser.pop_request(a_request)) {
            a_response = &data->connections[i].response;
            return true;
        }
    }


    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(data->server_fd, &fds);
    for (uint32_t i = 0; i < Implementation::MAX_CONNECTIONS; ++i) {
        if (data->connections[i].in_use()) {
          FD_SET(data->connections[i].fd, &fds);
          }
    }

    int active = select(data->maxfd + 1, &fds, NULL, NULL, NULL);

    if (active < 0) {
        return false;
    }

    if (FD_ISSET(data->server_fd, &fds)) {
        if (!accept()) {
            return false;
        }
    }

    for (uint32_t i = 0; i < Implementation::MAX_CONNECTIONS; ++i) {
        if (data->connections[i].in_use()) {
            if (FD_ISSET(data->connections[i].fd, &fds)) {
                receive(i);
            }
        }
    }

    return true;
}

bool Server::receive (uint32_t a_idx)
{
    int received = recv(data->connections[a_idx].fd, data->buffer, 1<<12, 0);
    //printf("received %d bytes\n", received);

    if (received <= 0) {
        close(a_idx);
        return true;
    }

    if (data->connections[a_idx].parser.feed(data->buffer, received) == false) {
        printf("false parsing\n");
        close(a_idx);
        return true;
    }

    return true;
}

bool Server::end (Request*& a_request, Response*& a_response)
{
    for (uint32_t i = 0; i < Implementation::MAX_CONNECTIONS; ++i) {
        if (data->connections[i].fd == a_response->socket()) {
            close(i);
            break;
        }
    }
    return true;
}

