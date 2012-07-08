
#include "view/base.h"

#include <sstream>

using kiwi::view::Base;

Base::Base ()
{
  output_buffer = new std::ostringstream();
}

Base::~Base ()
{
  delete output_buffer;
}

bool Base::with_output_buffer (std::function<void(void)> a_block, std::ostream& a_ostream)
{
  std::ostream* old = output_buffer;
  output_buffer = &a_ostream;
  a_block();
  output_buffer = old;
  return true;
}

bool Base::reset_buffer ()
{
  output_buffer->clear();
  return true;
}

