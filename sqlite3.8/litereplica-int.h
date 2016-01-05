
// worker thread commands
#define REPLICA_THREAD_ADD_REPLICA     0xcd01  /*  */
#define REPLICA_THREAD_REMOVE_REPLICA  0xcd02  /*  */
#define REPLICA_THREAD_EXIT            0xcd03  /*  */
#define REPLICA_THREAD_OK              0xcd04  /*  */

#define REPLICA_RESPONSE     0xdb02    /*  */

//#define DBLOG_PGNO         0xdb33    /*  */
//#define DBLOG_OFFSET       0xdb34    /*  */
//#define DBLOG_DATA         0xdb35    /*  */
#define REPLICA_COMMIT       0xdb33    /*  */
#define REPLICA_ROLLBACK     0xdb34    /*  */
#define REPLICA_SAVEPOINT    0xdb35    /*  */
#define REPLICA_RELEASE      0xdb36    /*  */
#define REPLICA_ROLLBACK_TO  0xdb37    /*  */
//#define REPLICA_COMMAND      0xdb38    /*  */

#define REPLICA_GET_HASHES   0xdb51    /*  */
#define REPLICA_GET_PAGE     0xdb52    /*  */
#define REPLICA_GET_DB       0xdb53    /*  */
#define REPLICA_SYNC_OK      0xdb54    /*  */

#define REPLICA_STARTING     0xdb55    /*  */
#define REPLICA_CONN_DOWN    0xdb56    /*  */
#define REPLICA_PING_REQ     0xdb57    /*  */
#define REPLICA_PING_RESP    0xdb58    /*  */
#define REPLICA_CLOSING      0xdb59    /*  */

#define REPLICA_NUMPAGES     0xdb61    /*  */
#define REPLICA_PAGESIZE     0xdb62    /*  */
#define REPLICA_CHECKSUM1    0xdb63    /*  */
#define REPLICA_CHECKSUM2    0xdb64    /*  */
#define REPLICA_PAGE         0xdb65    /*  */

// response codes
#define REPLICA_INVALID      0xdb90    /*  */
#define REPLICA_OK           0xdb91    /*  */
#define REPLICA_IN_SYNC      0xdb92    /*  */

// error codes
#define REPLICA_NO_RESPONSE  0xdb93
#define REPLICA_INVALID_PEER 0xdb94
#define REPLICA_BUSY         0xdb95
#define REPLICA_ERROR        0xdb96


// the state of the slave peer or connection
#define STATE_CONN_NONE       0         /*  */
#define STATE_IDENTIFIED      1         /*  */
#define STATE_UPDATING        2         /*  */
#define STATE_IN_SYNC         3         /*  */
#define STATE_CONN_LOST       4         /*  */
#define STATE_INVALID_PEER    5         /*  */
#define STATE_BUSY            6         /*  */
#define STATE_ERROR           7         /*  */

/*
#define STATE_CONN_NONE     CONN_STATUS_DISCONNECTED
#define STATE_IDENTIFIED    CONN_STATUS_STARTING
#define STATE_UPDATING      CONN_STATUS_UPDATING
#define STATE_IN_SYNC       CONN_STATUS_IN_SYNC
#define STATE_CONN_LOST     CONN_STATUS_CONN_LOST
#define STATE_INVALID_PEER  CONN_STATUS_INVALID_PEER
#define STATE_BUSY          CONN_STATUS_BUSY
#define STATE_ERROR         CONN_STATUS_ERROR
*/

typedef struct replica replica;

struct replica {
  replica *next;         /* Next item in the list */
  int   mode;            /* replica mode [master|slave] */
  int   active;          /* This connection is being used. 0=new connection 1=active -1=disabled */
  int   conn;            /* Replica server connection mode [connect|listen] */
  char *address;         /* Address to bind or connect */
  int   sock;            /* Socket used to connect with the other peer */
  int   state;           /* The state of this connection/peer */
  //int   dblog_open;    /* If a transaction is open while in sync - used in the slave peer */
  int   master_locked;   /* If this replica locked the database */
  int   locked_timer;
  int   idle_timer;
  sqlite3 *workerdb;
  Pager   *pPager;
  /* used for the query status */
  int     db_state;
  uint64  last_conn;        /* (monotonic time) the last time a connection was made */
  uint64  last_conn_loss;   /* (monotonic time) the last time the connection was lost */
  uint64  last_update;      /* (monotonic time) the last time the db was updated */
  uint64  time_out_of_date; /* (monotonic time) the first time an update was not processed. cleared when the db is updated */
};


SQLITE_PRIVATE int  some_slave_in_sync(Pager *pPager);
SQLITE_PRIVATE void send_dblog_to_slaves(Pager *pPager, binn *list);
SQLITE_PRIVATE void update_replicas_db_state(Pager *pPager);
SQLITE_PRIVATE int  disable_pager_replicas(Pager *pPager);
