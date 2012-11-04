
#include "activerecord/postgresql/iterator.h"

using kiwi::activerecord::postgresql::Iterator;

Iterator::Iterator (void* a_result)
{
  PGresult* result = static_cast<PGresult*>(a_result);

  column_count_ = PQnfields(result);
  row_count_ = PQntuples(result);
  current_row_ = 0;

  result_ = a_result;
}

bool Iterator::next (Row& a_row)
{
  PGresult* result = static_cast<PGresult*>(result_);
  if (current_row_ >= row_count_) {
    return false;
  }

  a_row.clear();

  for (uint32_t i = 0; i < column_count_; ++i) {
    a_row[PQfname(result, i)] = PQgetvalue(result, current_row_, i);
  }

  ++current_row_;
  return true;
}

