
#ifndef KIWI_ACTIVERECORD_ITERATOR_H_
#define KIWI_ACTIVERECORD_ITERATOR_H_

#include <string>
#include <map>

namespace kiwi {
  namespace activerecord {
    class Iterator {
      public:
        typedef std::map<std::string, std::string> Row;

        virtual ~Iterator ();
        virtual bool next (Row& a_row) = 0;
    };
  }
}

#endif // KIWI_ACTIVERECORD_ITERATOR_H_

