
#ifndef KIWI_ACTIVERECORD_POSTGRESQL_CONNECTION_H_
#define KIWI_ACTIVERECORD_POSTGRESQL_CONNECTION_H_

#include "activerecord/connection.h"

namespace kiwi {
  namespace activerecord {
    class Iterator;
    namespace postgresql {
      class Connection : public activerecord::Connection {
        public:
          Connection ();
          virtual bool connect (const std::string& a_db_name);
          virtual Iterator* execute (const std::string& a_query);

        protected:
          void* connection_;
      };
    }
  }
}

#endif // KIWI_ACTIVERECORD_POSTGRESQL_CONNECTION_H_

