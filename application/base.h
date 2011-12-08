
#ifndef KIWI_APPLICATION_BASE_H_
#define KIWI_APPLICATION_BASE_H_

namespace kiwi { namespace routing { class Base; } }
namespace kiwi { namespace controller { class Base; } }
namespace kiwi { namespace controller { class Engine; } }

namespace kiwi {
  namespace application {
    class Base {
      public:
      /**
       * @brief Default constructor.
       * This must be invoked in order to initialize the controller engine. 
       */
      Base ();

      public:
      void run (uint16_t a_port);

      protected:
      /**
       * @brief Sets the routing engine.
       */
      void set_routing (kiwi::routing::Base* a_routing);

      /**
       * @brief Adds a controller to the application.
       */
      void add_controller (kiwi::controller::Base* a_controller);

      protected:
      kiwi::routing::Base* routing_;
      kiwi::controller::Engine* controller_engine_;
    };
  }
}

#endif // KIWI_APPLICATION_BASE_H_

