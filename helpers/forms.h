
#ifndef KIWI_HELPERS_FORMS_H_
#define KIWI_HELPERS_FORMS_H_

#include <view/base.h>
#include <model/base.h>
#include <helpers/core.h>

namespace kiwi {
  namespace helpers {
    class Forms : virtual public view::Base {
      public:
        class FormBuilder;

      protected:
        template<typename T>
        std::string form_for (kiwi::model::Base<T>* a_object, std::function<void(FormBuilder& a_form)> a_block) {
          std::ostringstream s;
          FormBuilder form(a_object);

          s << "<form action='" << kiwi::helpers::path(a_object) << "' method='POST'>";
          with_output_buffer(std::bind<void>(a_block, form), s);
          s << "</form>";

          return s.str();
        }

      public:
        class FormBuilder {
          public:
            FormBuilder (model::Abstract* a_object) { object_ = a_object; }

            std::string input (const char* a_column) {
              return "<input type='text' name='" + h(a_column) + "' value='" + h(object_->get(a_column)) + "' />";
            }

            std::string textarea (const char* a_column) {
              return "<textarea name='" + h(a_column) + "'>" + h(object_->get(a_column)) + "</textarea>";
            }

            std::string submit (const char* a_label) {
              return "<input type='submit' name='commit' value='" + h(a_label) + "' />";
            }

          protected:
            model::Abstract* object_;
        };
    };
  }
}

#endif
