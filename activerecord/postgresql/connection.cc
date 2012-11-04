
#include "activerecord/postgresql/connection.h"
#include "activerecord/postgresql/iterator.h"

#include <libpq-fe.h>

using kiwi::activerecord::postgresql::Connection;

Connection::Connection ()
{
  connection_ = NULL;
}

bool Connection::connect (const std::string& a_db_name)
{
  std::string connection_string("dbname=" + a_db_name);
  PGconn* connection = PQconnectdb(connection_string.c_str());

  if (connection == NULL) {
    return false;
  }

  connection_ = static_cast<void*>(connection);

  return true;
}

kiwi::activerecord::Iterator* Connection::execute (const std::string& a_query)
{
  PGconn*   connection = static_cast<PGconn*>(connection_);
  PGresult* result     = PQexec(connection, a_query.c_str());

  if (result == NULL) return NULL;

  /* check status */
  ExecStatusType status = PQresultStatus(result);

  if (status == PGRES_COMMAND_OK) {
    /* successfull completion of a command returning no data */
    PQclear(result);
    return new Iterator(NULL);
  }

  if ( status == PGRES_TUPLES_OK ) {
      /* successful completion of a command returning data */
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

