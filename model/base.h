
#ifndef KIWI_MODEL_BASE_H_
#define KIWI_MODEL_BASE_H_

#include <string>
#include <map>

namespace kiwi {
  namespace model {
    template<typename Model>
    class Base {
    public:
      static Model* find (const std::string& a_id);

    protected:
      static std::map<std::string, Model*> cache_;
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
      if (a_id == "1") obj = new Model("Kiwi first post");
      if (a_id == "2") obj = new Model("Kiwi *second* post");

      return (cache_[a_id] = obj);
    }

    /**
     *Template static member instantiation 
     */
    template <class Model>
    std::map<std::string, Model*> Base<Model>::cache_;
  }
}

#endif // KIWI_MODEL_BASE_H_

