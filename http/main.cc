
#include "http/server.h"
#include "http/response.h"
#include "http/request.h"

#include <cstdio>
#include <cstring>

void keyvalue (kiwi::http::Request* a_request,
               kiwi::http::Response* a_response,
               int64_t a_oid,
               const std::string& a_method)
{
    std::string session_id = a_request->cookie("_session_id");

    char buf[1<<10];
    int x = snprintf(buf, sizeof(buf), "oid: %lld sid: %s m: %s\n", a_oid, session_id.c_str(), a_method.c_str());

    a_response->start_body();
    a_response->body_chunk(buf, x);
    a_response->finish_body();
}

void sync (kiwi::http::Request* a_request,
           kiwi::http::Response* a_response)
{
    const char* payload = "sync!\n";
    a_response->start_body();
    a_response->body_chunk(payload, strlen(payload));
    a_response->finish_body();
}


// TODO(hpeixoto): This is kind of the routing section. Grab a module from kiwi.
void process (kiwi::http::Request* a_request,
              kiwi::http::Response* a_response)
{
    std::string sync_prefix = "/sync/";
    std::string keystore_prefix = "/keyvalue/";

    if (a_request->uri().compare(0, sync_prefix.size(), sync_prefix) == 0) {
        // Format: /sync/
        sync(a_request, a_response);
    } else if (a_request->uri().compare(0, keystore_prefix.size(), keystore_prefix) == 0) {
        // Format: /keyvalue/:id/:method
        int64_t oid = strtol(a_request->uri().c_str() + keystore_prefix.size(), NULL, 10);
        size_t pos = a_request->uri().find('/', 10);
        std::string method = "read";
        if (pos != std::string::npos) {
            method = a_request->uri().substr(pos + 1);
        }

        keyvalue(a_request, a_response, oid, method);
    }
}

int main ()
{
    kiwi::http::Server server;
    kiwi::http::Request* request;
    kiwi::http::Response* response;

    server.construct(3000);

    while (server.begin(request, response))
    {
        if (request) {
            process(request, response);
            server.end(request, response);
        }
    }

    return 0;
}

