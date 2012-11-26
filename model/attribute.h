
#ifndef KIWI_MODEL_ATTRIBUTE_H_
#define KIWI_MODEL_ATTRIBUTE_H_

#include <string>
#include <sstream>

namespace kiwi {
  namespace model {
    class Attribute {
    public:
      Attribute ()
      {
      }

      Attribute (const std::string& a_value)
      {
        value_ = a_value;
      }

      operator const std::string&() const
      {
        return value_;
      }

      uint64_t to_i () const
      {
        uint64_t ret = 0;
        std::istringstream sin(value_);
        sin >> ret;
        return ret;
      }

      std::string to_s () const
      {
        return value_;
      }

    protected:
      std::string value_;
    };
  }
}

std::ostream& operator<<(std::ostream& a_o, const kiwi::model::Attribute& a_a);

#endif // KIWI_MODEL_ATTRIBUTE_H_

