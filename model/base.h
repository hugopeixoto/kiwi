
#ifndef KIWI_MODEL_BASE_H_
#define KIWI_MODEL_BASE_H_

#include <string>
#include <list>
#include <map>
#include <set>

#include "model/attribute.h"
#include "relation/base.h"

namespace kiwi {
  namespace model {

    class Abstract {
      public:
        enum AttributeType {
          Uint64,
          String
        };

        typedef std::map<std::string, Attribute> AttributeMap;
        typedef std::list<std::string> ColumnList;

        Abstract () { }
        Abstract (const AttributeMap& a_params) {
          values_ = a_params;
        }

        Attribute get(const std::string& a_column) const {
          auto it = values_.find(a_column);

          if (it == values_.end())
            return Attribute("");
          else
            return it->second;
        }

        Attribute operator[] (const std::string& a_column)
        {
          return get(a_column);
        }

        virtual const char* model_name () const = 0;

      protected:
        AttributeMap values_;
    };

    template<typename Model>
    class Base : public Abstract {
      public:
        typedef std::set<Model*> Set;
        /**
         * Object methods
         */
        Base () {}
        Base (const AttributeMap& a_params) : Abstract(a_params) { }

        const char* model_name () const {
          return model_name_;
        }

      public:
        /**
         * AR methods.
         * They'll be living here while I figure out
         * how to structure this out.
         */
      public:
        static Model* find (const uint64_t& a_id);

        static Set all () { return relation::Base<Model>(table_name()).execute(); }

        static relation::Base<Model> scoped() { return relation::Base<Model>(table_name()); }

        static std::string table_name () { return model_name_ + std::string("s"); }

        template<typename... Arguments>
        static relation::Base<Model> where(Arguments... a_arguments) { return scoped().where(a_arguments...); }

        /* Inheritance */

        /* TODO(hpeixoto): This is where the STI check will be done */
        static Model* instantiate (const AttributeMap& a_attributes)
        {
          Model* o = new Model(a_attributes);

          return o;
        }

        /**
         *
         */
        bool update_attributes (const AttributeMap& a_attributes)
        {
          values_ = a_attributes;
          return save();
        }

        bool save ()
        {
          //if (is_new()) {
          //    return create();
          //} else {
              return update();
          //}
        }

        bool update ()
        {
        #if 0
          attributes_with_values = arel_attributes_values(false, false, attribute_names)
          return 0 if attributes_with_values.empty?
          klass = self.class
          stmt = klass.unscoped.where(klass.arel_table[klass.primary_key].eq(id)).arel.compile_update(attributes_with_values)
          klass.connection.update stmt


          std::vector<std::vector<std::string> > x;
          for (const auto& column : columns_) {

          }
          #endif

          // UpdateManager
          return true;
        }

        /**
         * Static stuff. Cache and table definitions
         */
      protected:
        static std::map<uint64_t, Model*> cache_;

      public:
        static const char model_name_[];
        static const ColumnList columns_;
    };

    template<typename Model>
    Model* Base<Model>::find (const uint64_t& a_id)
    {
      auto it = cache_.find(a_id);

      if (it != cache_.end()) {
        return it->second;
      }

      fprintf(stderr, "Cache miss (%lld)\n", a_id);

      // Load the thing from the database.
      typename Model::Set results = scoped().where("id", a_id).first().execute();

      if (results.size() != 1) {
        return NULL;
      } else {
        return cache_[a_id] = *(results.begin());
      }
    }

    /**
     *Template static member instantiation 
     */
    template <typename Model>
    std::map<uint64_t, Model*> Base<Model>::cache_;
  }
}

#endif // KIWI_MODEL_BASE_H_

