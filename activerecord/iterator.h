
#ifndef KIWI_ACTIVERECORD_ITERATOR_H_
#define KIWI_ACTIVERECORD_ITERATOR_H_

#include <postgresql/libpq-fe.h>
#include <string>
#include <map>

namespace kiwi {
  namespace activerecord {
    class Iterator {
      public:
        Iterator (PGresult*);

        uint32_t columns () const { return column_count_; }

        bool next (std::map<std::string, std::string>& a_row);

        protected:
        PGresult* result_;
        uint32_t column_count_;
        uint32_t row_count_;
        uint32_t current_row_;
    };
  }
}

#endif // KIWI_ACTIVERECORD_ITERATOR_H_

