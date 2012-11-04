
#ifndef KIWI_ACTIVERECORD_CONNECTION_H_
#define KIWI_ACTIVERECORD_CONNECTION_H_

#include <string>

namespace kiwi {
  namespace activerecord {
    class Iterator;
    class Connection {
      public:
        virtual ~Connection ();
        virtual bool connect (const std::string& a_db_name) = 0;
        virtual Iterator* execute (const std::string& a_query) = 0;
    };
  }
}

#endif // KIWI_ACTIVERECORD_CONNECTION_H_

