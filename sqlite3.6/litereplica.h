
#ifndef _LITEREPLICA_H
#define _LITEREPLICA_H

/*
** declarations for the litereplica
*/

#include "../common/types.h"

/* replica mode */

#define REPLICA_NONE     0    /*  */
#define REPLICA_MASTER   1    /*  */
#define REPLICA_SLAVE    2    /*  */

/* socket action */

#define REPLICA_BIND     1    /*  */
#define REPLICA_LISTEN   1    /* this can be removed. there is no listening on ipc, I guess */
#define REPLICA_CONNECT  2    /*  */

/* exported functions */

int sqlite3_add_replica(sqlite3 *db, char *name, int replica, int connection, char *address);
int sqlite3_disable_replicas(sqlite3 *db, char *name);
int sqlite3_disable_replica(sqlite3 *db, char *address);
void litereplica_join();


/* replica status */

/* conn_status */

#define CONN_STATUS_DISCONNECTED  0
#define CONN_STATUS_STARTING      1
#define CONN_STATUS_UPDATING      2
#define CONN_STATUS_IN_SYNC       3
#define CONN_STATUS_CONN_LOST     4
#define CONN_STATUS_INVALID_PEER  5
#define CONN_STATUS_BUSY          6
#define CONN_STATUS_ERROR         7

/* db_state */
#define DB_STATE_UPDATED          2
#define DB_STATE_OUTOFDATE        1
#define DB_STATE_UNKNOWN          0

/* structs */

#define MAX_SLAVES   8

struct slave_status {
  char    address[256];      // it can be a path, so it must be large :/
  int     conn_status;
  int     db_state;
  uint64  last_conn;         // (seconds) the last time a connection was made
  uint64  last_update;       // (seconds) the last time the db was updated
  uint64  time_out_of_date;  // (seconds) the first time an update was not processed. cleared when the db is updated. -- or use only the one above. if the replica is outOfDate it has the time since the last update not processed.
};

struct replica_status {
  int is_master;
  int is_slave;
  //
  // status for the master side
  int num_slaves;
  //struct slave_status *slaves;          // this one requires a sqlite3_free() to be called on this item
  struct slave_status slaves[MAX_SLAVES]; // this one has a limit of items
  //
  // status for the slave side
  char   address[256];
  int    conn_status;
  uint64 db_state;
  uint64 last_conn;    // (seconds) the last time a connection was made
  uint64 last_update;  // (seconds) the last time the db was updated
};

typedef struct replica_status replica_status;

/* exported functions */

int sqlite3_replica_status(sqlite3 *db, char *name, replica_status *status);

#endif //_LITEREPLICA_H
