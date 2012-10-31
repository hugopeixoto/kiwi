
#ifndef KIWI_ACTIVERECORD_CONNECTION_H_
#define KIWI_ACTIVERECORD_CONNECTION_H_

#include <postgresql/libpq-fe.h>
#include <string>

namespace kiwi {
  namespace activerecord {
    class Iterator;
    class Connection {
      public:
        Connection () {}

        bool connect (const std::string& a_db_name);

        Iterator* execute (const std::string& a_query);

        protected:
        PGconn* connection_;
    };
  }
}

#endif // KIWI_ACTIVERECORD_CONNECTION_H_

