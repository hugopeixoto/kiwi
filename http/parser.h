#ifndef KIWI_HTTP_PARSER_H_
#define KIWI_HTTP_PARSER_H_

namespace kiwi {
  namespace http {
    class Request;

    enum class Status {
      ERROR,
      INCOMPLETE,
      COMPLETE
    };

    class Parser {
      public:
      void begin (Request& a_request);

      Status feed (const char* a_buffer, size_t a_size);

      protected:
      Request* request_;
    };
  }
}

#endif // KIWI_HTTP_PARSER_H_
