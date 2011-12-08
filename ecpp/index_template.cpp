#include <iostream>
#include <parameters.h>
void view_index_(null)(std::ostream& out, const parameters& params) {
out << "<";
out << "html>\n  ";
out << "<";
out << "body>\n    ";
out << "<";
out << "h1>";
out << ( params["name"] );out << "<";
out << "/h1>\n    ";
out << "<";
out << "ul>\n    ";
 for (int i = 0; i < 10; i++) { out << "\n      ";
out << "<";
out << "li>";
out << ( i );out << "<";
out << "/li>\n    ";
 } out << "\n    ";
out << "<";
out << "/ul>\n  ";
out << "<";
out << "p>";
 printf("This is a random footer message"); out << "<";
out << "/p>\n  ";
out << "<";
out << "/body>\n";
out << "<";
out << "/html>\n";
}
