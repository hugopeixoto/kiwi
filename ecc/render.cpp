
#include "index_template.cpp"

int main() {
  std::map<std::string, std::string> params;
  params["name"] = "Hugo Peixoto";
  print_template_index(std::cout, params);
}
