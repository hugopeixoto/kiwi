
#ifndef KIWI_CONTROLLER_DATA_H_
#define KIWI_CONTROLLER_DATA_H_

namespace kiwi {
namespace http {
  class Request;
  class Response;
}

namespace controller {
  struct Data {
    http::Request* request;
    http::Response* response;
  };
}
}

#endif

