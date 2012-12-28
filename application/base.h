
#ifndef KIWI_APPLICATION_BASE_H_
#define KIWI_APPLICATION_BASE_H_

#include <stdint.h>


namespace kiwi {
  namespace routing { class Base; }
  namespace application {
    class Base {
      public:
      /**
       * @brief Default constructor.
       * This must be invoked in order to initialize the controller engine. 
       */
      Base ();

      virtual ~Base ();

      public:
      void run (uint16_t a_port);

      protected:
      /**
       * @brief Sets the routing engine.
       */
      void set_routing (kiwi::routing::Base* a_routing);

      protected:
      kiwi::routing::Base* routing_;
    };
  }
}

#endif // KIWI_APPLICATION_BASE_H_

