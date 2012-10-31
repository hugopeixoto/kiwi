
#ifndef KIWI_RELATION_BASE_H_
#define KIWI_RELATION_BASE_H_

#include <string>
#include <list>
#include <map>
#include <set>

#include <c-arel.h>
#include "activerecord/base.h"
#include "activerecord/connection.h"
#include "activerecord/iterator.h"

namespace kiwi {
  namespace relation {
    template<typename Model>
    class Base {
      public:
        Base (const std::string& a_table_name) {
          manager_.from(a_table_name).project("*");

          table_ = a_table_name;
        }

        Base (const Base& a_source) {
          manager_ = a_source.manager_;
          table_   = a_source.table_;
        }

        ~Base () {
        }

        template<typename T>
        Base& where (const std::string& a_column, const T& a_value) {
          c_arel::Table table(table_.c_str());
          manager_.where(table[a_column].equal(a_value));
          return *this;
        }

        Base& limit (uint64_t a_limit) {
          manager_.take(a_limit);
          return *this;
        }

        Base& first (uint64_t a_limit = 1) {
          return limit(a_limit);
        }

        std::string to_sql () {
          return manager_.to_sql();
        }

        typename Model::Set execute () {
          typename Model::Set set;
          activerecord::Iterator* it = activerecord::Base::connection->execute(to_sql());

          std::map<std::string, std::string> row;
          while (it->next(row)) {
            std::map<std::string, model::Attribute> attributes;

            for (auto it = row.begin(); it != row.end(); ++it) {
              attributes.insert(std::make_pair(it->first, model::Attribute(it->second)));
            }

            set.insert(Model::instantiate(attributes));
          }

          delete it;
          return set;
        }

        protected:
        c_arel::SelectManager manager_;
        std::string table_;
    };
  }
}

#endif // KIWI_RELATION_BASE_H_

