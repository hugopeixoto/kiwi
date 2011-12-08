
#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>
#include <map>

class parameter;
class parameters {
  public:

  template<typename T>
  void set (const std::string& a_name, const T&& a_value);

  template<typename T>
  void set (const std::string& a_name, const T& a_value);

  template<typename T>
  const T& get (const std::string& a_name) const;

  void clear ();

  protected:
  std::map<std::string, parameter*> params_;

  void add (const std::string& a_name, parameter* a_param);
};

class parameter {
  public:
  virtual ~parameter () {}
};

template<typename T>
class templated_parameter : public parameter
{
  public:
  templated_parameter (const T&& a_value) : value_(std::move(a_value)) { }
  templated_parameter (const T& a_value) : value_(a_value) { }

  const T& value () const { return value_; }

  protected:
  const T value_;
};

template<typename T>
void parameters::set (const std::string& a_name, const T&& a_value)
{
  add(a_name, new templated_parameter<T>(std::move(a_value)));
}

template<typename T>
void parameters::set (const std::string& a_name, const T& a_value)
{
  add(a_name, new templated_parameter<T>(a_value));
}

template<typename T>
const T& parameters::get (const std::string& a_name) const
{
  std::map<std::string, parameter*>::const_iterator it = params_.find(a_name);
  if (it == params_.end()) {
  }
  return ((const templated_parameter<T>*)it->second)->value();
}

#endif // PARAMETERS_H_

