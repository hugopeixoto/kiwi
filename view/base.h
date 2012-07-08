
#ifndef VIEW_BASE_H_
#define VIEW_BASE_H_

#include <functional>
#include <ostream>

namespace kiwi {
namespace view {
  class Base {
  public:
    Base ();
    ~Base ();

  protected:

    bool with_output_buffer (std::function<void(void)> f, std::ostream& a_ostream);

    bool reset_buffer ();

  protected:
    std::ostream* output_buffer;
  };
}
}

#endif // PARAMETERS_H_

