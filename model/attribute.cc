
#include "model/attribute.h"

std::ostream& operator<<(std::ostream& a_o, const kiwi::model::Attribute& a_a)
{
  return a_o << a_a.to_s();
}
