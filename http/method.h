
#ifndef KIWI_HTTP_METHOD_H_
#define KIWI_HTTP_METHOD_H_

#include <stdint.h>

namespace kiwi {
  namespace http {
    enum class Method : uint8_t {
      GET,
      POST,
      PUT,
      DELETE,
      HEAD
    };
  }
}

#endif // KIWI_HTTP_METHOD_H_

