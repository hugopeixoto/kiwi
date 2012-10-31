
#ifndef KIWI_ACTIVERECORD_BASE_H_
#define KIWI_ACTIVERECORD_BASE_H_

namespace kiwi {
  namespace activerecord {
    class Connection;
    class Base {
      public:
      static Connection* connection;
    };
  }
}

#endif // KIWI_ACTIVERECORD_BASE_H_

