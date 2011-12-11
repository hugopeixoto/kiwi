
#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>
#include <map>

namespace kiwi {
  namespace view {
    class Parameter;
    class Parameters {
      public:

      template<typename T>
      void set (const std::string& a_name, const T&& a_value);

      template<typename T>
      void set (const std::string& a_name, const T& a_value);

      template<typename T>
      const T& get (const std::string& a_name) const;

      void clear ();

      protected:
      std::map<std::string, Parameter*> params_;

      void add (const std::string& a_name, Parameter* a_param);
    };

    class Parameter {
      public:
      virtual ~Parameter () {}
    };

    template<typename T>
    class TemplatedParameter : public Parameter
    {
      public:
      TemplatedParameter (const T&& a_value) : value_(std::move(a_value)) { }
      TemplatedParameter (const T& a_value) : value_(a_value) { }

      const T& value () const { return value_; }

      protected:
      const T value_;
    };

    template<typename T>
    void Parameters::set (const std::string& a_name, const T&& a_value)
    {
      add(a_name, new TemplatedParameter<T>(std::move(a_value)));
    }

    template<typename T>
    void Parameters::set (const std::string& a_name, const T& a_value)
    {
      add(a_name, new TemplatedParameter<T>(a_value));
    }

    template<typename T>
    const T& Parameters::get (const std::string& a_name) const
    {
      static T default_value;

      std::map<std::string, Parameter*>::const_iterator it = params_.find(a_name);
      if (it == params_.end()) {
        return default_value;
      }
      return ((const TemplatedParameter<T>*)it->second)->value();
    }
  }
}

#endif // PARAMETERS_H_

