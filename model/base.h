
#ifndef KIWI_MODEL_BASE_H_
#define KIWI_MODEL_BASE_H_

#include <string>
#include <map>
#include <set>

namespace kiwi {
  namespace model {
    template<typename Model>
    class Base {
    public:
      typedef std::set<Model*> Set;

      static Model* find (const std::string& a_id);

      static Set all ();

    protected:
      static std::map<std::string, Model*> cache_;

    public:
      static const char model_name_[];
    };

    template<typename Model>
    Model* Base<Model>::find (const std::string& a_id)
    {
      auto it = cache_.find(a_id);

      if (it != cache_.end()) {
        return it->second;
      }

      fprintf(stderr, "Cache miss (%s)\n", a_id.c_str());

      // Load the thing from the database.
      Model* obj = NULL;
      if (a_id == "1") obj = new Model(1, "Kiwi first post");
      if (a_id == "2") obj = new Model(2, "Kiwi *second* post");

      return (cache_[a_id] = obj);
    }

    template<typename Model>
    typename Base<Model>::Set Base<Model>::all ()
    {
      Base<Model>::Set set;
      set.insert(find("1"));
      set.insert(find("2"));
      return set;
    }

    /**
     *Template static member instantiation 
     */
    template <typename Model>
    std::map<std::string, Model*> Base<Model>::cache_;
  }
}

#endif // KIWI_MODEL_BASE_H_

