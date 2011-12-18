#ifndef KIWI_HTTP_PARSER_H_
#define KIWI_HTTP_PARSER_H_

#include <string>

#include "http/request.h"

namespace kiwi {
  namespace http {
    class Parser {
      protected:
      typedef void* http_parser_settings_ptr;
      typedef void* http_parser_ptr;

      public:
      Parser ();

      ~Parser ();

      bool feed (const char* a_buffer, size_t a_size);

      bool pop_request (Request*& a_request);

      static std::string percent_decode (const std::string& a_string);

      /**
       * Not so public member functions
       */
      public:
      int on_message_begin ();
      int on_message_complete ();
      int on_headers_complete ();

      int on_url (const char* a_buffer, size_t a_length);
      int on_header_field (const char* a_buffer, size_t a_length);
      int on_header_value (const char* a_buffer, size_t a_length);
      int on_body (const char* a_buffer, size_t a_length);

      protected:
      Request& current_request ();

      /**
       * Member variables
       */
      protected:
      Request requests_[10];
      int first_request_;
      int total_requests_;
      int completed_requests_;

      std::string buffer_;
      int extra_state_;

      http_parser_settings_ptr settings_;
      http_parser_ptr parser_;
    };
  }
}

#endif // KIWI_HTTP_PARSER_H_
