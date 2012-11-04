
#ifndef KIWI_ACTIVERECORD_POSTGRESQL_ITERATOR_H_
#define KIWI_ACTIVERECORD_POSTGRESQL_ITERATOR_H_

#include "activerecord/iterator.h"
#include <libpq-fe.h>

namespace kiwi {
  namespace activerecord {
    namespace postgresql {
      class Iterator : public activerecord::Iterator {
        public:
          Iterator (void* a_result);

          bool next (Row& a_row);

          protected:
          void* result_;
          uint32_t column_count_;
          uint32_t row_count_;
          uint32_t current_row_;
      };
    }
  }
}

#endif // KIWI_ACTIVERECORD_POSTGRESQL_ITERATOR_H_

