
#include <postgresql/libpq-fe.h>
#include "activerecord/connection.h"
#include "activerecord/iterator.h"

using kiwi::activerecord::Connection;

bool Connection::connect (const std::string& a_db_name)
{
  connection_ = PQconnectdb(("dbname=" + a_db_name).c_str());

  return connection_ != NULL;
}

kiwi::activerecord::Iterator* Connection::execute (const std::string& a_query)
{
  PGresult* result = PQexec(connection_, a_query.c_str());

  if (result == NULL) return NULL;

  /* check status */
  ExecStatusType status = PQresultStatus(result);

  if (status == PGRES_COMMAND_OK) {
    /* successfull completion of a command returning no data */
    PQclear(result);
    return new Iterator(NULL);
  }

  if ( status == PGRES_TUPLES_OK ) {
      /* successfull completion of a command returning data */
      return new Iterator(result);
  }

  if (status == PGRES_BAD_RESPONSE || status == PGRES_NONFATAL_ERROR || status == PGRES_FATAL_ERROR) {
      std::string error = PQresultErrorMessage(result);
      PQclear(result);
      return NULL;
  }

  /* Something else? */
  return NULL;
}

