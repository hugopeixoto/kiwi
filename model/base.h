
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

        static Set all ();

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
      Model* obj = NULL;

      // Base<Model>::Set results = scoped().where("id", a_id).first().load<Model>();

      // if (results.size() != 1) {
      //   return NULL;
      // } else {
      //   return cache_[a_id] = *(results.begin());
      // }

      AttributeMap m1({ {   "id", Attribute(1) },
                        { "name", Attribute("Kiwi first post")},
                        { "body", Attribute("Why hello here, sir.\nThis is my first kiwi post.") } });

      AttributeMap m2({ {   "id", Attribute(2) },
                        { "name", Attribute("Kiwi second post")},
                        { "body", Attribute("What is this? I do not even.") } });

      if (a_id == 1) obj = new Model(m1);
      if (a_id == 2) obj = new Model(m2);

      return (cache_[a_id] = obj);
    }

    template<typename Model>
    typename Base<Model>::Set Base<Model>::all ()
    {
      Base<Model>::Set set;
      set.insert(find(1));
      set.insert(find(2));
      return set;
    }

    /**
     *Template static member instantiation 
     */
    template <typename Model>
    std::map<uint64_t, Model*> Base<Model>::cache_;
  }
}

#endif // KIWI_MODEL_BASE_H_

