
#include "activerecord/iterator.h"

using kiwi::activerecord::Iterator;

Iterator::Iterator (PGresult* a_result) {
  result_ = a_result;
  column_count_ = PQnfields(a_result);
  row_count_ = PQntuples(a_result);
  current_row_ = 0;
}

bool Iterator::next (std::map<std::string, std::string>& a_row)
{
  if (current_row_ >= row_count_) return false;

  a_row.clear();

  for (uint32_t i = 0; i < column_count_; ++i) {
    a_row[PQfname(result_, i)] = PQgetvalue(result_, current_row_, i);
  }

  ++current_row_;
  return true;
}

