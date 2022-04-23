#ifndef SQLITE3_H
#define SQLITE3_H
#include <stdarg.h>    

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


/*
** Provide the ability to override linkage features of the interface.
*/
#ifndef SQLITE_EXTERN
# define SQLITE_EXTERN extern
#endif
#ifndef SQLITE_API
# define SQLITE_API
#endif
#ifndef SQLITE_CDECL
# define SQLITE_CDECL
#endif
#ifndef SQLITE_APICALL
# define SQLITE_APICALL
#endif
#ifndef SQLITE_STDCALL
# define SQLITE_STDCALL SQLITE_APICALL
#endif
#ifndef SQLITE_CALLBACK
# define SQLITE_CALLBACK
#endif
#ifndef SQLITE_SYSAPI
# define SQLITE_SYSAPI
#endif

#define SQLITE_DEPRECATED
#define SQLITE_EXPERIMENTAL

#ifdef SQLITE_VERSION
# undef SQLITE_VERSION
#endif
#ifdef SQLITE_VERSION_NUMBER
# undef SQLITE_VERSION_NUMBER
#endif

#define SQLITE_VERSION        "3.36.0"
#define SQLITE_VERSION_NUMBER 3036000
#define SQLITE_SOURCE_ID      "2021-06-18 18:36:39 5c9a6c06871cb9fe42814af9c039eb6da5427a6ec28f187af7ebfb62eafa66e5"

SQLITE_API SQLITE_EXTERN const char sqlite3_version[];
SQLITE_API const char *sqlite3_libversion(void);
SQLITE_API const char *sqlite3_sourceid(void);
SQLITE_API int sqlite3_libversion_number(void);

#ifndef SQLITE_OMIT_COMPILEOPTION_DIAGS
SQLITE_API int sqlite3_compileoption_used(const char *zOptName);
SQLITE_API const char *sqlite3_compileoption_get(int N);
#else
# define sqlite3_compileoption_used(X) 0
# define sqlite3_compileoption_get(X)  ((void*)0)
#endif

SQLITE_API int sqlite3_threadsafe(void);


typedef struct sqlite3 sqlite3;

#ifdef SQLITE_INT64_TYPE
  typedef SQLITE_INT64_TYPE sqlite_int64;
# ifdef SQLITE_UINT64_TYPE
    typedef SQLITE_UINT64_TYPE sqlite_uint64;
# else
    typedef unsigned SQLITE_INT64_TYPE sqlite_uint64;
# endif
#elif defined(_MSC_VER) || defined(__BORLANDC__)
  typedef __int64 sqlite_int64;
  typedef unsigned __int64 sqlite_uint64;
#else
  typedef long long int sqlite_int64;
  typedef unsigned long long int sqlite_uint64;
#endif
typedef sqlite_int64 sqlite3_int64;
typedef sqlite_uint64 sqlite3_uint64;

/*
** If compiling for a processor that lacks floating point support,
** substitute integer for floating-point.
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# define double sqlite3_int64
#endif


SQLITE_API int sqlite3_close(sqlite3*);
SQLITE_API int sqlite3_close_v2(sqlite3*);


typedef int (*sqlite3_callback)(void*,int,char**, char**);

/* 执行语句 */
SQLITE_API int sqlite3_exec(
  sqlite3*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
);

/* 状态码 */
#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Internal use only */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Not used */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
/* end-of-error-codes */

/*
** CAPI3REF: Extended Result Codes
** KEYWORDS: {extended result code definitions}
*/
#define SQLITE_ERROR_MISSING_COLLSEQ   (SQLITE_ERROR | (1<<8))
#define SQLITE_ERROR_RETRY             (SQLITE_ERROR | (2<<8))
#define SQLITE_ERROR_SNAPSHOT          (SQLITE_ERROR | (3<<8))
#define SQLITE_IOERR_READ              (SQLITE_IOERR | (1<<8))
#define SQLITE_IOERR_SHORT_READ        (SQLITE_IOERR | (2<<8))
#define SQLITE_IOERR_WRITE             (SQLITE_IOERR | (3<<8))
#define SQLITE_IOERR_FSYNC             (SQLITE_IOERR | (4<<8))
#define SQLITE_IOERR_DIR_FSYNC         (SQLITE_IOERR | (5<<8))
#define SQLITE_IOERR_TRUNCATE          (SQLITE_IOERR | (6<<8))
#define SQLITE_IOERR_FSTAT             (SQLITE_IOERR | (7<<8))
#define SQLITE_IOERR_UNLOCK            (SQLITE_IOERR | (8<<8))
#define SQLITE_IOERR_RDLOCK            (SQLITE_IOERR | (9<<8))
#define SQLITE_IOERR_DELETE            (SQLITE_IOERR | (10<<8))
#define SQLITE_IOERR_BLOCKED           (SQLITE_IOERR | (11<<8))
#define SQLITE_IOERR_NOMEM             (SQLITE_IOERR | (12<<8))
#define SQLITE_IOERR_ACCESS            (SQLITE_IOERR | (13<<8))
#define SQLITE_IOERR_CHECKRESERVEDLOCK (SQLITE_IOERR | (14<<8))
#define SQLITE_IOERR_LOCK              (SQLITE_IOERR | (15<<8))
#define SQLITE_IOERR_CLOSE             (SQLITE_IOERR | (16<<8))
#define SQLITE_IOERR_DIR_CLOSE         (SQLITE_IOERR | (17<<8))
#define SQLITE_IOERR_SHMOPEN           (SQLITE_IOERR | (18<<8))
#define SQLITE_IOERR_SHMSIZE           (SQLITE_IOERR | (19<<8))
#define SQLITE_IOERR_SHMLOCK           (SQLITE_IOERR | (20<<8))
#define SQLITE_IOERR_SHMMAP            (SQLITE_IOERR | (21<<8))
#define SQLITE_IOERR_SEEK              (SQLITE_IOERR | (22<<8))
#define SQLITE_IOERR_DELETE_NOENT      (SQLITE_IOERR | (23<<8))
#define SQLITE_IOERR_MMAP              (SQLITE_IOERR | (24<<8))
#define SQLITE_IOERR_GETTEMPPATH       (SQLITE_IOERR | (25<<8))
#define SQLITE_IOERR_CONVPATH          (SQLITE_IOERR | (26<<8))
#define SQLITE_IOERR_VNODE             (SQLITE_IOERR | (27<<8))
#define SQLITE_IOERR_AUTH              (SQLITE_IOERR | (28<<8))
#define SQLITE_IOERR_BEGIN_ATOMIC      (SQLITE_IOERR | (29<<8))
#define SQLITE_IOERR_COMMIT_ATOMIC     (SQLITE_IOERR | (30<<8))
#define SQLITE_IOERR_ROLLBACK_ATOMIC   (SQLITE_IOERR | (31<<8))
#define SQLITE_IOERR_DATA              (SQLITE_IOERR | (32<<8))
#define SQLITE_IOERR_CORRUPTFS         (SQLITE_IOERR | (33<<8))
#define SQLITE_LOCKED_SHAREDCACHE      (SQLITE_LOCKED |  (1<<8))
#define SQLITE_LOCKED_VTAB             (SQLITE_LOCKED |  (2<<8))
#define SQLITE_BUSY_RECOVERY           (SQLITE_BUSY   |  (1<<8))
#define SQLITE_BUSY_SNAPSHOT           (SQLITE_BUSY   |  (2<<8))
#define SQLITE_BUSY_TIMEOUT            (SQLITE_BUSY   |  (3<<8))
#define SQLITE_CANTOPEN_NOTEMPDIR      (SQLITE_CANTOPEN | (1<<8))
#define SQLITE_CANTOPEN_ISDIR          (SQLITE_CANTOPEN | (2<<8))
#define SQLITE_CANTOPEN_FULLPATH       (SQLITE_CANTOPEN | (3<<8))
#define SQLITE_CANTOPEN_CONVPATH       (SQLITE_CANTOPEN | (4<<8))
#define SQLITE_CANTOPEN_DIRTYWAL       (SQLITE_CANTOPEN | (5<<8)) /* Not Used */
#define SQLITE_CANTOPEN_SYMLINK        (SQLITE_CANTOPEN | (6<<8))
#define SQLITE_CORRUPT_VTAB            (SQLITE_CORRUPT | (1<<8))
#define SQLITE_CORRUPT_SEQUENCE        (SQLITE_CORRUPT | (2<<8))
#define SQLITE_CORRUPT_INDEX           (SQLITE_CORRUPT | (3<<8))
#define SQLITE_READONLY_RECOVERY       (SQLITE_READONLY | (1<<8))
#define SQLITE_READONLY_CANTLOCK       (SQLITE_READONLY | (2<<8))
#define SQLITE_READONLY_ROLLBACK       (SQLITE_READONLY | (3<<8))
#define SQLITE_READONLY_DBMOVED        (SQLITE_READONLY | (4<<8))
#define SQLITE_READONLY_CANTINIT       (SQLITE_READONLY | (5<<8))
#define SQLITE_READONLY_DIRECTORY      (SQLITE_READONLY | (6<<8))
#define SQLITE_ABORT_ROLLBACK          (SQLITE_ABORT | (2<<8))
#define SQLITE_CONSTRAINT_CHECK        (SQLITE_CONSTRAINT | (1<<8))
#define SQLITE_CONSTRAINT_COMMITHOOK   (SQLITE_CONSTRAINT | (2<<8))
#define SQLITE_CONSTRAINT_FOREIGNKEY   (SQLITE_CONSTRAINT | (3<<8))
#define SQLITE_CONSTRAINT_FUNCTION     (SQLITE_CONSTRAINT | (4<<8))
#define SQLITE_CONSTRAINT_NOTNULL      (SQLITE_CONSTRAINT | (5<<8))
#define SQLITE_CONSTRAINT_PRIMARYKEY   (SQLITE_CONSTRAINT | (6<<8))
#define SQLITE_CONSTRAINT_TRIGGER      (SQLITE_CONSTRAINT | (7<<8))
#define SQLITE_CONSTRAINT_UNIQUE       (SQLITE_CONSTRAINT | (8<<8))
#define SQLITE_CONSTRAINT_VTAB         (SQLITE_CONSTRAINT | (9<<8))
#define SQLITE_CONSTRAINT_ROWID        (SQLITE_CONSTRAINT |(10<<8))
#define SQLITE_CONSTRAINT_PINNED       (SQLITE_CONSTRAINT |(11<<8))
#define SQLITE_NOTICE_RECOVER_WAL      (SQLITE_NOTICE | (1<<8))
#define SQLITE_NOTICE_RECOVER_ROLLBACK (SQLITE_NOTICE | (2<<8))
#define SQLITE_WARNING_AUTOINDEX       (SQLITE_WARNING | (1<<8))
#define SQLITE_AUTH_USER               (SQLITE_AUTH | (1<<8))
#define SQLITE_OK_LOAD_PERMANENTLY     (SQLITE_OK | (1<<8))
#define SQLITE_OK_SYMLINK              (SQLITE_OK | (2<<8))

/*
** CAPI3REF: Flags For File Open Operations
**
** These bit values are intended for use in the
** 3rd parameter to the [sqlite3_open_v2()] interface and
** in the 4th parameter to the [sqlite3_vfs.xOpen] method.
*/
#define SQLITE_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define SQLITE_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define SQLITE_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define SQLITE_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MEMORY           0x00000080  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define SQLITE_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define SQLITE_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define SQLITE_OPEN_SUPER_JOURNAL    0x00004000  /* VFS only */
#define SQLITE_OPEN_NOMUTEX          0x00008000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_SHAREDCACHE      0x00020000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_PRIVATECACHE     0x00040000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_WAL              0x00080000  /* VFS only */
#define SQLITE_OPEN_NOFOLLOW         0x01000000  /* Ok for sqlite3_open_v2() */

/* Reserved:                         0x00F00000 */
/* Legacy compatibility: */
#define SQLITE_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */

#define SQLITE_IOCAP_ATOMIC                 0x00000001
#define SQLITE_IOCAP_ATOMIC512              0x00000002
#define SQLITE_IOCAP_ATOMIC1K               0x00000004
#define SQLITE_IOCAP_ATOMIC2K               0x00000008
#define SQLITE_IOCAP_ATOMIC4K               0x00000010
#define SQLITE_IOCAP_ATOMIC8K               0x00000020
#define SQLITE_IOCAP_ATOMIC16K              0x00000040
#define SQLITE_IOCAP_ATOMIC32K              0x00000080
#define SQLITE_IOCAP_ATOMIC64K              0x00000100
#define SQLITE_IOCAP_SAFE_APPEND            0x00000200
#define SQLITE_IOCAP_SEQUENTIAL             0x00000400
#define SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN  0x00000800
#define SQLITE_IOCAP_POWERSAFE_OVERWRITE    0x00001000
#define SQLITE_IOCAP_IMMUTABLE              0x00002000
#define SQLITE_IOCAP_BATCH_ATOMIC           0x00004000

/*
** CAPI3REF: File Locking Levels
**
** SQLite uses one of these integer values as the second
** argument to calls it makes to the xLock() and xUnlock() methods
** of an [sqlite3_io_methods] object.
*/
#define SQLITE_LOCK_NONE          0
#define SQLITE_LOCK_SHARED        1
#define SQLITE_LOCK_RESERVED      2
#define SQLITE_LOCK_PENDING       3
#define SQLITE_LOCK_EXCLUSIVE     4

#define SQLITE_SYNC_NORMAL        0x00002
#define SQLITE_SYNC_FULL          0x00003
#define SQLITE_SYNC_DATAONLY      0x00010

/*
** CAPI3REF: OS Interface Open File Handle
*/
typedef struct sqlite3_file sqlite3_file;
struct sqlite3_file {
  const struct sqlite3_io_methods *pMethods;  /* Methods for an open file */
};

/*
** CAPI3REF: OS Interface File Virtual Methods Object
*/
typedef struct sqlite3_io_methods sqlite3_io_methods;
struct sqlite3_io_methods {
  int iVersion;
  int (*xClose)(sqlite3_file*);
  int (*xRead)(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
  int (*xWrite)(sqlite3_file*, const void*, int iAmt, sqlite3_int64 iOfst);
  int (*xTruncate)(sqlite3_file*, sqlite3_int64 size);
  int (*xSync)(sqlite3_file*, int flags);
  int (*xFileSize)(sqlite3_file*, sqlite3_int64 *pSize);
  int (*xLock)(sqlite3_file*, int);
  int (*xUnlock)(sqlite3_file*, int);
  int (*xCheckReservedLock)(sqlite3_file*, int *pResOut);
  int (*xFileControl)(sqlite3_file*, int op, void *pArg);
  int (*xSectorSize)(sqlite3_file*);
  int (*xDeviceCharacteristics)(sqlite3_file*);
  /* Methods above are valid for version 1 */
  int (*xShmMap)(sqlite3_file*, int iPg, int pgsz, int, void volatile**);
  int (*xShmLock)(sqlite3_file*, int offset, int n, int flags);
  void (*xShmBarrier)(sqlite3_file*);
  int (*xShmUnmap)(sqlite3_file*, int deleteFlag);
  /* Methods above are valid for version 2 */
  int (*xFetch)(sqlite3_file*, sqlite3_int64 iOfst, int iAmt, void **pp);
  int (*xUnfetch)(sqlite3_file*, sqlite3_int64 iOfst, void *p);
  /* Methods above are valid for version 3 */
  /* Additional methods may be added in future releases */
};

/*
** CAPI3REF: Standard File Control Opcodes
** KEYWORDS: {file control opcodes} {file control opcode}
*/
#define SQLITE_FCNTL_LOCKSTATE               1
#define SQLITE_FCNTL_GET_LOCKPROXYFILE       2
#define SQLITE_FCNTL_SET_LOCKPROXYFILE       3
#define SQLITE_FCNTL_LAST_ERRNO              4
#define SQLITE_FCNTL_SIZE_HINT               5
#define SQLITE_FCNTL_CHUNK_SIZE              6
#define SQLITE_FCNTL_FILE_POINTER            7
#define SQLITE_FCNTL_SYNC_OMITTED            8
#define SQLITE_FCNTL_WIN32_AV_RETRY          9
#define SQLITE_FCNTL_PERSIST_WAL            10
#define SQLITE_FCNTL_OVERWRITE              11
#define SQLITE_FCNTL_VFSNAME                12
#define SQLITE_FCNTL_POWERSAFE_OVERWRITE    13
#define SQLITE_FCNTL_PRAGMA                 14
#define SQLITE_FCNTL_BUSYHANDLER            15
#define SQLITE_FCNTL_TEMPFILENAME           16
#define SQLITE_FCNTL_MMAP_SIZE              18
#define SQLITE_FCNTL_TRACE                  19
#define SQLITE_FCNTL_HAS_MOVED              20
#define SQLITE_FCNTL_SYNC                   21
#define SQLITE_FCNTL_COMMIT_PHASETWO        22
#define SQLITE_FCNTL_WIN32_SET_HANDLE       23
#define SQLITE_FCNTL_WAL_BLOCK              24
#define SQLITE_FCNTL_ZIPVFS                 25
#define SQLITE_FCNTL_RBU                    26
#define SQLITE_FCNTL_VFS_POINTER            27
#define SQLITE_FCNTL_JOURNAL_POINTER        28
#define SQLITE_FCNTL_WIN32_GET_HANDLE       29
#define SQLITE_FCNTL_PDB                    30
#define SQLITE_FCNTL_BEGIN_ATOMIC_WRITE     31
#define SQLITE_FCNTL_COMMIT_ATOMIC_WRITE    32
#define SQLITE_FCNTL_ROLLBACK_ATOMIC_WRITE  33
#define SQLITE_FCNTL_LOCK_TIMEOUT           34
#define SQLITE_FCNTL_DATA_VERSION           35
#define SQLITE_FCNTL_SIZE_LIMIT             36
#define SQLITE_FCNTL_CKPT_DONE              37
#define SQLITE_FCNTL_RESERVE_BYTES          38
#define SQLITE_FCNTL_CKPT_START             39
#define SQLITE_FCNTL_EXTERNAL_READER        40
#define SQLITE_FCNTL_CKSM_FILE              41

/* deprecated names */
#define SQLITE_GET_LOCKPROXYFILE      SQLITE_FCNTL_GET_LOCKPROXYFILE
#define SQLITE_SET_LOCKPROXYFILE      SQLITE_FCNTL_SET_LOCKPROXYFILE
#define SQLITE_LAST_ERRNO             SQLITE_FCNTL_LAST_ERRNO


/*
** CAPI3REF: Mutex Handle
*/
typedef struct sqlite3_mutex sqlite3_mutex;

/*
** CAPI3REF: Loadable Extension Thunk
*/
typedef struct sqlite3_api_routines sqlite3_api_routines;

/*
** CAPI3REF: OS Interface Object
*/
typedef struct sqlite3_vfs sqlite3_vfs;
typedef void (*sqlite3_syscall_ptr)(void);
struct sqlite3_vfs {
  int iVersion;            /* Structure version number (currently 3) */
  int szOsFile;            /* Size of subclassed sqlite3_file */
  int mxPathname;          /* Maximum file pathname length */
  sqlite3_vfs *pNext;      /* Next registered VFS */
  const char *zName;       /* Name of this virtual file system */
  void *pAppData;          /* Pointer to application-specific data */
  int (*xOpen)(sqlite3_vfs*, const char *zName, sqlite3_file*,
               int flags, int *pOutFlags);
  int (*xDelete)(sqlite3_vfs*, const char *zName, int syncDir);
  int (*xAccess)(sqlite3_vfs*, const char *zName, int flags, int *pResOut);
  int (*xFullPathname)(sqlite3_vfs*, const char *zName, int nOut, char *zOut);
  void *(*xDlOpen)(sqlite3_vfs*, const char *zFilename);
  void (*xDlError)(sqlite3_vfs*, int nByte, char *zErrMsg);
  void (*(*xDlSym)(sqlite3_vfs*,void*, const char *zSymbol))(void);
  void (*xDlClose)(sqlite3_vfs*, void*);
  int (*xRandomness)(sqlite3_vfs*, int nByte, char *zOut);
  int (*xSleep)(sqlite3_vfs*, int microseconds);
  int (*xCurrentTime)(sqlite3_vfs*, double*);
  int (*xGetLastError)(sqlite3_vfs*, int, char *);
  /*
  ** The methods above are in version 1 of the sqlite_vfs object
  ** definition.  Those that follow are added in version 2 or later
  */
  int (*xCurrentTimeInt64)(sqlite3_vfs*, sqlite3_int64*);
  /*
  ** The methods above are in versions 1 and 2 of the sqlite_vfs object.
  ** Those below are for version 3 and greater.
  */
  int (*xSetSystemCall)(sqlite3_vfs*, const char *zName, sqlite3_syscall_ptr);
  sqlite3_syscall_ptr (*xGetSystemCall)(sqlite3_vfs*, const char *zName);
  const char *(*xNextSystemCall)(sqlite3_vfs*, const char *zName);
  /*
  ** The methods above are in versions 1 through 3 of the sqlite_vfs object.
  ** New fields may be appended in future versions.  The iVersion
  ** value will increment whenever this happens.
  */
};

/*
** CAPI3REF: Flags for the xAccess VFS method
*/
#define SQLITE_ACCESS_EXISTS    0
#define SQLITE_ACCESS_READWRITE 1   /* Used by PRAGMA temp_store_directory */
#define SQLITE_ACCESS_READ      2   /* Unused */

/*
** CAPI3REF: Flags for the xShmLock VFS method
*/
#define SQLITE_SHM_UNLOCK       1
#define SQLITE_SHM_LOCK         2
#define SQLITE_SHM_SHARED       4
#define SQLITE_SHM_EXCLUSIVE    8

/*
** CAPI3REF: Maximum xShmLock index
*/
#define SQLITE_SHM_NLOCK        8


/*
** CAPI3REF: Initialize The SQLite Library
*/
SQLITE_API int sqlite3_initialize(void);
SQLITE_API int sqlite3_shutdown(void);
SQLITE_API int sqlite3_os_init(void);
SQLITE_API int sqlite3_os_end(void);

/*
** CAPI3REF: Configuring The SQLite Library
*/
SQLITE_API int sqlite3_config(int, ...);

/*
** CAPI3REF: Configure database connections
** METHOD: sqlite3.
*/
SQLITE_API int sqlite3_db_config(sqlite3*, int op, ...);

/*
** CAPI3REF: Memory Allocation Routines
*/
typedef struct sqlite3_mem_methods sqlite3_mem_methods;
struct sqlite3_mem_methods {
  void *(*xMalloc)(int);         /* Memory allocation function */
  void (*xFree)(void*);          /* Free a prior allocation */
  void *(*xRealloc)(void*,int);  /* Resize an allocation */
  int (*xSize)(void*);           /* Return the size of an allocation */
  int (*xRoundup)(int);          /* Round up request size to allocation size */
  int (*xInit)(void*);           /* Initialize the memory allocator */
  void (*xShutdown)(void*);      /* Deinitialize the memory allocator */
  void *pAppData;                /* Argument to xInit() and xShutdown() */
};

/*
** CAPI3REF: Configuration Options
** KEYWORDS: {configuration option}
*/
#define SQLITE_CONFIG_SINGLETHREAD  1  /* nil */
#define SQLITE_CONFIG_MULTITHREAD   2  /* nil */
#define SQLITE_CONFIG_SERIALIZED    3  /* nil */
#define SQLITE_CONFIG_MALLOC        4  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_GETMALLOC     5  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_SCRATCH       6  /* No longer used */
#define SQLITE_CONFIG_PAGECACHE     7  /* void*, int sz, int N */
#define SQLITE_CONFIG_HEAP          8  /* void*, int nByte, int min */
#define SQLITE_CONFIG_MEMSTATUS     9  /* boolean */
#define SQLITE_CONFIG_MUTEX        10  /* sqlite3_mutex_methods* */
#define SQLITE_CONFIG_GETMUTEX     11  /* sqlite3_mutex_methods* */
/* previously SQLITE_CONFIG_CHUNKALLOC 12 which is now unused. */
#define SQLITE_CONFIG_LOOKASIDE    13  /* int int */
#define SQLITE_CONFIG_PCACHE       14  /* no-op */
#define SQLITE_CONFIG_GETPCACHE    15  /* no-op */
#define SQLITE_CONFIG_LOG          16  /* xFunc, void* */
#define SQLITE_CONFIG_URI          17  /* int */
#define SQLITE_CONFIG_PCACHE2      18  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_GETPCACHE2   19  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_COVERING_INDEX_SCAN 20  /* int */
#define SQLITE_CONFIG_SQLLOG       21  /* xSqllog, void* */
#define SQLITE_CONFIG_MMAP_SIZE    22  /* sqlite3_int64, sqlite3_int64 */
#define SQLITE_CONFIG_WIN32_HEAPSIZE      23  /* int nByte */
#define SQLITE_CONFIG_PCACHE_HDRSZ        24  /* int *psz */
#define SQLITE_CONFIG_PMASZ               25  /* unsigned int szPma */
#define SQLITE_CONFIG_STMTJRNL_SPILL      26  /* int nByte */
#define SQLITE_CONFIG_SMALL_MALLOC        27  /* boolean */
#define SQLITE_CONFIG_SORTERREF_SIZE      28  /* int nByte */
#define SQLITE_CONFIG_MEMDB_MAXSIZE       29  /* sqlite3_int64 */

/*
** CAPI3REF: Database Connection Configuration Options
*/
#define SQLITE_DBCONFIG_MAINDBNAME            1000 /* const char* */
#define SQLITE_DBCONFIG_LOOKASIDE             1001 /* void* int int */
#define SQLITE_DBCONFIG_ENABLE_FKEY           1002 /* int int* */
#define SQLITE_DBCONFIG_ENABLE_TRIGGER        1003 /* int int* */
#define SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER 1004 /* int int* */
#define SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION 1005 /* int int* */
#define SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE      1006 /* int int* */
#define SQLITE_DBCONFIG_ENABLE_QPSG           1007 /* int int* */
#define SQLITE_DBCONFIG_TRIGGER_EQP           1008 /* int int* */
#define SQLITE_DBCONFIG_RESET_DATABASE        1009 /* int int* */
#define SQLITE_DBCONFIG_DEFENSIVE             1010 /* int int* */
#define SQLITE_DBCONFIG_WRITABLE_SCHEMA       1011 /* int int* */
#define SQLITE_DBCONFIG_LEGACY_ALTER_TABLE    1012 /* int int* */
#define SQLITE_DBCONFIG_DQS_DML               1013 /* int int* */
#define SQLITE_DBCONFIG_DQS_DDL               1014 /* int int* */
#define SQLITE_DBCONFIG_ENABLE_VIEW           1015 /* int int* */
#define SQLITE_DBCONFIG_LEGACY_FILE_FORMAT    1016 /* int int* */
#define SQLITE_DBCONFIG_TRUSTED_SCHEMA        1017 /* int int* */
#define SQLITE_DBCONFIG_MAX                   1017 /* Largest DBCONFIG */

/*
** CAPI3REF: Enable Or Disable Extended Result Codes
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_extended_result_codes(sqlite3*, int onoff);

/*
** CAPI3REF: Last Insert Rowid
** METHOD: sqlite3
*/
SQLITE_API sqlite3_int64 sqlite3_last_insert_rowid(sqlite3*);

/*
** CAPI3REF: Set the Last Insert Rowid value.
** METHOD: sqlite3
*/
SQLITE_API void sqlite3_set_last_insert_rowid(sqlite3*,sqlite3_int64);

/*
** CAPI3REF: Count The Number Of Rows Modified
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_changes(sqlite3*);

/*
** CAPI3REF: Total Number Of Rows Modified
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_total_changes(sqlite3*);

/*
** CAPI3REF: Interrupt A Long-Running Query
** METHOD: sqlite3
*/
SQLITE_API void sqlite3_interrupt(sqlite3*);

/*
** CAPI3REF: Determine If An SQL Statement Is Complete
*/
SQLITE_API int sqlite3_complete(const char *sql);
SQLITE_API int sqlite3_complete16(const void *sql);

/*
** CAPI3REF: Register A Callback To Handle SQLITE_BUSY Errors
** KEYWORDS: {busy-handler callback} {busy handler}
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_busy_handler(sqlite3*,int(*)(void*,int),void*);

/*
** CAPI3REF: Set A Busy Timeout
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_busy_timeout(sqlite3*, int ms);

/*
** CAPI3REF: Convenience Routines For Running Queries
** METHOD: sqlite3
*/
/*获取表中的数据*/
SQLITE_API int sqlite3_get_table(
  sqlite3 *db,          /* An open database */
  const char *zSql,     /* SQL to be evaluated */
  char ***pazResult,    /* Results of the query */
  int *pnRow,           /* Number of result rows written here */
  int *pnColumn,        /* Number of result columns written here */
  char **pzErrmsg       /* Error msg written here */
);

SQLITE_API void sqlite3_free_table(char **result);

/*
** CAPI3REF: Formatted String Printing Functions
*/
SQLITE_API char *sqlite3_mprintf(const char*,...);
SQLITE_API char *sqlite3_vmprintf(const char*, va_list);
SQLITE_API char *sqlite3_snprintf(int,char*,const char*, ...);
SQLITE_API char *sqlite3_vsnprintf(int,char*,const char*, va_list);

/*
** CAPI3REF: Memory Allocation Subsystem
*/
/*申请内存 空间*/
SQLITE_API void *sqlite3_malloc(int);
SQLITE_API void *sqlite3_malloc64(sqlite3_uint64);
SQLITE_API void *sqlite3_realloc(void*, int);
SQLITE_API void *sqlite3_realloc64(void*, sqlite3_uint64);
SQLITE_API void sqlite3_free(void*);
SQLITE_API sqlite3_uint64 sqlite3_msize(void*);

/*
** CAPI3REF: Memory Allocator Statistics
*/
SQLITE_API sqlite3_int64 sqlite3_memory_used(void);
SQLITE_API sqlite3_int64 sqlite3_memory_highwater(int resetFlag);

/*
** CAPI3REF: Pseudo-Random Number Generator
*/
SQLITE_API void sqlite3_randomness(int N, void *P);

/*
** CAPI3REF: Compile-Time Authorization Callbacks
** METHOD: sqlite3
** KEYWORDS: {authorizer callback
*/
SQLITE_API int sqlite3_set_authorizer(
  sqlite3*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);

/*
** CAPI3REF: Authorizer Return Codes
*/
#define SQLITE_DENY   1   /* Abort the SQL statement with an error */
#define SQLITE_IGNORE 2   /* Don't allow access, but don't generate an error */

/*
** CAPI3REF: Authorizer Action Codes
*/
/******************************************* 3rd ************ 4th ***********/
#define SQLITE_CREATE_INDEX          1   /* Index Name      Table Name      */
#define SQLITE_CREATE_TABLE          2   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
#define SQLITE_CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
#define SQLITE_CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_TEMP_VIEW      6   /* View Name       NULL            */
#define SQLITE_CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
#define SQLITE_CREATE_VIEW           8   /* View Name       NULL            */
#define SQLITE_DELETE                9   /* Table Name      NULL            */
#define SQLITE_DROP_INDEX           10   /* Index Name      Table Name      */
#define SQLITE_DROP_TABLE           11   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
#define SQLITE_DROP_TEMP_TABLE      13   /* Table Name      NULL            */
#define SQLITE_DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
#define SQLITE_DROP_TEMP_VIEW       15   /* View Name       NULL            */
#define SQLITE_DROP_TRIGGER         16   /* Trigger Name    Table Name      */
#define SQLITE_DROP_VIEW            17   /* View Name       NULL            */
#define SQLITE_INSERT               18   /* Table Name      NULL            */
#define SQLITE_PRAGMA               19   /* Pragma Name     1st arg or NULL */
#define SQLITE_READ                 20   /* Table Name      Column Name     */
#define SQLITE_SELECT               21   /* NULL            NULL            */
#define SQLITE_TRANSACTION          22   /* Operation       NULL            */
#define SQLITE_UPDATE               23   /* Table Name      Column Name     */
#define SQLITE_ATTACH               24   /* Filename        NULL            */
#define SQLITE_DETACH               25   /* Database Name   NULL            */
#define SQLITE_ALTER_TABLE          26   /* Database Name   Table Name      */
#define SQLITE_REINDEX              27   /* Index Name      NULL            */
#define SQLITE_ANALYZE              28   /* Table Name      NULL            */
#define SQLITE_CREATE_VTABLE        29   /* Table Name      Module Name     */
#define SQLITE_DROP_VTABLE          30   /* Table Name      Module Name     */
#define SQLITE_FUNCTION             31   /* NULL            Function Name   */
#define SQLITE_SAVEPOINT            32   /* Operation       Savepoint Name  */
#define SQLITE_COPY                  0   /* No longer used */
#define SQLITE_RECURSIVE            33   /* NULL            NULL            */

/*
** CAPI3REF: Tracing And Profiling Functions
** METHOD: sqlite3
*/
SQLITE_API SQLITE_DEPRECATED void *sqlite3_trace(sqlite3*,
   void(*xTrace)(void*,const char*), void*);
SQLITE_API SQLITE_DEPRECATED void *sqlite3_profile(sqlite3*,
   void(*xProfile)(void*,const char*,sqlite3_uint64), void*);

/*
** CAPI3REF: SQL Trace Event Codes
** KEYWORDS: SQLITE_TRACE
*/
#define SQLITE_TRACE_STMT       0x01
#define SQLITE_TRACE_PROFILE    0x02
#define SQLITE_TRACE_ROW        0x04
#define SQLITE_TRACE_CLOSE      0x08

/*
** CAPI3REF: SQL Trace Hook
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_trace_v2(
  sqlite3*,
  unsigned uMask,
  int(*xCallback)(unsigned,void*,void*,void*),
  void *pCtx
);

/*
** CAPI3REF: Query Progress Callbacks
** METHOD: sqlite3
*/
SQLITE_API void sqlite3_progress_handler(sqlite3*, int, int(*)(void*), void*);

/*
** CAPI3REF: Opening A New Database Connection
** CONSTRUCTOR: sqlite3
*/
/*数据库打开 API*/
SQLITE_API int sqlite3_open(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);
SQLITE_API int sqlite3_open16(
  const void *filename,   /* Database filename (UTF-16) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);
SQLITE_API int sqlite3_open_v2(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb,         /* OUT: SQLite db handle */
  int flags,              /* Flags */
  const char *zVfs        /* Name of VFS module to use */
);

/*
** CAPI3REF: Obtain Values For URI Parameters
*/
SQLITE_API const char *sqlite3_uri_parameter(const char *zFilename, const char *zParam);
SQLITE_API int sqlite3_uri_boolean(const char *zFile, const char *zParam, int bDefault);
SQLITE_API sqlite3_int64 sqlite3_uri_int64(const char*, const char*, sqlite3_int64);
SQLITE_API const char *sqlite3_uri_key(const char *zFilename, int N);

/*
** CAPI3REF:  Translate filenames
*/
SQLITE_API const char *sqlite3_filename_database(const char*);
SQLITE_API const char *sqlite3_filename_journal(const char*);
SQLITE_API const char *sqlite3_filename_wal(const char*);

/*
** CAPI3REF:  Database File Corresponding To A Journal
*/
SQLITE_API sqlite3_file *sqlite3_database_file_object(const char*);

/*
** CAPI3REF: Create and Destroy VFS Filenames
*/
SQLITE_API char *sqlite3_create_filename(
  const char *zDatabase,
  const char *zJournal,
  const char *zWal,
  int nParam,
  const char **azParam
);
SQLITE_API void sqlite3_free_filename(char*);

/*
** CAPI3REF: Error Codes And Messages
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_errcode(sqlite3 *db);
SQLITE_API int sqlite3_extended_errcode(sqlite3 *db);
SQLITE_API const char *sqlite3_errmsg(sqlite3*);
SQLITE_API const void *sqlite3_errmsg16(sqlite3*);
SQLITE_API const char *sqlite3_errstr(int);

/*
** CAPI3REF: Prepared Statement Object
** KEYWORDS: {prepared statement} {prepared statements}
*/
typedef struct sqlite3_stmt sqlite3_stmt;

/*
** CAPI3REF: Run-time Limits
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_limit(sqlite3*, int id, int newVal);

/*
** CAPI3REF: Run-Time Limit Categories
** KEYWORDS: {limit category} {*limit categories}
*/
#define SQLITE_LIMIT_LENGTH                    0
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_LIMIT_COLUMN                    2
#define SQLITE_LIMIT_EXPR_DEPTH                3
#define SQLITE_LIMIT_COMPOUND_SELECT           4
#define SQLITE_LIMIT_VDBE_OP                   5
#define SQLITE_LIMIT_FUNCTION_ARG              6
#define SQLITE_LIMIT_ATTACHED                  7
#define SQLITE_LIMIT_LIKE_PATTERN_LENGTH       8
#define SQLITE_LIMIT_VARIABLE_NUMBER           9
#define SQLITE_LIMIT_TRIGGER_DEPTH            10
#define SQLITE_LIMIT_WORKER_THREADS           11

/*
** CAPI3REF: Prepare Flags
*/
#define SQLITE_PREPARE_PERSISTENT              0x01
#define SQLITE_PREPARE_NORMALIZE               0x02
#define SQLITE_PREPARE_NO_VTAB                 0x04

/*
** CAPI3REF: Compiling An SQL Statement
** KEYWORDS: {SQL statement compiler}
** METHOD: sqlite3
** CONSTRUCTOR: sqlite3_stmt
*/
SQLITE_API int sqlite3_prepare(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
SQLITE_API int sqlite3_prepare_v2(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
SQLITE_API int sqlite3_prepare_v3(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  unsigned int prepFlags, /* Zero or more SQLITE_PREPARE_ flags */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
SQLITE_API int sqlite3_prepare16(
  sqlite3 *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
SQLITE_API int sqlite3_prepare16_v2(
  sqlite3 *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);
SQLITE_API int sqlite3_prepare16_v3(
  sqlite3 *db,            /* Database handle */
  const void *zSql,       /* SQL statement, UTF-16 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  unsigned int prepFlags, /* Zero or more SQLITE_PREPARE_ flags */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const void **pzTail     /* OUT: Pointer to unused portion of zSql */
);

/*
** CAPI3REF: Retrieving Statement SQL
** METHOD: sqlite3_stmt
*/
SQLITE_API const char *sqlite3_sql(sqlite3_stmt *pStmt);
SQLITE_API char *sqlite3_expanded_sql(sqlite3_stmt *pStmt);
SQLITE_API const char *sqlite3_normalized_sql(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Determine If An SQL Statement Writes The Database
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_stmt_readonly(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Query The EXPLAIN Setting For A Prepared Statement
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_stmt_isexplain(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Determine If A Prepared Statement Has Been Reset
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_stmt_busy(sqlite3_stmt*);

/*
** CAPI3REF: Dynamically Typed Value Object
** KEYWORDS: {protected sqlite3_value} {unprotected sqlite3_value}
*/
typedef struct sqlite3_value sqlite3_value;

/*
** CAPI3REF: SQL Function Context Object
*/
typedef struct sqlite3_context sqlite3_context;

/*
** CAPI3REF: Binding Values To Prepared Statements
** KEYWORDS: {host parameter} {host parameters} {host parameter name}
** KEYWORDS: {SQL parameter} {SQL parameters} {parameter binding}
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
SQLITE_API int sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64,
                        void(*)(void*));
SQLITE_API int sqlite3_bind_double(sqlite3_stmt*, int, double);
SQLITE_API int sqlite3_bind_int(sqlite3_stmt*, int, int);
SQLITE_API int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
SQLITE_API int sqlite3_bind_null(sqlite3_stmt*, int);
SQLITE_API int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
SQLITE_API int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64,
                         void(*)(void*), unsigned char encoding);
SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*,void(*)(void*));
SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);

/*
** CAPI3REF: Number Of SQL Parameters
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_bind_parameter_count(sqlite3_stmt*);

/*
** CAPI3REF: Name Of A Host Parameter
** METHOD: sqlite3_stmt
*/
SQLITE_API const char *sqlite3_bind_parameter_name(sqlite3_stmt*, int);

/*
** CAPI3REF: Index Of A Parameter With A Given Name
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);

/*
** CAPI3REF: Reset All Bindings On A Prepared Statement
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_clear_bindings(sqlite3_stmt*);

/*
** CAPI3REF: Number Of Columns In A Result Set
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_column_count(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Column Names In A Result Set
** METHOD: sqlite3_stmt
*/
SQLITE_API const char *sqlite3_column_name(sqlite3_stmt*, int N);
SQLITE_API const void *sqlite3_column_name16(sqlite3_stmt*, int N);

/*
** CAPI3REF: Source Of Data In A Query Result
** METHOD: sqlite3_stmt
*/
SQLITE_API const char *sqlite3_column_database_name(sqlite3_stmt*,int);
SQLITE_API const void *sqlite3_column_database_name16(sqlite3_stmt*,int);
SQLITE_API const char *sqlite3_column_table_name(sqlite3_stmt*,int);
SQLITE_API const void *sqlite3_column_table_name16(sqlite3_stmt*,int);
SQLITE_API const char *sqlite3_column_origin_name(sqlite3_stmt*,int);
SQLITE_API const void *sqlite3_column_origin_name16(sqlite3_stmt*,int);

/*
** CAPI3REF: Declared Datatype Of A Query Result
** METHOD: sqlite3_stmt
*/
SQLITE_API const char *sqlite3_column_decltype(sqlite3_stmt*,int);
SQLITE_API const void *sqlite3_column_decltype16(sqlite3_stmt*,int);

/*
** CAPI3REF: Evaluate An SQL Statement
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_step(sqlite3_stmt*);

/*
** CAPI3REF: Number of columns in a result set
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_data_count(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Fundamental Datatypes
** KEYWORDS: SQLITE_TEXT
*/
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

/*
** CAPI3REF: Result Values From A Query
** KEYWORDS: {column access functions}
** METHOD: sqlite3_stmt
*/
SQLITE_API const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
SQLITE_API double sqlite3_column_double(sqlite3_stmt*, int iCol);
SQLITE_API int sqlite3_column_int(sqlite3_stmt*, int iCol);
SQLITE_API sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
SQLITE_API const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
SQLITE_API const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
SQLITE_API sqlite3_value *sqlite3_column_value(sqlite3_stmt*, int iCol);
SQLITE_API int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
SQLITE_API int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
SQLITE_API int sqlite3_column_type(sqlite3_stmt*, int iCol);

/*
** CAPI3REF: Destroy A Prepared Statement Object
** DESTRUCTOR: sqlite3_stmt
*/
SQLITE_API int sqlite3_finalize(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Reset A Prepared Statement Object
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_reset(sqlite3_stmt *pStmt);

/*
** CAPI3REF: Create Or Redefine SQL Functions
** KEYWORDS: {function creation routines}
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_create_function(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
SQLITE_API int sqlite3_create_function16(
  sqlite3 *db,
  const void *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
SQLITE_API int sqlite3_create_function_v2(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*),
  void(*xDestroy)(void*)
);
SQLITE_API int sqlite3_create_window_function(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*),
  void (*xValue)(sqlite3_context*),
  void (*xInverse)(sqlite3_context*,int,sqlite3_value**),
  void(*xDestroy)(void*)
);

/*
** CAPI3REF: Text Encodings
*/
#define SQLITE_UTF8           1    /* IMP: R-37514-35566 */
#define SQLITE_UTF16LE        2    /* IMP: R-03371-37637 */
#define SQLITE_UTF16BE        3    /* IMP: R-51971-34154 */
#define SQLITE_UTF16          4    /* Use native byte order */
#define SQLITE_ANY            5    /* Deprecated */
#define SQLITE_UTF16_ALIGNED  8    /* sqlite3_create_collation only */

/*
** CAPI3REF: Function Flags
*/
#define SQLITE_DETERMINISTIC    0x000000800
#define SQLITE_DIRECTONLY       0x000080000
#define SQLITE_SUBTYPE          0x000100000
#define SQLITE_INNOCUOUS        0x000200000

/*
** CAPI3REF: Deprecated Functions
*/
#ifndef SQLITE_OMIT_DEPRECATED
SQLITE_API SQLITE_DEPRECATED int sqlite3_aggregate_count(sqlite3_context*);
SQLITE_API SQLITE_DEPRECATED int sqlite3_expired(sqlite3_stmt*);
SQLITE_API SQLITE_DEPRECATED int sqlite3_transfer_bindings(sqlite3_stmt*, sqlite3_stmt*);
SQLITE_API SQLITE_DEPRECATED int sqlite3_global_recover(void);
SQLITE_API SQLITE_DEPRECATED void sqlite3_thread_cleanup(void);
SQLITE_API SQLITE_DEPRECATED int sqlite3_memory_alarm(void(*)(void*,sqlite3_int64,int),
                      void*,sqlite3_int64);
#endif

/*
** CAPI3REF: Obtaining SQL Values
** METHOD: sqlite3_value
*/
SQLITE_API const void *sqlite3_value_blob(sqlite3_value*);
SQLITE_API double sqlite3_value_double(sqlite3_value*);
SQLITE_API int sqlite3_value_int(sqlite3_value*);
SQLITE_API sqlite3_int64 sqlite3_value_int64(sqlite3_value*);
SQLITE_API void *sqlite3_value_pointer(sqlite3_value*, const char*);
SQLITE_API const unsigned char *sqlite3_value_text(sqlite3_value*);
SQLITE_API const void *sqlite3_value_text16(sqlite3_value*);
SQLITE_API const void *sqlite3_value_text16le(sqlite3_value*);
SQLITE_API const void *sqlite3_value_text16be(sqlite3_value*);
SQLITE_API int sqlite3_value_bytes(sqlite3_value*);
SQLITE_API int sqlite3_value_bytes16(sqlite3_value*);
SQLITE_API int sqlite3_value_type(sqlite3_value*);
SQLITE_API int sqlite3_value_numeric_type(sqlite3_value*);
SQLITE_API int sqlite3_value_nochange(sqlite3_value*);
SQLITE_API int sqlite3_value_frombind(sqlite3_value*);

/*
** CAPI3REF: Finding The Subtype Of SQL Values
** METHOD: sqlite3_value
*/
SQLITE_API unsigned int sqlite3_value_subtype(sqlite3_value*);

/*
** CAPI3REF: Copy And Free SQL Values
** METHOD: sqlite3_value
*/
SQLITE_API sqlite3_value *sqlite3_value_dup(const sqlite3_value*);
SQLITE_API void sqlite3_value_free(sqlite3_value*);

/*
** CAPI3REF: Obtain Aggregate Function Context
** METHOD: sqlite3_context
*/
SQLITE_API void *sqlite3_aggregate_context(sqlite3_context*, int nBytes);

/*
** CAPI3REF: User Data For Functions
** METHOD: sqlite3_context
**
** ^The sqlite3_user_data() interface returns a copy of
** the pointer that was the pUserData parameter (the 5th parameter)
** of the [sqlite3_create_function()]
** and [sqlite3_create_function16()] routines that originally
** registered the application defined function.
**
** This routine must be called from the same thread in which
** the application-defined function is running.
*/
SQLITE_API void *sqlite3_user_data(sqlite3_context*);

/*
** CAPI3REF: Database Connection For Functions
** METHOD: sqlite3_context
**
** ^The sqlite3_context_db_handle() interface returns a copy of
** the pointer to the [database connection] (the 1st parameter)
** of the [sqlite3_create_function()]
** and [sqlite3_create_function16()] routines that originally
** registered the application defined function.
*/
SQLITE_API sqlite3 *sqlite3_context_db_handle(sqlite3_context*);

/*
** CAPI3REF: Function Auxiliary Data
** METHOD: sqlite3_context
*/
SQLITE_API void *sqlite3_get_auxdata(sqlite3_context*, int N);
SQLITE_API void sqlite3_set_auxdata(sqlite3_context*, int N, void*, void (*)(void*));


/*
** CAPI3REF: Constants Defining Special Destructor Behavior
*/
typedef void (*sqlite3_destructor_type)(void*);
#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

/*
** CAPI3REF: Setting The Result Of An SQL Function
** METHOD: sqlite3_context
*/
SQLITE_API void sqlite3_result_blob(sqlite3_context*, const void*, int, void(*)(void*));
SQLITE_API void sqlite3_result_blob64(sqlite3_context*,const void*,
                           sqlite3_uint64,void(*)(void*));
SQLITE_API void sqlite3_result_double(sqlite3_context*, double);
SQLITE_API void sqlite3_result_error(sqlite3_context*, const char*, int);
SQLITE_API void sqlite3_result_error16(sqlite3_context*, const void*, int);
SQLITE_API void sqlite3_result_error_toobig(sqlite3_context*);
SQLITE_API void sqlite3_result_error_nomem(sqlite3_context*);
SQLITE_API void sqlite3_result_error_code(sqlite3_context*, int);
SQLITE_API void sqlite3_result_int(sqlite3_context*, int);
SQLITE_API void sqlite3_result_int64(sqlite3_context*, sqlite3_int64);
SQLITE_API void sqlite3_result_null(sqlite3_context*);
SQLITE_API void sqlite3_result_text(sqlite3_context*, const char*, int, void(*)(void*));
SQLITE_API void sqlite3_result_text64(sqlite3_context*, const char*,sqlite3_uint64,
                           void(*)(void*), unsigned char encoding);
SQLITE_API void sqlite3_result_text16(sqlite3_context*, const void*, int, void(*)(void*));
SQLITE_API void sqlite3_result_text16le(sqlite3_context*, const void*, int,void(*)(void*));
SQLITE_API void sqlite3_result_text16be(sqlite3_context*, const void*, int,void(*)(void*));
SQLITE_API void sqlite3_result_value(sqlite3_context*, sqlite3_value*);
SQLITE_API void sqlite3_result_pointer(sqlite3_context*, void*,const char*,void(*)(void*));
SQLITE_API void sqlite3_result_zeroblob(sqlite3_context*, int n);
SQLITE_API int sqlite3_result_zeroblob64(sqlite3_context*, sqlite3_uint64 n);


/*
** CAPI3REF: Setting The Subtype Of An SQL Function
** METHOD: sqlite3_context
*/
SQLITE_API void sqlite3_result_subtype(sqlite3_context*,unsigned int);

/*
** CAPI3REF: Define New Collating Sequences
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_create_collation(
  sqlite3*,
  const char *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);
SQLITE_API int sqlite3_create_collation_v2(
  sqlite3*,
  const char *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDestroy)(void*)
);
SQLITE_API int sqlite3_create_collation16(
  sqlite3*,
  const void *zName,
  int eTextRep,
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

/*
** CAPI3REF: Collation Needed Callbacks
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_collation_needed(
  sqlite3*,
  void*,
  void(*)(void*,sqlite3*,int eTextRep,const char*)
);
SQLITE_API int sqlite3_collation_needed16(
  sqlite3*,
  void*,
  void(*)(void*,sqlite3*,int eTextRep,const void*)
);

#ifdef SQLITE_ENABLE_CEROD
/*
** Specify the activation key for a CEROD database.  Unless
** activated, none of the CEROD routines will work.
*/
SQLITE_API void sqlite3_activate_cerod(
  const char *zPassPhrase        /* Activation phrase */
);
#endif

/*
** CAPI3REF: Suspend Execution For A Short Time
*/
SQLITE_API int sqlite3_sleep(int);

/*
** CAPI3REF: Name Of The Folder Holding Temporary Files
*/
SQLITE_API SQLITE_EXTERN char *sqlite3_temp_directory;

/*
** CAPI3REF: Name Of The Folder Holding Database Files
*/
SQLITE_API SQLITE_EXTERN char *sqlite3_data_directory;

/*
** CAPI3REF: Win32 Specific Interface
*/
SQLITE_API int sqlite3_win32_set_directory(
  unsigned long type, /* Identifier for directory being set or reset */
  void *zValue        /* New value for directory being set or reset */
);
SQLITE_API int sqlite3_win32_set_directory8(unsigned long type, const char *zValue);
SQLITE_API int sqlite3_win32_set_directory16(unsigned long type, const void *zValue);

/*
** CAPI3REF: Win32 Directory Types
**
** These macros are only available on Windows.  They define the allowed values
** for the type argument to the [sqlite3_win32_set_directory] interface.
*/
#define SQLITE_WIN32_DATA_DIRECTORY_TYPE  1
#define SQLITE_WIN32_TEMP_DIRECTORY_TYPE  2

/*
** CAPI3REF: Test For Auto-Commit Mode
** KEYWORDS: {autocommit mode}
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_get_autocommit(sqlite3*);

/*
** CAPI3REF: Find The Database Handle Of A Prepared Statement
** METHOD: sqlite3_stmt
*/
SQLITE_API sqlite3 *sqlite3_db_handle(sqlite3_stmt*);

/*
** CAPI3REF: Return The Filename For A Database Connection
** METHOD: sqlite3
*/
SQLITE_API const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);

/*
** CAPI3REF: Determine if a database is read-only
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_db_readonly(sqlite3 *db, const char *zDbName);

/*
** CAPI3REF: Determine the transaction state of a database
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_txn_state(sqlite3*,const char *zSchema);

/*
** CAPI3REF: Allowed return values from [sqlite3_txn_state()]
** KEYWORDS: {transaction state}
*/
#define SQLITE_TXN_NONE  0
#define SQLITE_TXN_READ  1
#define SQLITE_TXN_WRITE 2

/*
** CAPI3REF: Find the next prepared statement
** METHOD: sqlite3
*/
SQLITE_API sqlite3_stmt *sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt);

/*
** CAPI3REF: Commit And Rollback Notification Callbacks
** METHOD: sqlite3
**
** ^The sqlite3_commit_hook() interface registers a callback
** function to be invoked whenever a transaction is [COMMIT | committed].
** ^Any callback set by a previous call to sqlite3_commit_hook()
** for the same database connection is overridden.
** ^The sqlite3_rollback_hook() interface registers a callback
** function to be invoked whenever a transaction is [ROLLBACK | rolled back].
** ^Any callback set by a previous call to sqlite3_rollback_hook()
** for the same database connection is overridden.
** ^The pArg argument is passed through to the callback.
** ^If the callback on a commit hook function returns non-zero,
** then the commit is converted into a rollback.
**
** ^The sqlite3_commit_hook(D,C,P) and sqlite3_rollback_hook(D,C,P) functions
** return the P argument from the previous call of the same function
** on the same [database connection] D, or NULL for
** the first call for each function on D.
**
** The commit and rollback hook callbacks are not reentrant.
** The callback implementation must not do anything that will modify
** the database connection that invoked the callback.  Any actions
** to modify the database connection must be deferred until after the
** completion of the [sqlite3_step()] call that triggered the commit
** or rollback hook in the first place.
** Note that running any other SQL statements, including SELECT statements,
** or merely calling [sqlite3_prepare_v2()] and [sqlite3_step()] will modify
** the database connections for the meaning of "modify" in this paragraph.
**
** ^Registering a NULL function disables the callback.
**
** ^When the commit hook callback routine returns zero, the [COMMIT]
** operation is allowed to continue normally.  ^If the commit hook
** returns non-zero, then the [COMMIT] is converted into a [ROLLBACK].
** ^The rollback hook is invoked on a rollback that results from a commit
** hook returning non-zero, just as it would be with any other rollback.
**
** ^For the purposes of this API, a transaction is said to have been
** rolled back if an explicit "ROLLBACK" statement is executed, or
** an error or constraint causes an implicit rollback to occur.
** ^The rollback callback is not invoked if a transaction is
** automatically rolled back because the database connection is closed.
**
** See also the [sqlite3_update_hook()] interface.
*/
SQLITE_API void *sqlite3_commit_hook(sqlite3*, int(*)(void*), void*);
SQLITE_API void *sqlite3_rollback_hook(sqlite3*, void(*)(void *), void*);

/*
** CAPI3REF: Data Change Notification Callbacks
** METHOD: sqlite3
**
** ^The sqlite3_update_hook() interface registers a callback function
** with the [database connection] identified by the first argument
** to be invoked whenever a row is updated, inserted or deleted in
** a [rowid table].
** ^Any callback set by a previous call to this function
** for the same database connection is overridden.
**
** ^The second argument is a pointer to the function to invoke when a
** row is updated, inserted or deleted in a rowid table.
** ^The first argument to the callback is a copy of the third argument
** to sqlite3_update_hook().
** ^The second callback argument is one of [SQLITE_INSERT], [SQLITE_DELETE],
** or [SQLITE_UPDATE], depending on the operation that caused the callback
** to be invoked.
** ^The third and fourth arguments to the callback contain pointers to the
** database and table name containing the affected row.
** ^The final callback parameter is the [rowid] of the row.
** ^In the case of an update, this is the [rowid] after the update takes place.
**
** ^(The update hook is not invoked when internal system tables are
** modified (i.e. sqlite_sequence).)^
** ^The update hook is not invoked when [WITHOUT ROWID] tables are modified.
**
** ^In the current implementation, the update hook
** is not invoked when conflicting rows are deleted because of an
** [ON CONFLICT | ON CONFLICT REPLACE] clause.  ^Nor is the update hook
** invoked when rows are deleted using the [truncate optimization].
** The exceptions defined in this paragraph might change in a future
** release of SQLite.
**
** The update hook implementation must not do anything that will modify
** the database connection that invoked the update hook.  Any actions
** to modify the database connection must be deferred until after the
** completion of the [sqlite3_step()] call that triggered the update hook.
** Note that [sqlite3_prepare_v2()] and [sqlite3_step()] both modify their
** database connections for the meaning of "modify" in this paragraph.
**
** ^The sqlite3_update_hook(D,C,P) function
** returns the P argument from the previous call
** on the same [database connection] D, or NULL for
** the first call on D.
**
** See also the [sqlite3_commit_hook()], [sqlite3_rollback_hook()],
** and [sqlite3_preupdate_hook()] interfaces.
*/
SQLITE_API void *sqlite3_update_hook(
  sqlite3*,
  void(*)(void *,int ,char const *,char const *,sqlite3_int64),
  void*
);

/*
** CAPI3REF: Enable Or Disable Shared Pager Cache
*/
SQLITE_API int sqlite3_enable_shared_cache(int);

/*
** CAPI3REF: Attempt To Free Heap Memory
*/
SQLITE_API int sqlite3_release_memory(int);

/*
** CAPI3REF: Free Memory Used By A Database Connection
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_db_release_memory(sqlite3*);

/*
** CAPI3REF: Impose A Limit On Heap Size
*/
SQLITE_API sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64 N);
SQLITE_API sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64 N);

/*
** CAPI3REF: Deprecated Soft Heap Limit Interface
** DEPRECATED
**
** This is a deprecated version of the [sqlite3_soft_heap_limit64()]
** interface.  This routine is provided for historical compatibility
** only.  All new applications should use the
** [sqlite3_soft_heap_limit64()] interface rather than this one.
*/
SQLITE_API SQLITE_DEPRECATED void sqlite3_soft_heap_limit(int N);


/*
** CAPI3REF: Extract Metadata About A Column Of A Table
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_table_column_metadata(
  sqlite3 *db,                /* Connection handle */
  const char *zDbName,        /* Database name or NULL */
  const char *zTableName,     /* Table name */
  const char *zColumnName,    /* Column name */
  char const **pzDataType,    /* OUTPUT: Declared data type */
  char const **pzCollSeq,     /* OUTPUT: Collation sequence name */
  int *pNotNull,              /* OUTPUT: True if NOT NULL constraint exists */
  int *pPrimaryKey,           /* OUTPUT: True if column part of PK */
  int *pAutoinc               /* OUTPUT: True if column is auto-increment */
);

/*
** CAPI3REF: Load An Extension
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_load_extension(
  sqlite3 *db,          /* Load the extension into this database connection */
  const char *zFile,    /* Name of the shared library containing extension */
  const char *zProc,    /* Entry point.  Derived from zFile if 0 */
  char **pzErrMsg       /* Put error message here if not 0 */
);

/*
** CAPI3REF: Enable Or Disable Extension Loading
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_enable_load_extension(sqlite3 *db, int onoff);

/*
** CAPI3REF: Automatically Load Statically Linked Extensions
*/
SQLITE_API int sqlite3_auto_extension(void(*xEntryPoint)(void));

/*
** CAPI3REF: Cancel Automatic Extension Loading
*/
SQLITE_API int sqlite3_cancel_auto_extension(void(*xEntryPoint)(void));

/*
** CAPI3REF: Reset Automatic Extension Loading
**
** ^This interface disables all automatic extensions previously
** registered using [sqlite3_auto_extension()].
*/
SQLITE_API void sqlite3_reset_auto_extension(void);

/*
** The interface to the virtual-table mechanism is currently considered
** to be experimental.  The interface might change in incompatible ways.
** If this is a problem for you, do not use the interface at this time.
**
** When the virtual-table mechanism stabilizes, we will declare the
** interface fixed, support it indefinitely, and remove this comment.
*/

/*
** Structures used by the virtual table interface
*/
typedef struct sqlite3_vtab sqlite3_vtab;
typedef struct sqlite3_index_info sqlite3_index_info;
typedef struct sqlite3_vtab_cursor sqlite3_vtab_cursor;
typedef struct sqlite3_module sqlite3_module;

/*
** CAPI3REF: Virtual Table Object
** KEYWORDS: sqlite3_module {virtual table module}
**
** This structure, sometimes called a "virtual table module",
** defines the implementation of a [virtual table].
** This structure consists mostly of methods for the module.
**
** ^A virtual table module is created by filling in a persistent
** instance of this structure and passing a pointer to that instance
** to [sqlite3_create_module()] or [sqlite3_create_module_v2()].
** ^The registration remains valid until it is replaced by a different
** module or until the [database connection] closes.  The content
** of this structure must not change while it is registered with
** any database connection.
*/
struct sqlite3_module {
  int iVersion;
  int (*xCreate)(sqlite3*, void *pAux,
               int argc, const char *const*argv,
               sqlite3_vtab **ppVTab, char**);
  int (*xConnect)(sqlite3*, void *pAux,
               int argc, const char *const*argv,
               sqlite3_vtab **ppVTab, char**);
  int (*xBestIndex)(sqlite3_vtab *pVTab, sqlite3_index_info*);
  int (*xDisconnect)(sqlite3_vtab *pVTab);
  int (*xDestroy)(sqlite3_vtab *pVTab);
  int (*xOpen)(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
  int (*xClose)(sqlite3_vtab_cursor*);
  int (*xFilter)(sqlite3_vtab_cursor*, int idxNum, const char *idxStr,
                int argc, sqlite3_value **argv);
  int (*xNext)(sqlite3_vtab_cursor*);
  int (*xEof)(sqlite3_vtab_cursor*);
  int (*xColumn)(sqlite3_vtab_cursor*, sqlite3_context*, int);
  int (*xRowid)(sqlite3_vtab_cursor*, sqlite3_int64 *pRowid);
  int (*xUpdate)(sqlite3_vtab *, int, sqlite3_value **, sqlite3_int64 *);
  int (*xBegin)(sqlite3_vtab *pVTab);
  int (*xSync)(sqlite3_vtab *pVTab);
  int (*xCommit)(sqlite3_vtab *pVTab);
  int (*xRollback)(sqlite3_vtab *pVTab);
  int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
                       void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                       void **ppArg);
  int (*xRename)(sqlite3_vtab *pVtab, const char *zNew);
  /* The methods above are in version 1 of the sqlite_module object. Those
  ** below are for version 2 and greater. */
  int (*xSavepoint)(sqlite3_vtab *pVTab, int);
  int (*xRelease)(sqlite3_vtab *pVTab, int);
  int (*xRollbackTo)(sqlite3_vtab *pVTab, int);
  /* The methods above are in versions 1 and 2 of the sqlite_module object.
  ** Those below are for version 3 and greater. */
  int (*xShadowName)(const char*);
};

/*
** CAPI3REF: Virtual Table Indexing Information
** KEYWORDS: sqlite3_index_info
*/
struct sqlite3_index_info {
  /* Inputs */
  int nConstraint;           /* Number of entries in aConstraint */
  struct sqlite3_index_constraint {
     int iColumn;              /* Column constrained.  -1 for ROWID */
     unsigned char op;         /* Constraint operator */
     unsigned char usable;     /* True if this constraint is usable */
     int iTermOffset;          /* Used internally - xBestIndex should ignore */
  } *aConstraint;            /* Table of WHERE clause constraints */
  int nOrderBy;              /* Number of terms in the ORDER BY clause */
  struct sqlite3_index_orderby {
     int iColumn;              /* Column number */
     unsigned char desc;       /* True for DESC.  False for ASC. */
  } *aOrderBy;               /* The ORDER BY clause */
  /* Outputs */
  struct sqlite3_index_constraint_usage {
    int argvIndex;           /* if >0, constraint is part of argv to xFilter */
    unsigned char omit;      /* Do not code a test for this constraint */
  } *aConstraintUsage;
  int idxNum;                /* Number used to identify the index */
  char *idxStr;              /* String, possibly obtained from sqlite3_malloc */
  int needToFreeIdxStr;      /* Free idxStr using sqlite3_free() if true */
  int orderByConsumed;       /* True if output is already ordered */
  double estimatedCost;           /* Estimated cost of using this index */
  /* Fields below are only available in SQLite 3.8.2 and later */
  sqlite3_int64 estimatedRows;    /* Estimated number of rows returned */
  /* Fields below are only available in SQLite 3.9.0 and later */
  int idxFlags;              /* Mask of SQLITE_INDEX_SCAN_* flags */
  /* Fields below are only available in SQLite 3.10.0 and later */
  sqlite3_uint64 colUsed;    /* Input: Mask of columns used by statement */
};

/*
** CAPI3REF: Virtual Table Scan Flags
**
** Virtual table implementations are allowed to set the
** [sqlite3_index_info].idxFlags field to some combination of
** these bits.
*/
#define SQLITE_INDEX_SCAN_UNIQUE      1     /* Scan visits at most 1 row */

/*
** CAPI3REF: Virtual Table Constraint Operator Codes
**
** These macros define the allowed values for the
** [sqlite3_index_info].aConstraint[].op field.  Each value represents
** an operator that is part of a constraint term in the wHERE clause of
** a query that uses a [virtual table].
*/
#define SQLITE_INDEX_CONSTRAINT_EQ         2
#define SQLITE_INDEX_CONSTRAINT_GT         4
#define SQLITE_INDEX_CONSTRAINT_LE         8
#define SQLITE_INDEX_CONSTRAINT_LT        16
#define SQLITE_INDEX_CONSTRAINT_GE        32
#define SQLITE_INDEX_CONSTRAINT_MATCH     64
#define SQLITE_INDEX_CONSTRAINT_LIKE      65
#define SQLITE_INDEX_CONSTRAINT_GLOB      66
#define SQLITE_INDEX_CONSTRAINT_REGEXP    67
#define SQLITE_INDEX_CONSTRAINT_NE        68
#define SQLITE_INDEX_CONSTRAINT_ISNOT     69
#define SQLITE_INDEX_CONSTRAINT_ISNOTNULL 70
#define SQLITE_INDEX_CONSTRAINT_ISNULL    71
#define SQLITE_INDEX_CONSTRAINT_IS        72
#define SQLITE_INDEX_CONSTRAINT_FUNCTION 150

/*
** CAPI3REF: Register A Virtual Table Implementation
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_create_module(
  sqlite3 *db,               /* SQLite connection to register module with */
  const char *zName,         /* Name of the module */
  const sqlite3_module *p,   /* Methods for the module */
  void *pClientData          /* Client data for xCreate/xConnect */
);
SQLITE_API int sqlite3_create_module_v2(
  sqlite3 *db,               /* SQLite connection to register module with */
  const char *zName,         /* Name of the module */
  const sqlite3_module *p,   /* Methods for the module */
  void *pClientData,         /* Client data for xCreate/xConnect */
  void(*xDestroy)(void*)     /* Module destructor function */
);

/*
** CAPI3REF: Remove Unnecessary Virtual Table Implementations
** METHOD: sqlite3
**
** ^The sqlite3_drop_modules(D,L) interface removes all virtual
** table modules from database connection D except those named on list L.
** The L parameter must be either NULL or a pointer to an array of pointers
** to strings where the array is terminated by a single NULL pointer.
** ^If the L parameter is NULL, then all virtual table modules are removed.
**
** See also: [sqlite3_create_module()]
*/
SQLITE_API int sqlite3_drop_modules(
  sqlite3 *db,                /* Remove modules from this connection */
  const char **azKeep         /* Except, do not remove the ones named here */
);

/*
** CAPI3REF: Virtual Table Instance Object
** KEYWORDS: sqlite3_vtab
**
** Every [virtual table module] implementation uses a subclass
** of this object to describe a particular instance
** of the [virtual table].  Each subclass will
** be tailored to the specific needs of the module implementation.
** The purpose of this superclass is to define certain fields that are
** common to all module implementations.
**
** ^Virtual tables methods can set an error message by assigning a
** string obtained from [sqlite3_mprintf()] to zErrMsg.  The method should
** take care that any prior string is freed by a call to [sqlite3_free()]
** prior to assigning a new string to zErrMsg.  ^After the error message
** is delivered up to the client application, the string will be automatically
** freed by sqlite3_free() and the zErrMsg field will be zeroed.
*/
struct sqlite3_vtab {
  const sqlite3_module *pModule;  /* The module for this virtual table */
  int nRef;                       /* Number of open cursors */
  char *zErrMsg;                  /* Error message from sqlite3_mprintf() */
  /* Virtual table implementations will typically add additional fields */
};

/*
** CAPI3REF: Virtual Table Cursor Object
** KEYWORDS: sqlite3_vtab_cursor {virtual table cursor}
**
** Every [virtual table module] implementation uses a subclass of the
** following structure to describe cursors that point into the
** [virtual table] and are used
** to loop through the virtual table.  Cursors are created using the
** [sqlite3_module.xOpen | xOpen] method of the module and are destroyed
** by the [sqlite3_module.xClose | xClose] method.  Cursors are used
** by the [xFilter], [xNext], [xEof], [xColumn], and [xRowid] methods
** of the module.  Each module implementation will define
** the content of a cursor structure to suit its own needs.
**
** This superclass exists in order to define fields of the cursor that
** are common to all implementations.
*/
struct sqlite3_vtab_cursor {
  sqlite3_vtab *pVtab;      /* Virtual table of this cursor */
  /* Virtual table implementations will typically add additional fields */
};

/*
** CAPI3REF: Declare The Schema Of A Virtual Table
**
** ^The [xCreate] and [xConnect] methods of a
** [virtual table module] call this interface
** to declare the format (the names and datatypes of the columns) of
** the virtual tables they implement.
*/
SQLITE_API int sqlite3_declare_vtab(sqlite3*, const char *zSQL);

/*
** CAPI3REF: Overload A Function For A Virtual Table
** METHOD: sqlite3
**
** ^(Virtual tables can provide alternative implementations of functions
** using the [xFindFunction] method of the [virtual table module].
** But global versions of those functions
** must exist in order to be overloaded.)^
**
** ^(This API makes sure a global version of a function with a particular
** name and number of parameters exists.  If no such function exists
** before this API is called, a new function is created.)^  ^The implementation
** of the new function always causes an exception to be thrown.  So
** the new function is not good for anything by itself.  Its only
** purpose is to be a placeholder function that can be overloaded
** by a [virtual table].
*/
SQLITE_API int sqlite3_overload_function(sqlite3*, const char *zFuncName, int nArg);

/*
** The interface to the virtual-table mechanism defined above (back up
** to a comment remarkably similar to this one) is currently considered
** to be experimental.  The interface might change in incompatible ways.
** If this is a problem for you, do not use the interface at this time.
**
** When the virtual-table mechanism stabilizes, we will declare the
** interface fixed, support it indefinitely, and remove this comment.
*/

/*
** CAPI3REF: A Handle To An Open BLOB
** KEYWORDS: {BLOB handle} {BLOB handles}
**
** An instance of this object represents an open BLOB on which
** [sqlite3_blob_open | incremental BLOB I/O] can be performed.
** ^Objects of this type are created by [sqlite3_blob_open()]
** and destroyed by [sqlite3_blob_close()].
** ^The [sqlite3_blob_read()] and [sqlite3_blob_write()] interfaces
** can be used to read or write small subsections of the BLOB.
** ^The [sqlite3_blob_bytes()] interface returns the size of the BLOB in bytes.
*/
typedef struct sqlite3_blob sqlite3_blob;

/*
** CAPI3REF: Open A BLOB For Incremental I/O
** METHOD: sqlite3
** CONSTRUCTOR: sqlite3_blob
*/
SQLITE_API int sqlite3_blob_open(
  sqlite3*,
  const char *zDb,
  const char *zTable,
  const char *zColumn,
  sqlite3_int64 iRow,
  int flags,
  sqlite3_blob **ppBlob
);

/*
** CAPI3REF: Move a BLOB Handle to a New Row
** METHOD: sqlite3_blob
*/
SQLITE_API int sqlite3_blob_reopen(sqlite3_blob *, sqlite3_int64);

/*
** CAPI3REF: Close A BLOB Handle
** DESTRUCTOR: sqlite3_blob
*/
SQLITE_API int sqlite3_blob_close(sqlite3_blob *);

/*
** CAPI3REF: Return The Size Of An Open BLOB
** METHOD: sqlite3_blob
*/
SQLITE_API int sqlite3_blob_bytes(sqlite3_blob *);

/*
** CAPI3REF: Read Data From A BLOB Incrementally
** METHOD: sqlite3_blob
**
** ^(This function is used to read data from an open [BLOB handle] into a
** caller-supplied buffer. N bytes of data are copied into buffer Z
** from the open BLOB, starting at offset iOffset.)^
**
** ^If offset iOffset is less than N bytes from the end of the BLOB,
** [SQLITE_ERROR] is returned and no data is read.  ^If N or iOffset is
** less than zero, [SQLITE_ERROR] is returned and no data is read.
** ^The size of the blob (and hence the maximum value of N+iOffset)
** can be determined using the [sqlite3_blob_bytes()] interface.
**
** ^An attempt to read from an expired [BLOB handle] fails with an
** error code of [SQLITE_ABORT].
**
** ^(On success, sqlite3_blob_read() returns SQLITE_OK.
** Otherwise, an [error code] or an [extended error code] is returned.)^
**
** This routine only works on a [BLOB handle] which has been created
** by a prior successful call to [sqlite3_blob_open()] and which has not
** been closed by [sqlite3_blob_close()].  Passing any other pointer in
** to this routine results in undefined and probably undesirable behavior.
**
** See also: [sqlite3_blob_write()].
*/
SQLITE_API int sqlite3_blob_read(sqlite3_blob *, void *Z, int N, int iOffset);

/*
** CAPI3REF: Write Data Into A BLOB Incrementally
** METHOD: sqlite3_blob
**
** ^(This function is used to write data into an open [BLOB handle] from a
** caller-supplied buffer. N bytes of data are copied from the buffer Z
** into the open BLOB, starting at offset iOffset.)^
**
** ^(On success, sqlite3_blob_write() returns SQLITE_OK.
** Otherwise, an  [error code] or an [extended error code] is returned.)^
** ^Unless SQLITE_MISUSE is returned, this function sets the
** [database connection] error code and message accessible via
** [sqlite3_errcode()] and [sqlite3_errmsg()] and related functions.
**
** ^If the [BLOB handle] passed as the first argument was not opened for
** writing (the flags parameter to [sqlite3_blob_open()] was zero),
** this function returns [SQLITE_READONLY].
**
** This function may only modify the contents of the BLOB; it is
** not possible to increase the size of a BLOB using this API.
** ^If offset iOffset is less than N bytes from the end of the BLOB,
** [SQLITE_ERROR] is returned and no data is written. The size of the
** BLOB (and hence the maximum value of N+iOffset) can be determined
** using the [sqlite3_blob_bytes()] interface. ^If N or iOffset are less
** than zero [SQLITE_ERROR] is returned and no data is written.
**
** ^An attempt to write to an expired [BLOB handle] fails with an
** error code of [SQLITE_ABORT].  ^Writes to the BLOB that occurred
** before the [BLOB handle] expired are not rolled back by the
** expiration of the handle, though of course those changes might
** have been overwritten by the statement that expired the BLOB handle
** or by other independent statements.
**
** This routine only works on a [BLOB handle] which has been created
** by a prior successful call to [sqlite3_blob_open()] and which has not
** been closed by [sqlite3_blob_close()].  Passing any other pointer in
** to this routine results in undefined and probably undesirable behavior.
**
** See also: [sqlite3_blob_read()].
*/
SQLITE_API int sqlite3_blob_write(sqlite3_blob *, const void *z, int n, int iOffset);

/*
** CAPI3REF: Virtual File System Objects
**
** A virtual filesystem (VFS) is an [sqlite3_vfs] object
** that SQLite uses to interact
** with the underlying operating system.  Most SQLite builds come with a
** single default VFS that is appropriate for the host computer.
** New VFSes can be registered and existing VFSes can be unregistered.
** The following interfaces are provided.
**
** ^The sqlite3_vfs_find() interface returns a pointer to a VFS given its name.
** ^Names are case sensitive.
** ^Names are zero-terminated UTF-8 strings.
** ^If there is no match, a NULL pointer is returned.
** ^If zVfsName is NULL then the default VFS is returned.
**
** ^New VFSes are registered with sqlite3_vfs_register().
** ^Each new VFS becomes the default VFS if the makeDflt flag is set.
** ^The same VFS can be registered multiple times without injury.
** ^To make an existing VFS into the default VFS, register it again
** with the makeDflt flag set.  If two different VFSes with the
** same name are registered, the behavior is undefined.  If a
** VFS is registered with a name that is NULL or an empty string,
** then the behavior is undefined.
**
** ^Unregister a VFS with the sqlite3_vfs_unregister() interface.
** ^(If the default VFS is unregistered, another VFS is chosen as
** the default.  The choice for the new VFS is arbitrary.)^
*/
SQLITE_API sqlite3_vfs *sqlite3_vfs_find(const char *zVfsName);
SQLITE_API int sqlite3_vfs_register(sqlite3_vfs*, int makeDflt);
SQLITE_API int sqlite3_vfs_unregister(sqlite3_vfs*);

/*
** CAPI3REF: Mutexes
*/
SQLITE_API sqlite3_mutex *sqlite3_mutex_alloc(int);
SQLITE_API void sqlite3_mutex_free(sqlite3_mutex*);
SQLITE_API void sqlite3_mutex_enter(sqlite3_mutex*);
SQLITE_API int sqlite3_mutex_try(sqlite3_mutex*);
SQLITE_API void sqlite3_mutex_leave(sqlite3_mutex*);

/*
** CAPI3REF: Mutex Methods Object
*/
typedef struct sqlite3_mutex_methods sqlite3_mutex_methods;
struct sqlite3_mutex_methods {
  int (*xMutexInit)(void);
  int (*xMutexEnd)(void);
  sqlite3_mutex *(*xMutexAlloc)(int);
  void (*xMutexFree)(sqlite3_mutex *);
  void (*xMutexEnter)(sqlite3_mutex *);
  int (*xMutexTry)(sqlite3_mutex *);
  void (*xMutexLeave)(sqlite3_mutex *);
  int (*xMutexHeld)(sqlite3_mutex *);
  int (*xMutexNotheld)(sqlite3_mutex *);
};

/*
** CAPI3REF: Mutex Verification Routines
*/
#ifndef NDEBUG
SQLITE_API int sqlite3_mutex_held(sqlite3_mutex*);
SQLITE_API int sqlite3_mutex_notheld(sqlite3_mutex*);
#endif

/*
** CAPI3REF: Mutex Types
**
** The [sqlite3_mutex_alloc()] interface takes a single argument
** which is one of these integer constants.
**
** The set of static mutexes may change from one SQLite release to the
** next.  Applications that override the built-in mutex logic must be
** prepared to accommodate additional static mutexes.
*/
#define SQLITE_MUTEX_FAST             0
#define SQLITE_MUTEX_RECURSIVE        1
#define SQLITE_MUTEX_STATIC_MAIN      2
#define SQLITE_MUTEX_STATIC_MEM       3  /* sqlite3_malloc() */
#define SQLITE_MUTEX_STATIC_MEM2      4  /* NOT USED */
#define SQLITE_MUTEX_STATIC_OPEN      4  /* sqlite3BtreeOpen() */
#define SQLITE_MUTEX_STATIC_PRNG      5  /* sqlite3_randomness() */
#define SQLITE_MUTEX_STATIC_LRU       6  /* lru page list */
#define SQLITE_MUTEX_STATIC_LRU2      7  /* NOT USED */
#define SQLITE_MUTEX_STATIC_PMEM      7  /* sqlite3PageMalloc() */
#define SQLITE_MUTEX_STATIC_APP1      8  /* For use by application */
#define SQLITE_MUTEX_STATIC_APP2      9  /* For use by application */
#define SQLITE_MUTEX_STATIC_APP3     10  /* For use by application */
#define SQLITE_MUTEX_STATIC_VFS1     11  /* For use by built-in VFS */
#define SQLITE_MUTEX_STATIC_VFS2     12  /* For use by extension VFS */
#define SQLITE_MUTEX_STATIC_VFS3     13  /* For use by application VFS */

/* Legacy compatibility: */
#define SQLITE_MUTEX_STATIC_MASTER    2


/*
** CAPI3REF: Retrieve the mutex for a database connection
** METHOD: sqlite3
**
** ^This interface returns a pointer the [sqlite3_mutex] object that
** serializes access to the [database connection] given in the argument
** when the [threading mode] is Serialized.
** ^If the [threading mode] is Single-thread or Multi-thread then this
** routine returns a NULL pointer.
*/
SQLITE_API sqlite3_mutex *sqlite3_db_mutex(sqlite3*);

/*
** CAPI3REF: Low-Level Control Of Database Files
** METHOD: sqlite3
** KEYWORDS: {file control}
*/
SQLITE_API int sqlite3_file_control(sqlite3*, const char *zDbName, int op, void*);

/*
** CAPI3REF: Testing Interface
*/
SQLITE_API int sqlite3_test_control(int op, ...);

/*
** CAPI3REF: Testing Interface Operation Codes
*/
#define SQLITE_TESTCTRL_FIRST                    5
#define SQLITE_TESTCTRL_PRNG_SAVE                5
#define SQLITE_TESTCTRL_PRNG_RESTORE             6
#define SQLITE_TESTCTRL_PRNG_RESET               7  /* NOT USED */
#define SQLITE_TESTCTRL_BITVEC_TEST              8
#define SQLITE_TESTCTRL_FAULT_INSTALL            9
#define SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS     10
#define SQLITE_TESTCTRL_PENDING_BYTE            11
#define SQLITE_TESTCTRL_ASSERT                  12
#define SQLITE_TESTCTRL_ALWAYS                  13
#define SQLITE_TESTCTRL_RESERVE                 14  /* NOT USED */
#define SQLITE_TESTCTRL_OPTIMIZATIONS           15
#define SQLITE_TESTCTRL_ISKEYWORD               16  /* NOT USED */
#define SQLITE_TESTCTRL_SCRATCHMALLOC           17  /* NOT USED */
#define SQLITE_TESTCTRL_INTERNAL_FUNCTIONS      17
#define SQLITE_TESTCTRL_LOCALTIME_FAULT         18
#define SQLITE_TESTCTRL_EXPLAIN_STMT            19  /* NOT USED */
#define SQLITE_TESTCTRL_ONCE_RESET_THRESHOLD    19
#define SQLITE_TESTCTRL_NEVER_CORRUPT           20
#define SQLITE_TESTCTRL_VDBE_COVERAGE           21
#define SQLITE_TESTCTRL_BYTEORDER               22
#define SQLITE_TESTCTRL_ISINIT                  23
#define SQLITE_TESTCTRL_SORTER_MMAP             24
#define SQLITE_TESTCTRL_IMPOSTER                25
#define SQLITE_TESTCTRL_PARSER_COVERAGE         26
#define SQLITE_TESTCTRL_RESULT_INTREAL          27
#define SQLITE_TESTCTRL_PRNG_SEED               28
#define SQLITE_TESTCTRL_EXTRA_SCHEMA_CHECKS     29
#define SQLITE_TESTCTRL_SEEK_COUNT              30
#define SQLITE_TESTCTRL_TRACEFLAGS              31
#define SQLITE_TESTCTRL_TUNE                    32
#define SQLITE_TESTCTRL_LAST                    32  /* Largest TESTCTRL */

/*
** CAPI3REF: SQL Keyword Checking
*/
SQLITE_API int sqlite3_keyword_count(void);
SQLITE_API int sqlite3_keyword_name(int,const char**,int*);
SQLITE_API int sqlite3_keyword_check(const char*,int);

/*
** CAPI3REF: Dynamic String Object
** KEYWORDS: {dynamic string}
*/
typedef struct sqlite3_str sqlite3_str;

/*
** CAPI3REF: Create A New Dynamic String Object
** CONSTRUCTOR: sqlite3_str
*/
SQLITE_API sqlite3_str *sqlite3_str_new(sqlite3*);

/*
** CAPI3REF: Finalize A Dynamic String
** DESTRUCTOR: sqlite3_str
*/
SQLITE_API char *sqlite3_str_finish(sqlite3_str*);

/*
** CAPI3REF: Add Content To A Dynamic String
** METHOD: sqlite3_str
*/
SQLITE_API void sqlite3_str_appendf(sqlite3_str*, const char *zFormat, ...);
SQLITE_API void sqlite3_str_vappendf(sqlite3_str*, const char *zFormat, va_list);
SQLITE_API void sqlite3_str_append(sqlite3_str*, const char *zIn, int N);
SQLITE_API void sqlite3_str_appendall(sqlite3_str*, const char *zIn);
SQLITE_API void sqlite3_str_appendchar(sqlite3_str*, int N, char C);
SQLITE_API void sqlite3_str_reset(sqlite3_str*);

/*
** CAPI3REF: Status Of A Dynamic String
** METHOD: sqlite3_str
*/
SQLITE_API int sqlite3_str_errcode(sqlite3_str*);
SQLITE_API int sqlite3_str_length(sqlite3_str*);
SQLITE_API char *sqlite3_str_value(sqlite3_str*);

/*
** CAPI3REF: SQLite Runtime Status
*/
SQLITE_API int sqlite3_status(int op, int *pCurrent, int *pHighwater, int resetFlag);
SQLITE_API int sqlite3_status64(
  int op,
  sqlite3_int64 *pCurrent,
  sqlite3_int64 *pHighwater,
  int resetFlag
);


/*
** CAPI3REF: Status Parameters
** KEYWORDS: {status parameters}
*/
#define SQLITE_STATUS_MEMORY_USED          0
#define SQLITE_STATUS_PAGECACHE_USED       1
#define SQLITE_STATUS_PAGECACHE_OVERFLOW   2
#define SQLITE_STATUS_SCRATCH_USED         3  /* NOT USED */
#define SQLITE_STATUS_SCRATCH_OVERFLOW     4  /* NOT USED */
#define SQLITE_STATUS_MALLOC_SIZE          5
#define SQLITE_STATUS_PARSER_STACK         6
#define SQLITE_STATUS_PAGECACHE_SIZE       7
#define SQLITE_STATUS_SCRATCH_SIZE         8  /* NOT USED */
#define SQLITE_STATUS_MALLOC_COUNT         9

/*
** CAPI3REF: Database Connection Status
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_db_status(sqlite3*, int op, int *pCur, int *pHiwtr, int resetFlg);

/*
** CAPI3REF: Status Parameters for database connections
** KEYWORDS: {SQLITE_DBSTATUS options}
*/
#define SQLITE_DBSTATUS_LOOKASIDE_USED       0
#define SQLITE_DBSTATUS_CACHE_USED           1
#define SQLITE_DBSTATUS_SCHEMA_USED          2
#define SQLITE_DBSTATUS_STMT_USED            3
#define SQLITE_DBSTATUS_LOOKASIDE_HIT        4
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE  5
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL  6
#define SQLITE_DBSTATUS_CACHE_HIT            7
#define SQLITE_DBSTATUS_CACHE_MISS           8
#define SQLITE_DBSTATUS_CACHE_WRITE          9
#define SQLITE_DBSTATUS_DEFERRED_FKS        10
#define SQLITE_DBSTATUS_CACHE_USED_SHARED   11
#define SQLITE_DBSTATUS_CACHE_SPILL         12
#define SQLITE_DBSTATUS_MAX                 12   /* Largest defined DBSTATUS */


/*
** CAPI3REF: Prepared Statement Status
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_stmt_status(sqlite3_stmt*, int op,int resetFlg);

/*
** CAPI3REF: Status Parameters for prepared statements
** KEYWORDS: {SQLITE_STMTSTATUS counter} {SQLITE_STMTSTATUS counters}
*/
#define SQLITE_STMTSTATUS_FULLSCAN_STEP     1
#define SQLITE_STMTSTATUS_SORT              2
#define SQLITE_STMTSTATUS_AUTOINDEX         3
#define SQLITE_STMTSTATUS_VM_STEP           4
#define SQLITE_STMTSTATUS_REPREPARE         5
#define SQLITE_STMTSTATUS_RUN               6
#define SQLITE_STMTSTATUS_MEMUSED           99

/*
** CAPI3REF: Custom Page Cache Object
*/
typedef struct sqlite3_pcache sqlite3_pcache;

/*
** CAPI3REF: Custom Page Cache Object
**
** The sqlite3_pcache_page object represents a single page in the
** page cache.  The page cache will allocate instances of this
** object.  Various methods of the page cache use pointers to instances
** of this object as parameters or as their return value.
**
** See [sqlite3_pcache_methods2] for additional information.
*/
typedef struct sqlite3_pcache_page sqlite3_pcache_page;
struct sqlite3_pcache_page {
  void *pBuf;        /* The content of the page */
  void *pExtra;      /* Extra information associated with the page */
};

/*
** CAPI3REF: Application Defined Page Cache.
** KEYWORDS: {page cache}
*/
typedef struct sqlite3_pcache_methods2 sqlite3_pcache_methods2;
struct sqlite3_pcache_methods2 {
  int iVersion;
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  sqlite3_pcache *(*xCreate)(int szPage, int szExtra, int bPurgeable);
  void (*xCachesize)(sqlite3_pcache*, int nCachesize);
  int (*xPagecount)(sqlite3_pcache*);
  sqlite3_pcache_page *(*xFetch)(sqlite3_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(sqlite3_pcache*, sqlite3_pcache_page*, int discard);
  void (*xRekey)(sqlite3_pcache*, sqlite3_pcache_page*,
      unsigned oldKey, unsigned newKey);
  void (*xTruncate)(sqlite3_pcache*, unsigned iLimit);
  void (*xDestroy)(sqlite3_pcache*);
  void (*xShrink)(sqlite3_pcache*);
};

/*
** This is the obsolete pcache_methods object that has now been replaced
** by sqlite3_pcache_methods2.  This object is not used by SQLite.  It is
** retained in the header file for backwards compatibility only.
*/
typedef struct sqlite3_pcache_methods sqlite3_pcache_methods;
struct sqlite3_pcache_methods {
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  sqlite3_pcache *(*xCreate)(int szPage, int bPurgeable);
  void (*xCachesize)(sqlite3_pcache*, int nCachesize);
  int (*xPagecount)(sqlite3_pcache*);
  void *(*xFetch)(sqlite3_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(sqlite3_pcache*, void*, int discard);
  void (*xRekey)(sqlite3_pcache*, void*, unsigned oldKey, unsigned newKey);
  void (*xTruncate)(sqlite3_pcache*, unsigned iLimit);
  void (*xDestroy)(sqlite3_pcache*);
};


/*
** CAPI3REF: Online Backup Object
** See Also: [Using the SQLite Online Backup API]
*/
typedef struct sqlite3_backup sqlite3_backup;

/*
** CAPI3REF: Online Backup API.
*/
SQLITE_API sqlite3_backup *sqlite3_backup_init(
  sqlite3 *pDest,                        /* Destination database handle */
  const char *zDestName,                 /* Destination database name */
  sqlite3 *pSource,                      /* Source database handle */
  const char *zSourceName                /* Source database name */
);
SQLITE_API int sqlite3_backup_step(sqlite3_backup *p, int nPage);
SQLITE_API int sqlite3_backup_finish(sqlite3_backup *p);
SQLITE_API int sqlite3_backup_remaining(sqlite3_backup *p);
SQLITE_API int sqlite3_backup_pagecount(sqlite3_backup *p);

/*
** CAPI3REF: Unlock Notification
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_unlock_notify(
  sqlite3 *pBlocked,                          /* Waiting connection */
  void (*xNotify)(void **apArg, int nArg),    /* Callback function to invoke */
  void *pNotifyArg                            /* Argument to pass to xNotify */
);


/*
** CAPI3REF: String Comparison
**
** ^The [sqlite3_stricmp()] and [sqlite3_strnicmp()] APIs allow applications
** and extensions to compare the contents of two buffers containing UTF-8
** strings in a case-independent fashion, using the same definition of "case
** independence" that SQLite uses internally when comparing identifiers.
*/
SQLITE_API int sqlite3_stricmp(const char *, const char *);
SQLITE_API int sqlite3_strnicmp(const char *, const char *, int);

/*
** CAPI3REF: String Globbing
*/
SQLITE_API int sqlite3_strglob(const char *zGlob, const char *zStr);

/*
** CAPI3REF: String LIKE Matching
*/
SQLITE_API int sqlite3_strlike(const char *zGlob, const char *zStr, unsigned int cEsc);

/*
** CAPI3REF: Error Logging Interface
*/
SQLITE_API void sqlite3_log(int iErrCode, const char *zFormat, ...);

/*
** CAPI3REF: Write-Ahead Log Commit Hook
** METHOD: sqlite3
*/
SQLITE_API void *sqlite3_wal_hook(
  sqlite3*,
  int(*)(void *,sqlite3*,const char*,int),
  void*
);

/*
** CAPI3REF: Configure an auto-checkpoint
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_wal_autocheckpoint(sqlite3 *db, int N);

/*
** CAPI3REF: Checkpoint a database
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_wal_checkpoint(sqlite3 *db, const char *zDb);

/*
** CAPI3REF: Checkpoint a database
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_wal_checkpoint_v2(
  sqlite3 *db,                    /* Database handle */
  const char *zDb,                /* Name of attached database (or NULL) */
  int eMode,                      /* SQLITE_CHECKPOINT_* value */
  int *pnLog,                     /* OUT: Size of WAL log in frames */
  int *pnCkpt                     /* OUT: Total number of frames checkpointed */
);

/*
** CAPI3REF: Checkpoint Mode Values
** KEYWORDS: {checkpoint mode}
*/
#define SQLITE_CHECKPOINT_PASSIVE  0  /* Do as much as possible w/o blocking */
#define SQLITE_CHECKPOINT_FULL     1  /* Wait for writers, then checkpoint */
#define SQLITE_CHECKPOINT_RESTART  2  /* Like FULL but wait for for readers */
#define SQLITE_CHECKPOINT_TRUNCATE 3  /* Like RESTART but also truncate WAL */

/*
** CAPI3REF: Virtual Table Interface Configuration
*/
SQLITE_API int sqlite3_vtab_config(sqlite3*, int op, ...);

/*
** CAPI3REF: Virtual Table Configuration Options
** KEYWORDS: {virtual table configuration options}
** KEYWORDS: {virtual table configuration option}
*/
#define SQLITE_VTAB_CONSTRAINT_SUPPORT 1
#define SQLITE_VTAB_INNOCUOUS          2
#define SQLITE_VTAB_DIRECTONLY         3

/*
** CAPI3REF: Determine The Virtual Table Conflict Policy
*/
SQLITE_API int sqlite3_vtab_on_conflict(sqlite3 *);

/*
** CAPI3REF: Determine If Virtual Table Column Access Is For UPDATE
*/
SQLITE_API int sqlite3_vtab_nochange(sqlite3_context*);

/*
** CAPI3REF: Determine The Collation For a Virtual Table Constraint
*/
SQLITE_API SQLITE_EXPERIMENTAL const char *sqlite3_vtab_collation(sqlite3_index_info*,int);

/*
** CAPI3REF: Conflict resolution modes
** KEYWORDS: {conflict resolution mode}
*/
#define SQLITE_ROLLBACK 1
/* #define SQLITE_IGNORE 2 // Also used by sqlite3_authorizer() callback */
#define SQLITE_FAIL     3
/* #define SQLITE_ABORT 4  // Also an error code */
#define SQLITE_REPLACE  5

/*
** CAPI3REF: Prepared Statement Scan Status Opcodes
** KEYWORDS: {scanstatus options}
*/
#define SQLITE_SCANSTAT_NLOOP    0
#define SQLITE_SCANSTAT_NVISIT   1
#define SQLITE_SCANSTAT_EST      2
#define SQLITE_SCANSTAT_NAME     3
#define SQLITE_SCANSTAT_EXPLAIN  4
#define SQLITE_SCANSTAT_SELECTID 5

/*
** CAPI3REF: Prepared Statement Scan Status
** METHOD: sqlite3_stmt
*/
SQLITE_API int sqlite3_stmt_scanstatus(
  sqlite3_stmt *pStmt,      /* Prepared statement for which info desired */
  int idx,                  /* Index of loop to report on */
  int iScanStatusOp,        /* Information desired.  SQLITE_SCANSTAT_* */
  void *pOut                /* Result written here */
);

/*
** CAPI3REF: Zero Scan-Status Counters
** METHOD: sqlite3_stmt
**
** ^Zero all [sqlite3_stmt_scanstatus()] related event counters.
**
** This API is only available if the library is built with pre-processor
** symbol [SQLITE_ENABLE_STMT_SCANSTATUS] defined.
*/
SQLITE_API void sqlite3_stmt_scanstatus_reset(sqlite3_stmt*);

/*
** CAPI3REF: Flush caches to disk mid-transaction
** METHOD: sqlite3.
*/
SQLITE_API int sqlite3_db_cacheflush(sqlite3*);

/*
** CAPI3REF: The pre-update hook.
** METHOD: sqlite3
*/
#if defined(SQLITE_ENABLE_PREUPDATE_HOOK)
SQLITE_API void *sqlite3_preupdate_hook(
  sqlite3 *db,
  void(*xPreUpdate)(
    void *pCtx,                   /* Copy of third arg to preupdate_hook() */
    sqlite3 *db,                  /* Database handle */
    int op,                       /* SQLITE_UPDATE, DELETE or INSERT */
    char const *zDb,              /* Database name */
    char const *zName,            /* Table name */
    sqlite3_int64 iKey1,          /* Rowid of row about to be deleted/updated */
    sqlite3_int64 iKey2           /* New rowid value (for a rowid UPDATE) */
  ),
  void*
);
SQLITE_API int sqlite3_preupdate_old(sqlite3 *, int, sqlite3_value **);
SQLITE_API int sqlite3_preupdate_count(sqlite3 *);
SQLITE_API int sqlite3_preupdate_depth(sqlite3 *);
SQLITE_API int sqlite3_preupdate_new(sqlite3 *, int, sqlite3_value **);
SQLITE_API int sqlite3_preupdate_blobwrite(sqlite3 *);
#endif

/*
** CAPI3REF: Low-level system error code
** METHOD: sqlite3
*/
SQLITE_API int sqlite3_system_errno(sqlite3*);

/*
** CAPI3REF: Database Snapshot
** KEYWORDS: {snapshot} {sqlite3_snapshot}
*/
typedef struct sqlite3_snapshot {
  unsigned char hidden[48];
} sqlite3_snapshot;

/*
** CAPI3REF: Record A Database Snapshot
** CONSTRUCTOR: sqlite3_snapshot
*/
SQLITE_API SQLITE_EXPERIMENTAL int sqlite3_snapshot_get(
  sqlite3 *db,
  const char *zSchema,
  sqlite3_snapshot **ppSnapshot
);

/*
** CAPI3REF: Start a read transaction on an historical snapshot
** METHOD: sqlite3_snapshot
*/
SQLITE_API SQLITE_EXPERIMENTAL int sqlite3_snapshot_open(
  sqlite3 *db,
  const char *zSchema,
  sqlite3_snapshot *pSnapshot
);

/*
** CAPI3REF: Destroy a snapshot
** DESTRUCTOR: sqlite3_snapshot
*/
SQLITE_API SQLITE_EXPERIMENTAL void sqlite3_snapshot_free(sqlite3_snapshot*);

/*
** CAPI3REF: Compare the ages of two snapshot handles.
** METHOD: sqlite3_snapshot
*/
SQLITE_API SQLITE_EXPERIMENTAL int sqlite3_snapshot_cmp(
  sqlite3_snapshot *p1,
  sqlite3_snapshot *p2
);

/*
** CAPI3REF: Recover snapshots from a wal file
** METHOD: sqlite3_snapshot
*/
SQLITE_API SQLITE_EXPERIMENTAL int sqlite3_snapshot_recover(sqlite3 *db, const char *zDb);

/*
** CAPI3REF: Serialize a database
*/
SQLITE_API unsigned char *sqlite3_serialize(
  sqlite3 *db,           /* The database connection */
  const char *zSchema,   /* Which DB to serialize. ex: "main", "temp", ... */
  sqlite3_int64 *piSize, /* Write size of the DB here, if not NULL */
  unsigned int mFlags    /* Zero or more SQLITE_SERIALIZE_* flags */
);

/*
** CAPI3REF: Flags for sqlite3_serialize
*/
#define SQLITE_SERIALIZE_NOCOPY 0x001   /* Do no memory allocations */

/*
** CAPI3REF: Deserialize a database
*/
SQLITE_API int sqlite3_deserialize(
  sqlite3 *db,            /* The database connection */
  const char *zSchema,    /* Which DB to reopen with the deserialization */
  unsigned char *pData,   /* The serialized database content */
  sqlite3_int64 szDb,     /* Number bytes in the deserialization */
  sqlite3_int64 szBuf,    /* Total size of buffer pData[] */
  unsigned mFlags         /* Zero or more SQLITE_DESERIALIZE_* flags */
);

/*
** CAPI3REF: Flags for sqlite3_deserialize()
*/
#define SQLITE_DESERIALIZE_FREEONCLOSE 1 /* Call sqlite3_free() on close */
#define SQLITE_DESERIALIZE_RESIZEABLE  2 /* Resize using sqlite3_realloc64() */
#define SQLITE_DESERIALIZE_READONLY    4 /* Database is read-only */

/*
** Undo the hack that converts floating point types to integer for
** builds on processors without floating point support.
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# undef double
#endif

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
#endif /* SQLITE3_H */

/******** Begin file sqlite3rtree.h *********/
/*
** 2010 August 30
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
*/

#ifndef _SQLITE3RTREE_H_
#define _SQLITE3RTREE_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlite3_rtree_geometry sqlite3_rtree_geometry;
typedef struct sqlite3_rtree_query_info sqlite3_rtree_query_info;

/* The double-precision datatype used by RTree depends on the
** SQLITE_RTREE_INT_ONLY compile-time option.
*/
#ifdef SQLITE_RTREE_INT_ONLY
  typedef sqlite3_int64 sqlite3_rtree_dbl;
#else
  typedef double sqlite3_rtree_dbl;
#endif

/*
** Register a geometry callback named zGeom that can be used as part of an
** R-Tree geometry query as follows:
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zGeom(... params ...)
*/
SQLITE_API int sqlite3_rtree_geometry_callback(
  sqlite3 *db,
  const char *zGeom,
  int (*xGeom)(sqlite3_rtree_geometry*, int, sqlite3_rtree_dbl*,int*),
  void *pContext
);


/*
** A pointer to a structure of the following type is passed as the first
** argument to callbacks registered using rtree_geometry_callback().
*/
struct sqlite3_rtree_geometry {
  void *pContext;                 /* Copy of pContext passed to s_r_g_c() */
  int nParam;                     /* Size of array aParam[] */
  sqlite3_rtree_dbl *aParam;      /* Parameters passed to SQL geom function */
  void *pUser;                    /* Callback implementation user data */
  void (*xDelUser)(void *);       /* Called by SQLite to clean up pUser */
};

/*
** Register a 2nd-generation geometry callback named zScore that can be
** used as part of an R-Tree geometry query as follows:
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zQueryFunc(... params ...)
*/
SQLITE_API int sqlite3_rtree_query_callback(
  sqlite3 *db,
  const char *zQueryFunc,
  int (*xQueryFunc)(sqlite3_rtree_query_info*),
  void *pContext,
  void (*xDestructor)(void*)
);


/*
** A pointer to a structure of the following type is passed as the
** argument to scored geometry callback registered using
** sqlite3_rtree_query_callback().
**
** Note that the first 5 fields of this structure are identical to
** sqlite3_rtree_geometry.  This structure is a subclass of
** sqlite3_rtree_geometry.
*/
struct sqlite3_rtree_query_info {
  void *pContext;                   /* pContext from when function registered */
  int nParam;                       /* Number of function parameters */
  sqlite3_rtree_dbl *aParam;        /* value of function parameters */
  void *pUser;                      /* callback can use this, if desired */
  void (*xDelUser)(void*);          /* function to free pUser */
  sqlite3_rtree_dbl *aCoord;        /* Coordinates of node or entry to check */
  unsigned int *anQueue;            /* Number of pending entries in the queue */
  int nCoord;                       /* Number of coordinates */
  int iLevel;                       /* Level of current node or entry */
  int mxLevel;                      /* The largest iLevel value in the tree */
  sqlite3_int64 iRowid;             /* Rowid for current entry */
  sqlite3_rtree_dbl rParentScore;   /* Score of parent node */
  int eParentWithin;                /* Visibility of parent node */
  int eWithin;                      /* OUT: Visibility */
  sqlite3_rtree_dbl rScore;         /* OUT: Write the score here */
  /* The following fields are only available in 3.8.11 and later */
  sqlite3_value **apSqlParam;       /* Original SQL values of parameters */
};

/*
** Allowed values for sqlite3_rtree_query.eWithin and .eParentWithin.
*/
#define NOT_WITHIN       0   /* Object completely outside of query region */
#define PARTLY_WITHIN    1   /* Object partially overlaps query region */
#define FULLY_WITHIN     2   /* Object fully contained within query region */


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif  /* ifndef _SQLITE3RTREE_H_ */

/******** End of sqlite3rtree.h *********/
/******** Begin file sqlite3session.h *********/

#if !defined(__SQLITESESSION_H_) && defined(SQLITE_ENABLE_SESSION)
#define __SQLITESESSION_H_ 1

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


/*
** CAPI3REF: Session Object Handle
**
** An instance of this object is a [session] that can be used to
** record changes to a database.
*/
typedef struct sqlite3_session sqlite3_session;

/*
** CAPI3REF: Changeset Iterator Handle
**
** An instance of this object acts as a cursor for iterating
** over the elements of a [changeset] or [patchset].
*/
typedef struct sqlite3_changeset_iter sqlite3_changeset_iter;

/*
** CAPI3REF: Create A New Session Object
** CONSTRUCTOR: sqlite3_session
**
** Create a new session object attached to database handle db. If successful,
** a pointer to the new object is written to *ppSession and SQLITE_OK is
** returned. If an error occurs, *ppSession is set to NULL and an SQLite
** error code (e.g. SQLITE_NOMEM) is returned.
**
** It is possible to create multiple session objects attached to a single
** database handle.
**
** Session objects created using this function should be deleted using the
** [sqlite3session_delete()] function before the database handle that they
** are attached to is itself closed. If the database handle is closed before
** the session object is deleted, then the results of calling any session
** module function, including [sqlite3session_delete()] on the session object
** are undefined.
**
** Because the session module uses the [sqlite3_preupdate_hook()] API, it
** is not possible for an application to register a pre-update hook on a
** database handle that has one or more session objects attached. Nor is
** it possible to create a session object attached to a database handle for
** which a pre-update hook is already defined. The results of attempting
** either of these things are undefined.
**
** The session object will be used to create changesets for tables in
** database zDb, where zDb is either "main", or "temp", or the name of an
** attached database. It is not an error if database zDb is not attached
** to the database when the session object is created.
*/
SQLITE_API int sqlite3session_create(
  sqlite3 *db,                    /* Database handle */
  const char *zDb,                /* Name of db (e.g. "main") */
  sqlite3_session **ppSession     /* OUT: New session object */
);

/*
** CAPI3REF: Delete A Session Object
** DESTRUCTOR: sqlite3_session
**
** Delete a session object previously allocated using
** [sqlite3session_create()]. Once a session object has been deleted, the
** results of attempting to use pSession with any other session module
** function are undefined.
**
** Session objects must be deleted before the database handle to which they
** are attached is closed. Refer to the documentation for
** [sqlite3session_create()] for details.
*/
SQLITE_API void sqlite3session_delete(sqlite3_session *pSession);

/*
** CAPIREF: Conigure a Session Object
** METHOD: sqlite3_session
**
** This method is used to configure a session object after it has been
** created. At present the only valid value for the second parameter is
** [SQLITE_SESSION_OBJCONFIG_SIZE].
**
** Arguments for sqlite3session_object_config()
**
** The following values may passed as the the 4th parameter to
** sqlite3session_object_config().
**
** <dt>SQLITE_SESSION_OBJCONFIG_SIZE <dd>
**   This option is used to set, clear or query the flag that enables
**   the [sqlite3session_changeset_size()] API. Because it imposes some
**   computational overhead, this API is disabled by default. Argument
**   pArg must point to a value of type (int). If the value is initially
**   0, then the sqlite3session_changeset_size() API is disabled. If it
**   is greater than 0, then the same API is enabled. Or, if the initial
**   value is less than zero, no change is made. In all cases the (int)
**   variable is set to 1 if the sqlite3session_changeset_size() API is
**   enabled following the current call, or 0 otherwise.
**
**   It is an error (SQLITE_MISUSE) to attempt to modify this setting after
**   the first table has been attached to the session object.
*/
SQLITE_API int sqlite3session_object_config(sqlite3_session*, int op, void *pArg);

/*
*/
#define SQLITE_SESSION_OBJCONFIG_SIZE 1

/*
** CAPI3REF: Enable Or Disable A Session Object
** METHOD: sqlite3_session
**
** Enable or disable the recording of changes by a session object. When
** enabled, a session object records changes made to the database. When
** disabled - it does not. A newly created session object is enabled.
** Refer to the documentation for [sqlite3session_changeset()] for further
** details regarding how enabling and disabling a session object affects
** the eventual changesets.
**
** Passing zero to this function disables the session. Passing a value
** greater than zero enables it. Passing a value less than zero is a
** no-op, and may be used to query the current state of the session.
**
** The return value indicates the final state of the session object: 0 if
** the session is disabled, or 1 if it is enabled.
*/
SQLITE_API int sqlite3session_enable(sqlite3_session *pSession, int bEnable);

/*
** CAPI3REF: Set Or Clear the Indirect Change Flag
** METHOD: sqlite3_session
**
** Each change recorded by a session object is marked as either direct or
** indirect. A change is marked as indirect if either:
**
** <ul>
**   <li> The session object "indirect" flag is set when the change is
**        made, or
**   <li> The change is made by an SQL trigger or foreign key action
**        instead of directly as a result of a users SQL statement.
** </ul>
**
** If a single row is affected by more than one operation within a session,
** then the change is considered indirect if all operations meet the criteria
** for an indirect change above, or direct otherwise.
**
** This function is used to set, clear or query the session object indirect
** flag.  If the second argument passed to this function is zero, then the
** indirect flag is cleared. If it is greater than zero, the indirect flag
** is set. Passing a value less than zero does not modify the current value
** of the indirect flag, and may be used to query the current state of the
** indirect flag for the specified session object.
**
** The return value indicates the final state of the indirect flag: 0 if
** it is clear, or 1 if it is set.
*/
SQLITE_API int sqlite3session_indirect(sqlite3_session *pSession, int bIndirect);

/*
** CAPI3REF: Attach A Table To A Session Object
** METHOD: sqlite3_session
**
** If argument zTab is not NULL, then it is the name of a table to attach
** to the session object passed as the first argument. All subsequent changes
** made to the table while the session object is enabled will be recorded. See
** documentation for [sqlite3session_changeset()] for further details.
**
** Or, if argument zTab is NULL, then changes are recorded for all tables
** in the database. If additional tables are added to the database (by
** executing "CREATE TABLE" statements) after this call is made, changes for
** the new tables are also recorded.
**
** Changes can only be recorded for tables that have a PRIMARY KEY explicitly
** defined as part of their CREATE TABLE statement. It does not matter if the
** PRIMARY KEY is an "INTEGER PRIMARY KEY" (rowid alias) or not. The PRIMARY
** KEY may consist of a single column, or may be a composite key.
**
** It is not an error if the named table does not exist in the database. Nor
** is it an error if the named table does not have a PRIMARY KEY. However,
** no changes will be recorded in either of these scenarios.
**
** Changes are not recorded for individual rows that have NULL values stored
** in one or more of their PRIMARY KEY columns.
**
** SQLITE_OK is returned if the call completes without error. Or, if an error
** occurs, an SQLite error code (e.g. SQLITE_NOMEM) is returned.
**
** <h3>Special sqlite_stat1 Handling</h3>
**
** As of SQLite version 3.22.0, the "sqlite_stat1" table is an exception to
** some of the rules above. In SQLite, the schema of sqlite_stat1 is:
**  <pre>
**  &nbsp;     CREATE TABLE sqlite_stat1(tbl,idx,stat)
**  </pre>
**
** Even though sqlite_stat1 does not have a PRIMARY KEY, changes are
** recorded for it as if the PRIMARY KEY is (tbl,idx). Additionally, changes
** are recorded for rows for which (idx IS NULL) is true. However, for such
** rows a zero-length blob (SQL value X'') is stored in the changeset or
** patchset instead of a NULL value. This allows such changesets to be
** manipulated by legacy implementations of sqlite3changeset_invert(),
** concat() and similar.
**
** The sqlite3changeset_apply() function automatically converts the
** zero-length blob back to a NULL value when updating the sqlite_stat1
** table. However, if the application calls sqlite3changeset_new(),
** sqlite3changeset_old() or sqlite3changeset_conflict on a changeset
** iterator directly (including on a changeset iterator passed to a
** conflict-handler callback) then the X'' value is returned. The application
** must translate X'' to NULL itself if required.
**
** Legacy (older than 3.22.0) versions of the sessions module cannot capture
** changes made to the sqlite_stat1 table. Legacy versions of the
** sqlite3changeset_apply() function silently ignore any modifications to the
** sqlite_stat1 table that are part of a changeset or patchset.
*/
SQLITE_API int sqlite3session_attach(
  sqlite3_session *pSession,      /* Session object */
  const char *zTab                /* Table name */
);

/*
** CAPI3REF: Set a table filter on a Session Object.
** METHOD: sqlite3_session
**
** The second argument (xFilter) is the "filter callback". For changes to rows
** in tables that are not attached to the Session object, the filter is called
** to determine whether changes to the table's rows should be tracked or not.
** If xFilter returns 0, changes are not tracked. Note that once a table is
** attached, xFilter will not be called again.
*/
SQLITE_API void sqlite3session_table_filter(
  sqlite3_session *pSession,      /* Session object */
  int(*xFilter)(
    void *pCtx,                   /* Copy of third arg to _filter_table() */
    const char *zTab              /* Table name */
  ),
  void *pCtx                      /* First argument passed to xFilter */
);

/*
** CAPI3REF: Generate A Changeset From A Session Object
** METHOD: sqlite3_session
**
** Obtain a changeset containing changes to the tables attached to the
** session object passed as the first argument. If successful,
** set *ppChangeset to point to a buffer containing the changeset
** and *pnChangeset to the size of the changeset in bytes before returning
** SQLITE_OK. If an error occurs, set both *ppChangeset and *pnChangeset to
** zero and return an SQLite error code.
**
** A changeset consists of zero or more INSERT, UPDATE and/or DELETE changes,
** each representing a change to a single row of an attached table. An INSERT
** change contains the values of each field of a new database row. A DELETE
** contains the original values of each field of a deleted database row. An
** UPDATE change contains the original values of each field of an updated
** database row along with the updated values for each updated non-primary-key
** column. It is not possible for an UPDATE change to represent a change that
** modifies the values of primary key columns. If such a change is made, it
** is represented in a changeset as a DELETE followed by an INSERT.
**
** Changes are not recorded for rows that have NULL values stored in one or
** more of their PRIMARY KEY columns. If such a row is inserted or deleted,
** no corresponding change is present in the changesets returned by this
** function. If an existing row with one or more NULL values stored in
** PRIMARY KEY columns is updated so that all PRIMARY KEY columns are non-NULL,
** only an INSERT is appears in the changeset. Similarly, if an existing row
** with non-NULL PRIMARY KEY values is updated so that one or more of its
** PRIMARY KEY columns are set to NULL, the resulting changeset contains a
** DELETE change only.
**
** The contents of a changeset may be traversed using an iterator created
** using the [sqlite3changeset_start()] API. A changeset may be applied to
** a database with a compatible schema using the [sqlite3changeset_apply()]
** API.
**
** Within a changeset generated by this function, all changes related to a
** single table are grouped together. In other words, when iterating through
** a changeset or when applying a changeset to a database, all changes related
** to a single table are processed before moving on to the next table. Tables
** are sorted in the same order in which they were attached (or auto-attached)
** to the sqlite3_session object. The order in which the changes related to
** a single table are stored is undefined.
**
** Following a successful call to this function, it is the responsibility of
** the caller to eventually free the buffer that *ppChangeset points to using
** [sqlite3_free()].
**
** <h3>Changeset Generation</h3>
**
** Once a table has been attached to a session object, the session object
** records the primary key values of all new rows inserted into the table.
** It also records the original primary key and other column values of any
** deleted or updated rows. For each unique primary key value, data is only
** recorded once - the first time a row with said primary key is inserted,
** updated or deleted in the lifetime of the session.
**
** There is one exception to the previous paragraph: when a row is inserted,
** updated or deleted, if one or more of its primary key columns contain a
** NULL value, no record of the change is made.
**
** The session object therefore accumulates two types of records - those
** that consist of primary key values only (created when the user inserts
** a new record) and those that consist of the primary key values and the
** original values of other table columns (created when the users deletes
** or updates a record).
**
** When this function is called, the requested changeset is created using
** both the accumulated records and the current contents of the database
** file. Specifically:
**
** <ul>
**   <li> For each record generated by an insert, the database is queried
**        for a row with a matching primary key. If one is found, an INSERT
**        change is added to the changeset. If no such row is found, no change
**        is added to the changeset.
**
**   <li> For each record generated by an update or delete, the database is
**        queried for a row with a matching primary key. If such a row is
**        found and one or more of the non-primary key fields have been
**        modified from their original values, an UPDATE change is added to
**        the changeset. Or, if no such row is found in the table, a DELETE
**        change is added to the changeset. If there is a row with a matching
**        primary key in the database, but all fields contain their original
**        values, no change is added to the changeset.
** </ul>
**
** This means, amongst other things, that if a row is inserted and then later
** deleted while a session object is active, neither the insert nor the delete
** will be present in the changeset. Or if a row is deleted and then later a
** row with the same primary key values inserted while a session object is
** active, the resulting changeset will contain an UPDATE change instead of
** a DELETE and an INSERT.
**
** When a session object is disabled (see the [sqlite3session_enable()] API),
** it does not accumulate records when rows are inserted, updated or deleted.
** This may appear to have some counter-intuitive effects if a single row
** is written to more than once during a session. For example, if a row
** is inserted while a session object is enabled, then later deleted while
** the same session object is disabled, no INSERT record will appear in the
** changeset, even though the delete took place while the session was disabled.
** Or, if one field of a row is updated while a session is disabled, and
** another field of the same row is updated while the session is enabled, the
** resulting changeset will contain an UPDATE change that updates both fields.
*/
SQLITE_API int sqlite3session_changeset(
  sqlite3_session *pSession,      /* Session object */
  int *pnChangeset,               /* OUT: Size of buffer at *ppChangeset */
  void **ppChangeset              /* OUT: Buffer containing changeset */
);

/*
** CAPI3REF: Return An Upper-limit For The Size Of The Changeset
** METHOD: sqlite3_session
**
** By default, this function always returns 0. For it to return
** a useful result, the sqlite3_session object must have been configured
** to enable this API using sqlite3session_object_config() with the
** SQLITE_SESSION_OBJCONFIG_SIZE verb.
**
** When enabled, this function returns an upper limit, in bytes, for the size
** of the changeset that might be produced if sqlite3session_changeset() were
** called. The final changeset size might be equal to or smaller than the
** size in bytes returned by this function.
*/
SQLITE_API sqlite3_int64 sqlite3session_changeset_size(sqlite3_session *pSession);

/*
** CAPI3REF: Load The Difference Between Tables Into A Session
** METHOD: sqlite3_session
**
** If it is not already attached to the session object passed as the first
** argument, this function attaches table zTbl in the same manner as the
** [sqlite3session_attach()] function. If zTbl does not exist, or if it
** does not have a primary key, this function is a no-op (but does not return
** an error).
**
** Argument zFromDb must be the name of a database ("main", "temp" etc.)
** attached to the same database handle as the session object that contains
** a table compatible with the table attached to the session by this function.
** A table is considered compatible if it:
**
** <ul>
**   <li> Has the same name,
**   <li> Has the same set of columns declared in the same order, and
**   <li> Has the same PRIMARY KEY definition.
** </ul>
**
** If the tables are not compatible, SQLITE_SCHEMA is returned. If the tables
** are compatible but do not have any PRIMARY KEY columns, it is not an error
** but no changes are added to the session object. As with other session
** APIs, tables without PRIMARY KEYs are simply ignored.
**
** This function adds a set of changes to the session object that could be
** used to update the table in database zFrom (call this the "from-table")
** so that its content is the same as the table attached to the session
** object (call this the "to-table"). Specifically:
**
** <ul>
**   <li> For each row (primary key) that exists in the to-table but not in
**     the from-table, an INSERT record is added to the session object.
**
**   <li> For each row (primary key) that exists in the to-table but not in
**     the from-table, a DELETE record is added to the session object.
**
**   <li> For each row (primary key) that exists in both tables, but features
**     different non-PK values in each, an UPDATE record is added to the
**     session.
** </ul>
**
** To clarify, if this function is called and then a changeset constructed
** using [sqlite3session_changeset()], then after applying that changeset to
** database zFrom the contents of the two compatible tables would be
** identical.
**
** It an error if database zFrom does not exist or does not contain the
** required compatible table.
**
** If the operation is successful, SQLITE_OK is returned. Otherwise, an SQLite
** error code. In this case, if argument pzErrMsg is not NULL, *pzErrMsg
** may be set to point to a buffer containing an English language error
** message. It is the responsibility of the caller to free this buffer using
** sqlite3_free().
*/
SQLITE_API int sqlite3session_diff(
  sqlite3_session *pSession,
  const char *zFromDb,
  const char *zTbl,
  char **pzErrMsg
);


/*
** CAPI3REF: Generate A Patchset From A Session Object
** METHOD: sqlite3_session
**
** The differences between a patchset and a changeset are that:
**
** <ul>
**   <li> DELETE records consist of the primary key fields only. The
**        original values of other fields are omitted.
**   <li> The original values of any modified fields are omitted from
**        UPDATE records.
** </ul>
**
** A patchset blob may be used with up to date versions of all
** sqlite3changeset_xxx API functions except for sqlite3changeset_invert(),
** which returns SQLITE_CORRUPT if it is passed a patchset. Similarly,
** attempting to use a patchset blob with old versions of the
** sqlite3changeset_xxx APIs also provokes an SQLITE_CORRUPT error.
**
** Because the non-primary key "old.*" fields are omitted, no
** SQLITE_CHANGESET_DATA conflicts can be detected or reported if a patchset
** is passed to the sqlite3changeset_apply() API. Other conflict types work
** in the same way as for changesets.
**
** Changes within a patchset are ordered in the same way as for changesets
** generated by the sqlite3session_changeset() function (i.e. all changes for
** a single table are grouped together, tables appear in the order in which
** they were attached to the session object).
*/
SQLITE_API int sqlite3session_patchset(
  sqlite3_session *pSession,      /* Session object */
  int *pnPatchset,                /* OUT: Size of buffer at *ppPatchset */
  void **ppPatchset               /* OUT: Buffer containing patchset */
);

/*
** CAPI3REF: Test if a changeset has recorded any changes.
**
** Return non-zero if no changes to attached tables have been recorded by
** the session object passed as the first argument. Otherwise, if one or
** more changes have been recorded, return zero.
**
** Even if this function returns zero, it is possible that calling
** [sqlite3session_changeset()] on the session handle may still return a
** changeset that contains no changes. This can happen when a row in
** an attached table is modified and then later on the original values
** are restored. However, if this function returns non-zero, then it is
** guaranteed that a call to sqlite3session_changeset() will return a
** changeset containing zero changes.
*/
SQLITE_API int sqlite3session_isempty(sqlite3_session *pSession);

/*
** CAPI3REF: Query for the amount of heap memory used by a session object.
**
** This API returns the total amount of heap memory in bytes currently
** used by the session object passed as the only argument.
*/
SQLITE_API sqlite3_int64 sqlite3session_memory_used(sqlite3_session *pSession);

/*
** CAPI3REF: Create An Iterator To Traverse A Changeset
** CONSTRUCTOR: sqlite3_changeset_iter
**
** Create an iterator used to iterate through the contents of a changeset.
** If successful, *pp is set to point to the iterator handle and SQLITE_OK
** is returned. Otherwise, if an error occurs, *pp is set to zero and an
** SQLite error code is returned.
**
** The following functions can be used to advance and query a changeset
** iterator created by this function:
**
** <ul>
**   <li> [sqlite3changeset_next()]
**   <li> [sqlite3changeset_op()]
**   <li> [sqlite3changeset_new()]
**   <li> [sqlite3changeset_old()]
** </ul>
**
** It is the responsibility of the caller to eventually destroy the iterator
** by passing it to [sqlite3changeset_finalize()]. The buffer containing the
** changeset (pChangeset) must remain valid until after the iterator is
** destroyed.
**
** Assuming the changeset blob was created by one of the
** [sqlite3session_changeset()], [sqlite3changeset_concat()] or
** [sqlite3changeset_invert()] functions, all changes within the changeset
** that apply to a single table are grouped together. This means that when
** an application iterates through a changeset using an iterator created by
** this function, all changes that relate to a single table are visited
** consecutively. There is no chance that the iterator will visit a change
** the applies to table X, then one for table Y, and then later on visit
** another change for table X.
**
** The behavior of sqlite3changeset_start_v2() and its streaming equivalent
** may be modified by passing a combination of
** [SQLITE_CHANGESETSTART_INVERT | supported flags] as the 4th parameter.
**
** Note that the sqlite3changeset_start_v2() API is still <b>experimental</b>
** and therefore subject to change.
*/
SQLITE_API int sqlite3changeset_start(
  sqlite3_changeset_iter **pp,    /* OUT: New changeset iterator handle */
  int nChangeset,                 /* Size of changeset blob in bytes */
  void *pChangeset                /* Pointer to blob containing changeset */
);
SQLITE_API int sqlite3changeset_start_v2(
  sqlite3_changeset_iter **pp,    /* OUT: New changeset iterator handle */
  int nChangeset,                 /* Size of changeset blob in bytes */
  void *pChangeset,               /* Pointer to blob containing changeset */
  int flags                       /* SESSION_CHANGESETSTART_* flags */
);

/*
** CAPI3REF: Flags for sqlite3changeset_start_v2
**
** The following flags may passed via the 4th parameter to
** [sqlite3changeset_start_v2] and [sqlite3changeset_start_v2_strm]:
**
** <dt>SQLITE_CHANGESETAPPLY_INVERT <dd>
**   Invert the changeset while iterating through it. This is equivalent to
**   inverting a changeset using sqlite3changeset_invert() before applying it.
**   It is an error to specify this flag with a patchset.
*/
#define SQLITE_CHANGESETSTART_INVERT        0x0002


/*
** CAPI3REF: Advance A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** This function may only be used with iterators created by the function
** [sqlite3changeset_start()]. If it is called on an iterator passed to
** a conflict-handler callback by [sqlite3changeset_apply()], SQLITE_MISUSE
** is returned and the call has no effect.
**
** Immediately after an iterator is created by sqlite3changeset_start(), it
** does not point to any change in the changeset. Assuming the changeset
** is not empty, the first call to this function advances the iterator to
** point to the first change in the changeset. Each subsequent call advances
** the iterator to point to the next change in the changeset (if any). If
** no error occurs and the iterator points to a valid change after a call
** to sqlite3changeset_next() has advanced it, SQLITE_ROW is returned.
** Otherwise, if all changes in the changeset have already been visited,
** SQLITE_DONE is returned.
**
** If an error occurs, an SQLite error code is returned. Possible error
** codes include SQLITE_CORRUPT (if the changeset buffer is corrupt) or
** SQLITE_NOMEM.
*/
SQLITE_API int sqlite3changeset_next(sqlite3_changeset_iter *pIter);

/*
** CAPI3REF: Obtain The Current Operation From A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [sqlite3changeset_apply()], or an iterator
** created by [sqlite3changeset_start()]. In the latter case, the most recent
** call to [sqlite3changeset_next()] must have returned [SQLITE_ROW]. If this
** is not the case, this function returns [SQLITE_MISUSE].
**
** Arguments pOp, pnCol and pzTab may not be NULL. Upon return, three
** outputs are set through these pointers:
**
** *pOp is set to one of [SQLITE_INSERT], [SQLITE_DELETE] or [SQLITE_UPDATE],
** depending on the type of change that the iterator currently points to;
**
** *pnCol is set to the number of columns in the table affected by the change; and
**
** *pzTab is set to point to a nul-terminated utf-8 encoded string containing
** the name of the table affected by the current change. The buffer remains
** valid until either sqlite3changeset_next() is called on the iterator
** or until the conflict-handler function returns.
**
** If pbIndirect is not NULL, then *pbIndirect is set to true (1) if the change
** is an indirect change, or false (0) otherwise. See the documentation for
** [sqlite3session_indirect()] for a description of direct and indirect
** changes.
**
** If no error occurs, SQLITE_OK is returned. If an error does occur, an
** SQLite error code is returned. The values of the output variables may not
** be trusted in this case.
*/
SQLITE_API int sqlite3changeset_op(
  sqlite3_changeset_iter *pIter,  /* Iterator object */
  const char **pzTab,             /* OUT: Pointer to table name */
  int *pnCol,                     /* OUT: Number of columns in table */
  int *pOp,                       /* OUT: SQLITE_INSERT, DELETE or UPDATE */
  int *pbIndirect                 /* OUT: True for an 'indirect' change */
);

/*
** CAPI3REF: Obtain The Primary Key Definition Of A Table
** METHOD: sqlite3_changeset_iter
**
** For each modified table, a changeset includes the following:
**
** <ul>
**   <li> The number of columns in the table, and
**   <li> Which of those columns make up the tables PRIMARY KEY.
** </ul>
**
** This function is used to find which columns comprise the PRIMARY KEY of
** the table modified by the change that iterator pIter currently points to.
** If successful, *pabPK is set to point to an array of nCol entries, where
** nCol is the number of columns in the table. Elements of *pabPK are set to
** 0x01 if the corresponding column is part of the tables primary key, or
** 0x00 if it is not.
**
** If argument pnCol is not NULL, then *pnCol is set to the number of columns
** in the table.
**
** If this function is called when the iterator does not point to a valid
** entry, SQLITE_MISUSE is returned and the output variables zeroed. Otherwise,
** SQLITE_OK is returned and the output variables populated as described
** above.
*/
SQLITE_API int sqlite3changeset_pk(
  sqlite3_changeset_iter *pIter,  /* Iterator object */
  unsigned char **pabPK,          /* OUT: Array of boolean - true for PK cols */
  int *pnCol                      /* OUT: Number of entries in output array */
);

/*
** CAPI3REF: Obtain old.* Values From A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [sqlite3changeset_apply()], or an iterator
** created by [sqlite3changeset_start()]. In the latter case, the most recent
** call to [sqlite3changeset_next()] must have returned SQLITE_ROW.
** Furthermore, it may only be called if the type of change that the iterator
** currently points to is either [SQLITE_DELETE] or [SQLITE_UPDATE]. Otherwise,
** this function returns [SQLITE_MISUSE] and sets *ppValue to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [SQLITE_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** sqlite3_value object containing the iVal'th value from the vector of
** original row values stored as part of the UPDATE or DELETE change and
** returns SQLITE_OK. The name of the function comes from the fact that this
** is similar to the "old.*" columns available to update or delete triggers.
**
** If some other error occurs (e.g. an OOM condition), an SQLite error code
** is returned and *ppValue is set to NULL.
*/
SQLITE_API int sqlite3changeset_old(
  sqlite3_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  sqlite3_value **ppValue         /* OUT: Old value (or NULL pointer) */
);

/*
** CAPI3REF: Obtain new.* Values From A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** The pIter argument passed to this function may either be an iterator
** passed to a conflict-handler by [sqlite3changeset_apply()], or an iterator
** created by [sqlite3changeset_start()]. In the latter case, the most recent
** call to [sqlite3changeset_next()] must have returned SQLITE_ROW.
** Furthermore, it may only be called if the type of change that the iterator
** currently points to is either [SQLITE_UPDATE] or [SQLITE_INSERT]. Otherwise,
** this function returns [SQLITE_MISUSE] and sets *ppValue to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [SQLITE_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** sqlite3_value object containing the iVal'th value from the vector of
** new row values stored as part of the UPDATE or INSERT change and
** returns SQLITE_OK. If the change is an UPDATE and does not include
** a new value for the requested column, *ppValue is set to NULL and
** SQLITE_OK returned. The name of the function comes from the fact that
** this is similar to the "new.*" columns available to update or delete
** triggers.
**
** If some other error occurs (e.g. an OOM condition), an SQLite error code
** is returned and *ppValue is set to NULL.
*/
SQLITE_API int sqlite3changeset_new(
  sqlite3_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  sqlite3_value **ppValue         /* OUT: New value (or NULL pointer) */
);

/*
** CAPI3REF: Obtain Conflicting Row Values From A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** This function should only be used with iterator objects passed to a
** conflict-handler callback by [sqlite3changeset_apply()] with either
** [SQLITE_CHANGESET_DATA] or [SQLITE_CHANGESET_CONFLICT]. If this function
** is called on any other iterator, [SQLITE_MISUSE] is returned and *ppValue
** is set to NULL.
**
** Argument iVal must be greater than or equal to 0, and less than the number
** of columns in the table affected by the current change. Otherwise,
** [SQLITE_RANGE] is returned and *ppValue is set to NULL.
**
** If successful, this function sets *ppValue to point to a protected
** sqlite3_value object containing the iVal'th value from the
** "conflicting row" associated with the current conflict-handler callback
** and returns SQLITE_OK.
**
** If some other error occurs (e.g. an OOM condition), an SQLite error code
** is returned and *ppValue is set to NULL.
*/
SQLITE_API int sqlite3changeset_conflict(
  sqlite3_changeset_iter *pIter,  /* Changeset iterator */
  int iVal,                       /* Column number */
  sqlite3_value **ppValue         /* OUT: Value from conflicting row */
);

/*
** CAPI3REF: Determine The Number Of Foreign Key Constraint Violations
** METHOD: sqlite3_changeset_iter
**
** This function may only be called with an iterator passed to an
** SQLITE_CHANGESET_FOREIGN_KEY conflict handler callback. In this case
** it sets the output variable to the total number of known foreign key
** violations in the destination database and returns SQLITE_OK.
**
** In all other cases this function returns SQLITE_MISUSE.
*/
SQLITE_API int sqlite3changeset_fk_conflicts(
  sqlite3_changeset_iter *pIter,  /* Changeset iterator */
  int *pnOut                      /* OUT: Number of FK violations */
);


/*
** CAPI3REF: Finalize A Changeset Iterator
** METHOD: sqlite3_changeset_iter
**
** This function is used to finalize an iterator allocated with
** [sqlite3changeset_start()].
**
** This function should only be called on iterators created using the
** [sqlite3changeset_start()] function. If an application calls this
** function with an iterator passed to a conflict-handler by
** [sqlite3changeset_apply()], [SQLITE_MISUSE] is immediately returned and the
** call has no effect.
**
** If an error was encountered within a call to an sqlite3changeset_xxx()
** function (for example an [SQLITE_CORRUPT] in [sqlite3changeset_next()] or an
** [SQLITE_NOMEM] in [sqlite3changeset_new()]) then an error code corresponding
** to that error is returned by this function. Otherwise, SQLITE_OK is
** returned. This is to allow the following pattern (pseudo-code):
**
** <pre>
**   sqlite3changeset_start();
**   while( SQLITE_ROW==sqlite3changeset_next() ){
**     // Do something with change.
**   }
**   rc = sqlite3changeset_finalize();
**   if( rc!=SQLITE_OK ){
**     // An error has occurred
**   }
** </pre>
*/
SQLITE_API int sqlite3changeset_finalize(sqlite3_changeset_iter *pIter);

/*
** CAPI3REF: Invert A Changeset
**
** This function is used to "invert" a changeset object. Applying an inverted
** changeset to a database reverses the effects of applying the uninverted
** changeset. Specifically:
**
** <ul>
**   <li> Each DELETE change is changed to an INSERT, and
**   <li> Each INSERT change is changed to a DELETE, and
**   <li> For each UPDATE change, the old.* and new.* values are exchanged.
** </ul>
**
** This function does not change the order in which changes appear within
** the changeset. It merely reverses the sense of each individual change.
**
** If successful, a pointer to a buffer containing the inverted changeset
** is stored in *ppOut, the size of the same buffer is stored in *pnOut, and
** SQLITE_OK is returned. If an error occurs, both *pnOut and *ppOut are
** zeroed and an SQLite error code returned.
**
** It is the responsibility of the caller to eventually call sqlite3_free()
** on the *ppOut pointer to free the buffer allocation following a successful
** call to this function.
**
** WARNING/TODO: This function currently assumes that the input is a valid
** changeset. If it is not, the results are undefined.
*/
SQLITE_API int sqlite3changeset_invert(
  int nIn, const void *pIn,       /* Input changeset */
  int *pnOut, void **ppOut        /* OUT: Inverse of input */
);

/*
** CAPI3REF: Concatenate Two Changeset Objects
**
** This function is used to concatenate two changesets, A and B, into a
** single changeset. The result is a changeset equivalent to applying
** changeset A followed by changeset B.
**
** This function combines the two input changesets using an
** sqlite3_changegroup object. Calling it produces similar results as the
** following code fragment:
**
** <pre>
**   sqlite3_changegroup *pGrp;
**   rc = sqlite3_changegroup_new(&pGrp);
**   if( rc==SQLITE_OK ) rc = sqlite3changegroup_add(pGrp, nA, pA);
**   if( rc==SQLITE_OK ) rc = sqlite3changegroup_add(pGrp, nB, pB);
**   if( rc==SQLITE_OK ){
**     rc = sqlite3changegroup_output(pGrp, pnOut, ppOut);
**   }else{
**     *ppOut = 0;
**     *pnOut = 0;
**   }
** </pre>
**
** Refer to the sqlite3_changegroup documentation below for details.
*/
SQLITE_API int sqlite3changeset_concat(
  int nA,                         /* Number of bytes in buffer pA */
  void *pA,                       /* Pointer to buffer containing changeset A */
  int nB,                         /* Number of bytes in buffer pB */
  void *pB,                       /* Pointer to buffer containing changeset B */
  int *pnOut,                     /* OUT: Number of bytes in output changeset */
  void **ppOut                    /* OUT: Buffer containing output changeset */
);


/*
** CAPI3REF: Changegroup Handle
**
** A changegroup is an object used to combine two or more
** [changesets] or [patchsets]
*/
typedef struct sqlite3_changegroup sqlite3_changegroup;

/*
** CAPI3REF: Create A New Changegroup Object
** CONSTRUCTOR: sqlite3_changegroup
**
** An sqlite3_changegroup object is used to combine two or more changesets
** (or patchsets) into a single changeset (or patchset). A single changegroup
** object may combine changesets or patchsets, but not both. The output is
** always in the same format as the input.
**
** If successful, this function returns SQLITE_OK and populates (*pp) with
** a pointer to a new sqlite3_changegroup object before returning. The caller
** should eventually free the returned object using a call to
** sqlite3changegroup_delete(). If an error occurs, an SQLite error code
** (i.e. SQLITE_NOMEM) is returned and *pp is set to NULL.
**
** The usual usage pattern for an sqlite3_changegroup object is as follows:
**
** <ul>
**   <li> It is created using a call to sqlite3changegroup_new().
**
**   <li> Zero or more changesets (or patchsets) are added to the object
**        by calling sqlite3changegroup_add().
**
**   <li> The result of combining all input changesets together is obtained
**        by the application via a call to sqlite3changegroup_output().
**
**   <li> The object is deleted using a call to sqlite3changegroup_delete().
** </ul>
**
** Any number of calls to add() and output() may be made between the calls to
** new() and delete(), and in any order.
**
** As well as the regular sqlite3changegroup_add() and
** sqlite3changegroup_output() functions, also available are the streaming
** versions sqlite3changegroup_add_strm() and sqlite3changegroup_output_strm().
*/
SQLITE_API int sqlite3changegroup_new(sqlite3_changegroup **pp);

/*
** CAPI3REF: Add A Changeset To A Changegroup
** METHOD: sqlite3_changegroup
**
** Add all changes within the changeset (or patchset) in buffer pData (size
** nData bytes) to the changegroup.
**
** If the buffer contains a patchset, then all prior calls to this function
** on the same changegroup object must also have specified patchsets. Or, if
** the buffer contains a changeset, so must have the earlier calls to this
** function. Otherwise, SQLITE_ERROR is returned and no changes are added
** to the changegroup.
**
** Rows within the changeset and changegroup are identified by the values in
** their PRIMARY KEY columns. A change in the changeset is considered to
** apply to the same row as a change already present in the changegroup if
** the two rows have the same primary key.
**
** Changes to rows that do not already appear in the changegroup are
** simply copied into it. Or, if both the new changeset and the changegroup
** contain changes that apply to a single row, the final contents of the
** changegroup depends on the type of each change, as follows:
**
** <table border=1 style="margin-left:8ex;margin-right:8ex">
**   <tr><th style="white-space:pre">Existing Change  </th>
**       <th style="white-space:pre">New Change       </th>
**       <th>Output Change
**   <tr><td>INSERT <td>INSERT <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>INSERT <td>UPDATE <td>
**       The INSERT change remains in the changegroup. The values in the
**       INSERT change are modified as if the row was inserted by the
**       existing change and then updated according to the new change.
**   <tr><td>INSERT <td>DELETE <td>
**       The existing INSERT is removed from the changegroup. The DELETE is
**       not added.
**   <tr><td>UPDATE <td>INSERT <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>UPDATE <td>UPDATE <td>
**       The existing UPDATE remains within the changegroup. It is amended
**       so that the accompanying values are as if the row was updated once
**       by the existing change and then again by the new change.
**   <tr><td>UPDATE <td>DELETE <td>
**       The existing UPDATE is replaced by the new DELETE within the
**       changegroup.
**   <tr><td>DELETE <td>INSERT <td>
**       If one or more of the column values in the row inserted by the
**       new change differ from those in the row deleted by the existing
**       change, the existing DELETE is replaced by an UPDATE within the
**       changegroup. Otherwise, if the inserted row is exactly the same
**       as the deleted row, the existing DELETE is simply discarded.
**   <tr><td>DELETE <td>UPDATE <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
**   <tr><td>DELETE <td>DELETE <td>
**       The new change is ignored. This case does not occur if the new
**       changeset was recorded immediately after the changesets already
**       added to the changegroup.
** </table>
**
** If the new changeset contains changes to a table that is already present
** in the changegroup, then the number of columns and the position of the
** primary key columns for the table must be consistent. If this is not the
** case, this function fails with SQLITE_SCHEMA. If the input changeset
** appears to be corrupt and the corruption is detected, SQLITE_CORRUPT is
** returned. Or, if an out-of-memory condition occurs during processing, this
** function returns SQLITE_NOMEM. In all cases, if an error occurs the state
** of the final contents of the changegroup is undefined.
**
** If no error occurs, SQLITE_OK is returned.
*/
SQLITE_API int sqlite3changegroup_add(sqlite3_changegroup*, int nData, void *pData);

/*
** CAPI3REF: Obtain A Composite Changeset From A Changegroup
** METHOD: sqlite3_changegroup
**
** Obtain a buffer containing a changeset (or patchset) representing the
** current contents of the changegroup. If the inputs to the changegroup
** were themselves changesets, the output is a changeset. Or, if the
** inputs were patchsets, the output is also a patchset.
**
** As with the output of the sqlite3session_changeset() and
** sqlite3session_patchset() functions, all changes related to a single
** table are grouped together in the output of this function. Tables appear
** in the same order as for the very first changeset added to the changegroup.
** If the second or subsequent changesets added to the changegroup contain
** changes for tables that do not appear in the first changeset, they are
** appended onto the end of the output changeset, again in the order in
** which they are first encountered.
**
** If an error occurs, an SQLite error code is returned and the output
** variables (*pnData) and (*ppData) are set to 0. Otherwise, SQLITE_OK
** is returned and the output variables are set to the size of and a
** pointer to the output buffer, respectively. In this case it is the
** responsibility of the caller to eventually free the buffer using a
** call to sqlite3_free().
*/
SQLITE_API int sqlite3changegroup_output(
  sqlite3_changegroup*,
  int *pnData,                    /* OUT: Size of output buffer in bytes */
  void **ppData                   /* OUT: Pointer to output buffer */
);

/*
** CAPI3REF: Delete A Changegroup Object
** DESTRUCTOR: sqlite3_changegroup
*/
SQLITE_API void sqlite3changegroup_delete(sqlite3_changegroup*);

/*
** CAPI3REF: Apply A Changeset To A Database
**
** Apply a changeset or patchset to a database. These functions attempt to
** update the "main" database attached to handle db with the changes found in
** the changeset passed via the second and third arguments.
**
** The fourth argument (xFilter) passed to these functions is the "filter
** callback". If it is not NULL, then for each table affected by at least one
** change in the changeset, the filter callback is invoked with
** the table name as the second argument, and a copy of the context pointer
** passed as the sixth argument as the first. If the "filter callback"
** returns zero, then no attempt is made to apply any changes to the table.
** Otherwise, if the return value is non-zero or the xFilter argument to
** is NULL, all changes related to the table are attempted.
**
** For each table that is not excluded by the filter callback, this function
** tests that the target database contains a compatible table. A table is
** considered compatible if all of the following are true:
**
** <ul>
**   <li> The table has the same name as the name recorded in the
**        changeset, and
**   <li> The table has at least as many columns as recorded in the
**        changeset, and
**   <li> The table has primary key columns in the same position as
**        recorded in the changeset.
** </ul>
**
** If there is no compatible table, it is not an error, but none of the
** changes associated with the table are applied. A warning message is issued
** via the sqlite3_log() mechanism with the error code SQLITE_SCHEMA. At most
** one such warning is issued for each table in the changeset.
**
** For each change for which there is a compatible table, an attempt is made
** to modify the table contents according to the UPDATE, INSERT or DELETE
** change. If a change cannot be applied cleanly, the conflict handler
** function passed as the fifth argument to sqlite3changeset_apply() may be
** invoked. A description of exactly when the conflict handler is invoked for
** each type of change is below.
**
** Unlike the xFilter argument, xConflict may not be passed NULL. The results
** of passing anything other than a valid function pointer as the xConflict
** argument are undefined.
**
** Each time the conflict handler function is invoked, it must return one
** of [SQLITE_CHANGESET_OMIT], [SQLITE_CHANGESET_ABORT] or
** [SQLITE_CHANGESET_REPLACE]. SQLITE_CHANGESET_REPLACE may only be returned
** if the second argument passed to the conflict handler is either
** SQLITE_CHANGESET_DATA or SQLITE_CHANGESET_CONFLICT. If the conflict-handler
** returns an illegal value, any changes already made are rolled back and
** the call to sqlite3changeset_apply() returns SQLITE_MISUSE. Different
** actions are taken by sqlite3changeset_apply() depending on the value
** returned by each invocation of the conflict-handler function. Refer to
** the documentation for the three
** [SQLITE_CHANGESET_OMIT|available return values] for details.
**
** <dl>
** <dt>DELETE Changes<dd>
**   For each DELETE change, the function checks if the target database
**   contains a row with the same primary key value (or values) as the
**   original row values stored in the changeset. If it does, and the values
**   stored in all non-primary key columns also match the values stored in
**   the changeset the row is deleted from the target database.
**
**   If a row with matching primary key values is found, but one or more of
**   the non-primary key fields contains a value different from the original
**   row value stored in the changeset, the conflict-handler function is
**   invoked with [SQLITE_CHANGESET_DATA] as the second argument. If the
**   database table has more columns than are recorded in the changeset,
**   only the values of those non-primary key fields are compared against
**   the current database contents - any trailing database table columns
**   are ignored.
**
**   If no row with matching primary key values is found in the database,
**   the conflict-handler function is invoked with [SQLITE_CHANGESET_NOTFOUND]
**   passed as the second argument.
**
**   If the DELETE operation is attempted, but SQLite returns SQLITE_CONSTRAINT
**   (which can only happen if a foreign key constraint is violated), the
**   conflict-handler function is invoked with [SQLITE_CHANGESET_CONSTRAINT]
**   passed as the second argument. This includes the case where the DELETE
**   operation is attempted because an earlier call to the conflict handler
**   function returned [SQLITE_CHANGESET_REPLACE].
**
** <dt>INSERT Changes<dd>
**   For each INSERT change, an attempt is made to insert the new row into
**   the database. If the changeset row contains fewer fields than the
**   database table, the trailing fields are populated with their default
**   values.
**
**   If the attempt to insert the row fails because the database already
**   contains a row with the same primary key values, the conflict handler
**   function is invoked with the second argument set to
**   [SQLITE_CHANGESET_CONFLICT].
**
**   If the attempt to insert the row fails because of some other constraint
**   violation (e.g. NOT NULL or UNIQUE), the conflict handler function is
**   invoked with the second argument set to [SQLITE_CHANGESET_CONSTRAINT].
**   This includes the case where the INSERT operation is re-attempted because
**   an earlier call to the conflict handler function returned
**   [SQLITE_CHANGESET_REPLACE].
**
** <dt>UPDATE Changes<dd>
**   For each UPDATE change, the function checks if the target database
**   contains a row with the same primary key value (or values) as the
**   original row values stored in the changeset. If it does, and the values
**   stored in all modified non-primary key columns also match the values
**   stored in the changeset the row is updated within the target database.
**
**   If a row with matching primary key values is found, but one or more of
**   the modified non-primary key fields contains a value different from an
**   original row value stored in the changeset, the conflict-handler function
**   is invoked with [SQLITE_CHANGESET_DATA] as the second argument. Since
**   UPDATE changes only contain values for non-primary key fields that are
**   to be modified, only those fields need to match the original values to
**   avoid the SQLITE_CHANGESET_DATA conflict-handler callback.
**
**   If no row with matching primary key values is found in the database,
**   the conflict-handler function is invoked with [SQLITE_CHANGESET_NOTFOUND]
**   passed as the second argument.
**
**   If the UPDATE operation is attempted, but SQLite returns
**   SQLITE_CONSTRAINT, the conflict-handler function is invoked with
**   [SQLITE_CHANGESET_CONSTRAINT] passed as the second argument.
**   This includes the case where the UPDATE operation is attempted after
**   an earlier call to the conflict handler function returned
**   [SQLITE_CHANGESET_REPLACE].
** </dl>
**
** It is safe to execute SQL statements, including those that write to the
** table that the callback related to, from within the xConflict callback.
** This can be used to further customize the application's conflict
** resolution strategy.
**
** All changes made by these functions are enclosed in a savepoint transaction.
** If any other error (aside from a constraint failure when attempting to
** write to the target database) occurs, then the savepoint transaction is
** rolled back, restoring the target database to its original state, and an
** SQLite error code returned.
**
** If the output parameters (ppRebase) and (pnRebase) are non-NULL and
** the input is a changeset (not a patchset), then sqlite3changeset_apply_v2()
** may set (*ppRebase) to point to a "rebase" that may be used with the
** sqlite3_rebaser APIs buffer before returning. In this case (*pnRebase)
** is set to the size of the buffer in bytes. It is the responsibility of the
** caller to eventually free any such buffer using sqlite3_free(). The buffer
** is only allocated and populated if one or more conflicts were encountered
** while applying the patchset. See comments surrounding the sqlite3_rebaser
** APIs for further details.
**
** The behavior of sqlite3changeset_apply_v2() and its streaming equivalent
** may be modified by passing a combination of
** [SQLITE_CHANGESETAPPLY_NOSAVEPOINT | supported flags] as the 9th parameter.
**
** Note that the sqlite3changeset_apply_v2() API is still <b>experimental</b>
** and therefore subject to change.
*/
SQLITE_API int sqlite3changeset_apply(
  sqlite3 *db,                    /* Apply change to "main" db of this handle */
  int nChangeset,                 /* Size of changeset in bytes */
  void *pChangeset,               /* Changeset blob */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    sqlite3_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx                      /* First argument passed to xConflict */
);
SQLITE_API int sqlite3changeset_apply_v2(
  sqlite3 *db,                    /* Apply change to "main" db of this handle */
  int nChangeset,                 /* Size of changeset in bytes */
  void *pChangeset,               /* Changeset blob */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    sqlite3_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase, /* OUT: Rebase data */
  int flags                       /* SESSION_CHANGESETAPPLY_* flags */
);

/*
** CAPI3REF: Flags for sqlite3changeset_apply_v2
**
** The following flags may passed via the 9th parameter to
** [sqlite3changeset_apply_v2] and [sqlite3changeset_apply_v2_strm]:
**
** <dl>
** <dt>SQLITE_CHANGESETAPPLY_NOSAVEPOINT <dd>
**   Usually, the sessions module encloses all operations performed by
**   a single call to apply_v2() or apply_v2_strm() in a [SAVEPOINT]. The
**   SAVEPOINT is committed if the changeset or patchset is successfully
**   applied, or rolled back if an error occurs. Specifying this flag
**   causes the sessions module to omit this savepoint. In this case, if the
**   caller has an open transaction or savepoint when apply_v2() is called,
**   it may revert the partially applied changeset by rolling it back.
**
** <dt>SQLITE_CHANGESETAPPLY_INVERT <dd>
**   Invert the changeset before applying it. This is equivalent to inverting
**   a changeset using sqlite3changeset_invert() before applying it. It is
**   an error to specify this flag with a patchset.
*/
#define SQLITE_CHANGESETAPPLY_NOSAVEPOINT   0x0001
#define SQLITE_CHANGESETAPPLY_INVERT        0x0002

/*
** CAPI3REF: Constants Passed To The Conflict Handler
**
** Values that may be passed as the second argument to a conflict-handler.
**
** <dl>
** <dt>SQLITE_CHANGESET_DATA<dd>
**   The conflict handler is invoked with CHANGESET_DATA as the second argument
**   when processing a DELETE or UPDATE change if a row with the required
**   PRIMARY KEY fields is present in the database, but one or more other
**   (non primary-key) fields modified by the update do not contain the
**   expected "before" values.
**
**   The conflicting row, in this case, is the database row with the matching
**   primary key.
**
** <dt>SQLITE_CHANGESET_NOTFOUND<dd>
**   The conflict handler is invoked with CHANGESET_NOTFOUND as the second
**   argument when processing a DELETE or UPDATE change if a row with the
**   required PRIMARY KEY fields is not present in the database.
**
**   There is no conflicting row in this case. The results of invoking the
**   sqlite3changeset_conflict() API are undefined.
**
** <dt>SQLITE_CHANGESET_CONFLICT<dd>
**   CHANGESET_CONFLICT is passed as the second argument to the conflict
**   handler while processing an INSERT change if the operation would result
**   in duplicate primary key values.
**
**   The conflicting row in this case is the database row with the matching
**   primary key.
**
** <dt>SQLITE_CHANGESET_FOREIGN_KEY<dd>
**   If foreign key handling is enabled, and applying a changeset leaves the
**   database in a state containing foreign key violations, the conflict
**   handler is invoked with CHANGESET_FOREIGN_KEY as the second argument
**   exactly once before the changeset is committed. If the conflict handler
**   returns CHANGESET_OMIT, the changes, including those that caused the
**   foreign key constraint violation, are committed. Or, if it returns
**   CHANGESET_ABORT, the changeset is rolled back.
**
**   No current or conflicting row information is provided. The only function
**   it is possible to call on the supplied sqlite3_changeset_iter handle
**   is sqlite3changeset_fk_conflicts().
**
** <dt>SQLITE_CHANGESET_CONSTRAINT<dd>
**   If any other constraint violation occurs while applying a change (i.e.
**   a UNIQUE, CHECK or NOT NULL constraint), the conflict handler is
**   invoked with CHANGESET_CONSTRAINT as the second argument.
**
**   There is no conflicting row in this case. The results of invoking the
**   sqlite3changeset_conflict() API are undefined.
**
** </dl>
*/
#define SQLITE_CHANGESET_DATA        1
#define SQLITE_CHANGESET_NOTFOUND    2
#define SQLITE_CHANGESET_CONFLICT    3
#define SQLITE_CHANGESET_CONSTRAINT  4
#define SQLITE_CHANGESET_FOREIGN_KEY 5

/*
** CAPI3REF: Constants Returned By The Conflict Handler
**
** A conflict handler callback must return one of the following three values.
**
** <dl>
** <dt>SQLITE_CHANGESET_OMIT<dd>
**   If a conflict handler returns this value no special action is taken. The
**   change that caused the conflict is not applied. The session module
**   continues to the next change in the changeset.
**
** <dt>SQLITE_CHANGESET_REPLACE<dd>
**   This value may only be returned if the second argument to the conflict
**   handler was SQLITE_CHANGESET_DATA or SQLITE_CHANGESET_CONFLICT. If this
**   is not the case, any changes applied so far are rolled back and the
**   call to sqlite3changeset_apply() returns SQLITE_MISUSE.
**
**   If CHANGESET_REPLACE is returned by an SQLITE_CHANGESET_DATA conflict
**   handler, then the conflicting row is either updated or deleted, depending
**   on the type of change.
**
**   If CHANGESET_REPLACE is returned by an SQLITE_CHANGESET_CONFLICT conflict
**   handler, then the conflicting row is removed from the database and a
**   second attempt to apply the change is made. If this second attempt fails,
**   the original row is restored to the database before continuing.
**
** <dt>SQLITE_CHANGESET_ABORT<dd>
**   If this value is returned, any changes applied so far are rolled back
**   and the call to sqlite3changeset_apply() returns SQLITE_ABORT.
** </dl>
*/
#define SQLITE_CHANGESET_OMIT       0
#define SQLITE_CHANGESET_REPLACE    1
#define SQLITE_CHANGESET_ABORT      2

/*
** CAPI3REF: Rebasing changesets
** EXPERIMENTAL
**
** Suppose there is a site hosting a database in state S0. And that
** modifications are made that move that database to state S1 and a
** changeset recorded (the "local" changeset). Then, a changeset based
** on S0 is received from another site (the "remote" changeset) and
** applied to the database. The database is then in state
** (S1+"remote"), where the exact state depends on any conflict
** resolution decisions (OMIT or REPLACE) made while applying "remote".
** Rebasing a changeset is to update it to take those conflict
** resolution decisions into account, so that the same conflicts
** do not have to be resolved elsewhere in the network.
**
** For example, if both the local and remote changesets contain an
** INSERT of the same key on "CREATE TABLE t1(a PRIMARY KEY, b)":
**
**   local:  INSERT INTO t1 VALUES(1, 'v1');
**   remote: INSERT INTO t1 VALUES(1, 'v2');
**
** and the conflict resolution is REPLACE, then the INSERT change is
** removed from the local changeset (it was overridden). Or, if the
** conflict resolution was "OMIT", then the local changeset is modified
** to instead contain:
**
**           UPDATE t1 SET b = 'v2' WHERE a=1;
**
** Changes within the local changeset are rebased as follows:
**
** <dl>
** <dt>Local INSERT<dd>
**   This may only conflict with a remote INSERT. If the conflict
**   resolution was OMIT, then add an UPDATE change to the rebased
**   changeset. Or, if the conflict resolution was REPLACE, add
**   nothing to the rebased changeset.
**
** <dt>Local DELETE<dd>
**   This may conflict with a remote UPDATE or DELETE. In both cases the
**   only possible resolution is OMIT. If the remote operation was a
**   DELETE, then add no change to the rebased changeset. If the remote
**   operation was an UPDATE, then the old.* fields of change are updated
**   to reflect the new.* values in the UPDATE.
**
** <dt>Local UPDATE<dd>
**   This may conflict with a remote UPDATE or DELETE. If it conflicts
**   with a DELETE, and the conflict resolution was OMIT, then the update
**   is changed into an INSERT. Any undefined values in the new.* record
**   from the update change are filled in using the old.* values from
**   the conflicting DELETE. Or, if the conflict resolution was REPLACE,
**   the UPDATE change is simply omitted from the rebased changeset.
**
**   If conflict is with a remote UPDATE and the resolution is OMIT, then
**   the old.* values are rebased using the new.* values in the remote
**   change. Or, if the resolution is REPLACE, then the change is copied
**   into the rebased changeset with updates to columns also updated by
**   the conflicting remote UPDATE removed. If this means no columns would
**   be updated, the change is omitted.
** </dl>
**
** A local change may be rebased against multiple remote changes
** simultaneously. If a single key is modified by multiple remote
** changesets, they are combined as follows before the local changeset
** is rebased:
**
** <ul>
**    <li> If there has been one or more REPLACE resolutions on a
**         key, it is rebased according to a REPLACE.
**
**    <li> If there have been no REPLACE resolutions on a key, then
**         the local changeset is rebased according to the most recent
**         of the OMIT resolutions.
** </ul>
**
** Note that conflict resolutions from multiple remote changesets are
** combined on a per-field basis, not per-row. This means that in the
** case of multiple remote UPDATE operations, some fields of a single
** local change may be rebased for REPLACE while others are rebased for
** OMIT.
**
** In order to rebase a local changeset, the remote changeset must first
** be applied to the local database using sqlite3changeset_apply_v2() and
** the buffer of rebase information captured. Then:
**
** <ol>
**   <li> An sqlite3_rebaser object is created by calling
**        sqlite3rebaser_create().
**   <li> The new object is configured with the rebase buffer obtained from
**        sqlite3changeset_apply_v2() by calling sqlite3rebaser_configure().
**        If the local changeset is to be rebased against multiple remote
**        changesets, then sqlite3rebaser_configure() should be called
**        multiple times, in the same order that the multiple
**        sqlite3changeset_apply_v2() calls were made.
**   <li> Each local changeset is rebased by calling sqlite3rebaser_rebase().
**   <li> The sqlite3_rebaser object is deleted by calling
**        sqlite3rebaser_delete().
** </ol>
*/
typedef struct sqlite3_rebaser sqlite3_rebaser;

/*
** CAPI3REF: Create a changeset rebaser object.
** EXPERIMENTAL
**
** Allocate a new changeset rebaser object. If successful, set (*ppNew) to
** point to the new object and return SQLITE_OK. Otherwise, if an error
** occurs, return an SQLite error code (e.g. SQLITE_NOMEM) and set (*ppNew)
** to NULL.
*/
SQLITE_API int sqlite3rebaser_create(sqlite3_rebaser **ppNew);

/*
** CAPI3REF: Configure a changeset rebaser object.
** EXPERIMENTAL
**
** Configure the changeset rebaser object to rebase changesets according
** to the conflict resolutions described by buffer pRebase (size nRebase
** bytes), which must have been obtained from a previous call to
** sqlite3changeset_apply_v2().
*/
SQLITE_API int sqlite3rebaser_configure(
  sqlite3_rebaser*,
  int nRebase, const void *pRebase
);

/*
** CAPI3REF: Rebase a changeset
** EXPERIMENTAL
**
** Argument pIn must point to a buffer containing a changeset nIn bytes
** in size. This function allocates and populates a buffer with a copy
** of the changeset rebased according to the configuration of the
** rebaser object passed as the first argument. If successful, (*ppOut)
** is set to point to the new buffer containing the rebased changeset and
** (*pnOut) to its size in bytes and SQLITE_OK returned. It is the
** responsibility of the caller to eventually free the new buffer using
** sqlite3_free(). Otherwise, if an error occurs, (*ppOut) and (*pnOut)
** are set to zero and an SQLite error code returned.
*/
SQLITE_API int sqlite3rebaser_rebase(
  sqlite3_rebaser*,
  int nIn, const void *pIn,
  int *pnOut, void **ppOut
);

/*
** CAPI3REF: Delete a changeset rebaser object.
** EXPERIMENTAL
**
** Delete the changeset rebaser object and all associated resources. There
** should be one call to this function for each successful invocation
** of sqlite3rebaser_create().
*/
SQLITE_API void sqlite3rebaser_delete(sqlite3_rebaser *p);

/*
** CAPI3REF: Streaming Versions of API functions.
**
** The six streaming API xxx_strm() functions serve similar purposes to the
** corresponding non-streaming API functions:
**
** <table border=1 style="margin-left:8ex;margin-right:8ex">
**   <tr><th>Streaming function<th>Non-streaming equivalent</th>
**   <tr><td>sqlite3changeset_apply_strm<td>[sqlite3changeset_apply]
**   <tr><td>sqlite3changeset_apply_strm_v2<td>[sqlite3changeset_apply_v2]
**   <tr><td>sqlite3changeset_concat_strm<td>[sqlite3changeset_concat]
**   <tr><td>sqlite3changeset_invert_strm<td>[sqlite3changeset_invert]
**   <tr><td>sqlite3changeset_start_strm<td>[sqlite3changeset_start]
**   <tr><td>sqlite3session_changeset_strm<td>[sqlite3session_changeset]
**   <tr><td>sqlite3session_patchset_strm<td>[sqlite3session_patchset]
** </table>
**
** Non-streaming functions that accept changesets (or patchsets) as input
** require that the entire changeset be stored in a single buffer in memory.
** Similarly, those that return a changeset or patchset do so by returning
** a pointer to a single large buffer allocated using sqlite3_malloc().
** Normally this is convenient. However, if an application running in a
** low-memory environment is required to handle very large changesets, the
** large contiguous memory allocations required can become onerous.
**
** In order to avoid this problem, instead of a single large buffer, input
** is passed to a streaming API functions by way of a callback function that
** the sessions module invokes to incrementally request input data as it is
** required. In all cases, a pair of API function parameters such as
**
**  <pre>
**  &nbsp;     int nChangeset,
**  &nbsp;     void *pChangeset,
**  </pre>
**
** Is replaced by:
**
**  <pre>
**  &nbsp;     int (*xInput)(void *pIn, void *pData, int *pnData),
**  &nbsp;     void *pIn,
**  </pre>
**
** Each time the xInput callback is invoked by the sessions module, the first
** argument passed is a copy of the supplied pIn context pointer. The second
** argument, pData, points to a buffer (*pnData) bytes in size. Assuming no
** error occurs the xInput method should copy up to (*pnData) bytes of data
** into the buffer and set (*pnData) to the actual number of bytes copied
** before returning SQLITE_OK. If the input is completely exhausted, (*pnData)
** should be set to zero to indicate this. Or, if an error occurs, an SQLite
** error code should be returned. In all cases, if an xInput callback returns
** an error, all processing is abandoned and the streaming API function
** returns a copy of the error code to the caller.
**
** In the case of sqlite3changeset_start_strm(), the xInput callback may be
** invoked by the sessions module at any point during the lifetime of the
** iterator. If such an xInput callback returns an error, the iterator enters
** an error state, whereby all subsequent calls to iterator functions
** immediately fail with the same error code as returned by xInput.
**
** Similarly, streaming API functions that return changesets (or patchsets)
** return them in chunks by way of a callback function instead of via a
** pointer to a single large buffer. In this case, a pair of parameters such
** as:
**
**  <pre>
**  &nbsp;     int *pnChangeset,
**  &nbsp;     void **ppChangeset,
**  </pre>
**
** Is replaced by:
**
**  <pre>
**  &nbsp;     int (*xOutput)(void *pOut, const void *pData, int nData),
**  &nbsp;     void *pOut
**  </pre>
**
** The xOutput callback is invoked zero or more times to return data to
** the application. The first parameter passed to each call is a copy of the
** pOut pointer supplied by the application. The second parameter, pData,
** points to a buffer nData bytes in size containing the chunk of output
** data being returned. If the xOutput callback successfully processes the
** supplied data, it should return SQLITE_OK to indicate success. Otherwise,
** it should return some other SQLite error code. In this case processing
** is immediately abandoned and the streaming API function returns a copy
** of the xOutput error code to the application.
**
** The sessions module never invokes an xOutput callback with the third
** parameter set to a value less than or equal to zero. Other than this,
** no guarantees are made as to the size of the chunks of data returned.
*/
SQLITE_API int sqlite3changeset_apply_strm(
  sqlite3 *db,                    /* Apply change to "main" db of this handle */
  int (*xInput)(void *pIn, void *pData, int *pnData), /* Input function */
  void *pIn,                                          /* First arg for xInput */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    sqlite3_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx                      /* First argument passed to xConflict */
);
SQLITE_API int sqlite3changeset_apply_v2_strm(
  sqlite3 *db,                    /* Apply change to "main" db of this handle */
  int (*xInput)(void *pIn, void *pData, int *pnData), /* Input function */
  void *pIn,                                          /* First arg for xInput */
  int(*xFilter)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    const char *zTab              /* Table name */
  ),
  int(*xConflict)(
    void *pCtx,                   /* Copy of sixth arg to _apply() */
    int eConflict,                /* DATA, MISSING, CONFLICT, CONSTRAINT */
    sqlite3_changeset_iter *p     /* Handle describing change and conflict */
  ),
  void *pCtx,                     /* First argument passed to xConflict */
  void **ppRebase, int *pnRebase,
  int flags
);
SQLITE_API int sqlite3changeset_concat_strm(
  int (*xInputA)(void *pIn, void *pData, int *pnData),
  void *pInA,
  int (*xInputB)(void *pIn, void *pData, int *pnData),
  void *pInB,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
SQLITE_API int sqlite3changeset_invert_strm(
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
SQLITE_API int sqlite3changeset_start_strm(
  sqlite3_changeset_iter **pp,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn
);
SQLITE_API int sqlite3changeset_start_v2_strm(
  sqlite3_changeset_iter **pp,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int flags
);
SQLITE_API int sqlite3session_changeset_strm(
  sqlite3_session *pSession,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
SQLITE_API int sqlite3session_patchset_strm(
  sqlite3_session *pSession,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);
SQLITE_API int sqlite3changegroup_add_strm(sqlite3_changegroup*,
    int (*xInput)(void *pIn, void *pData, int *pnData),
    void *pIn
);
SQLITE_API int sqlite3changegroup_output_strm(sqlite3_changegroup*,
    int (*xOutput)(void *pOut, const void *pData, int nData),
    void *pOut
);
SQLITE_API int sqlite3rebaser_rebase_strm(
  sqlite3_rebaser *pRebaser,
  int (*xInput)(void *pIn, void *pData, int *pnData),
  void *pIn,
  int (*xOutput)(void *pOut, const void *pData, int nData),
  void *pOut
);

/*
** CAPI3REF: Configure global parameters
**
** The sqlite3session_config() interface is used to make global configuration
** changes to the sessions module in order to tune it to the specific needs
** of the application.
**
** The sqlite3session_config() interface is not threadsafe. If it is invoked
** while any other thread is inside any other sessions method then the
** results are undefined. Furthermore, if it is invoked after any sessions
** related objects have been created, the results are also undefined.
**
** The first argument to the sqlite3session_config() function must be one
** of the SQLITE_SESSION_CONFIG_XXX constants defined below. The
** interpretation of the (void*) value passed as the second parameter and
** the effect of calling this function depends on the value of the first
** parameter.
**
** <dl>
** <dt>SQLITE_SESSION_CONFIG_STRMSIZE<dd>
**    By default, the sessions module streaming interfaces attempt to input
**    and output data in approximately 1 KiB chunks. This operand may be used
**    to set and query the value of this configuration setting. The pointer
**    passed as the second argument must point to a value of type (int).
**    If this value is greater than 0, it is used as the new streaming data
**    chunk size for both input and output. Before returning, the (int) value
**    pointed to by pArg is set to the final value of the streaming interface
**    chunk size.
** </dl>
**
** This function returns SQLITE_OK if successful, or an SQLite error code
** otherwise.
*/
SQLITE_API int sqlite3session_config(int op, void *pArg);

/*
** CAPI3REF: Values for sqlite3session_config().
*/
#define SQLITE_SESSION_CONFIG_STRMSIZE 1

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
}
#endif

#endif  /* !defined(__SQLITESESSION_H_) && defined(SQLITE_ENABLE_SESSION) */

/******** End of sqlite3session.h *********/
/******** Begin file fts5.h *********/
/*
** 2014 May 31
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** Interfaces to extend FTS5. Using the interfaces defined in this file,
** FTS5 may be extended with:
**
**     * custom tokenizers, and
**     * custom auxiliary functions.
*/


#ifndef _FTS5_H
#define _FTS5_H


#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
** CUSTOM AUXILIARY FUNCTIONS
**
** Virtual table implementations may overload SQL functions by implementing
** the sqlite3_module.xFindFunction() method.
*/

typedef struct Fts5ExtensionApi Fts5ExtensionApi;
typedef struct Fts5Context Fts5Context;
typedef struct Fts5PhraseIter Fts5PhraseIter;

typedef void (*fts5_extension_function)(
  const Fts5ExtensionApi *pApi,   /* API offered by current FTS version */
  Fts5Context *pFts,              /* First arg to pass to pApi functions */
  sqlite3_context *pCtx,          /* Context for returning result/error */
  int nVal,                       /* Number of values in apVal[] array */
  sqlite3_value **apVal           /* Array of trailing arguments */
);

struct Fts5PhraseIter {
  const unsigned char *a;
  const unsigned char *b;
};

/*
** EXTENSION API FUNCTIONS
**
** xUserData(pFts):
**   Return a copy of the context pointer the extension function was
**   registered with.
**
** xColumnTotalSize(pFts, iCol, pnToken):
**   If parameter iCol is less than zero, set output variable *pnToken
**   to the total number of tokens in the FTS5 table. Or, if iCol is
**   non-negative but less than the number of columns in the table, return
**   the total number of tokens in column iCol, considering all rows in
**   the FTS5 table.
**
**   If parameter iCol is greater than or equal to the number of columns
**   in the table, SQLITE_RANGE is returned. Or, if an error occurs (e.g.
**   an OOM condition or IO error), an appropriate SQLite error code is
**   returned.
**
** xColumnCount(pFts):
**   Return the number of columns in the table.
**
** xColumnSize(pFts, iCol, pnToken):
**   If parameter iCol is less than zero, set output variable *pnToken
**   to the total number of tokens in the current row. Or, if iCol is
**   non-negative but less than the number of columns in the table, set
**   *pnToken to the number of tokens in column iCol of the current row.
**
**   If parameter iCol is greater than or equal to the number of columns
**   in the table, SQLITE_RANGE is returned. Or, if an error occurs (e.g.
**   an OOM condition or IO error), an appropriate SQLite error code is
**   returned.
**
**   This function may be quite inefficient if used with an FTS5 table
**   created with the "columnsize=0" option.
**
** xColumnText:
**   This function attempts to retrieve the text of column iCol of the
**   current document. If successful, (*pz) is set to point to a buffer
**   containing the text in utf-8 encoding, (*pn) is set to the size in bytes
**   (not characters) of the buffer and SQLITE_OK is returned. Otherwise,
**   if an error occurs, an SQLite error code is returned and the final values
**   of (*pz) and (*pn) are undefined.
**
** xPhraseCount:
**   Returns the number of phrases in the current query expression.
**
** xPhraseSize:
**   Returns the number of tokens in phrase iPhrase of the query. Phrases
**   are numbered starting from zero.
**
** xInstCount:
**   Set *pnInst to the total number of occurrences of all phrases within
**   the query within the current row. Return SQLITE_OK if successful, or
**   an error code (i.e. SQLITE_NOMEM) if an error occurs.
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option. If the FTS5 table is created
**   with either "detail=none" or "detail=column" and "content=" option
**   (i.e. if it is a contentless table), then this API always returns 0.
**
** xInst:
**   Query for the details of phrase match iIdx within the current row.
**   Phrase matches are numbered starting from zero, so the iIdx argument
**   should be greater than or equal to zero and smaller than the value
**   output by xInstCount().
**
**   Usually, output parameter *piPhrase is set to the phrase number, *piCol
**   to the column in which it occurs and *piOff the token offset of the
**   first token of the phrase. Returns SQLITE_OK if successful, or an error
**   code (i.e. SQLITE_NOMEM) if an error occurs.
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option.
**
** xRowid:
**   Returns the rowid of the current row.
**
** xTokenize:
**   Tokenize text using the tokenizer belonging to the FTS5 table.
**
** xQueryPhrase(pFts5, iPhrase, pUserData, xCallback):
**   This API function is used to query the FTS table for phrase iPhrase
**   of the current query. Specifically, a query equivalent to:
**
**       ... FROM ftstable WHERE ftstable MATCH $p ORDER BY rowid
**
**   with $p set to a phrase equivalent to the phrase iPhrase of the
**   current query is executed. Any column filter that applies to
**   phrase iPhrase of the current query is included in $p. For each
**   row visited, the callback function passed as the fourth argument
**   is invoked. The context and API objects passed to the callback
**   function may be used to access the properties of each matched row.
**   Invoking Api.xUserData() returns a copy of the pointer passed as
**   the third argument to pUserData.
**
**   If the callback function returns any value other than SQLITE_OK, the
**   query is abandoned and the xQueryPhrase function returns immediately.
**   If the returned value is SQLITE_DONE, xQueryPhrase returns SQLITE_OK.
**   Otherwise, the error code is propagated upwards.
**
**   If the query runs to completion without incident, SQLITE_OK is returned.
**   Or, if some error occurs before the query completes or is aborted by
**   the callback, an SQLite error code is returned.
**
**
** xSetAuxdata(pFts5, pAux, xDelete)
**
**   Save the pointer passed as the second argument as the extension function's
**   "auxiliary data". The pointer may then be retrieved by the current or any
**   future invocation of the same fts5 extension function made as part of
**   the same MATCH query using the xGetAuxdata() API.
**
**   Each extension function is allocated a single auxiliary data slot for
**   each FTS query (MATCH expression). If the extension function is invoked
**   more than once for a single FTS query, then all invocations share a
**   single auxiliary data context.
**
**   If there is already an auxiliary data pointer when this function is
**   invoked, then it is replaced by the new pointer. If an xDelete callback
**   was specified along with the original pointer, it is invoked at this
**   point.
**
**   The xDelete callback, if one is specified, is also invoked on the
**   auxiliary data pointer after the FTS5 query has finished.
**
**   If an error (e.g. an OOM condition) occurs within this function,
**   the auxiliary data is set to NULL and an error code returned. If the
**   xDelete parameter was not NULL, it is invoked on the auxiliary data
**   pointer before returning.
**
**
** xGetAuxdata(pFts5, bClear)
**
**   Returns the current auxiliary data pointer for the fts5 extension
**   function. See the xSetAuxdata() method for details.
**
**   If the bClear argument is non-zero, then the auxiliary data is cleared
**   (set to NULL) before this function returns. In this case the xDelete,
**   if any, is not invoked.
**
**
** xRowCount(pFts5, pnRow)
**
**   This function is used to retrieve the total number of rows in the table.
**   In other words, the same value that would be returned by:
**
**        SELECT count(*) FROM ftstable;
**
** xPhraseFirst()
**   This function is used, along with type Fts5PhraseIter and the xPhraseNext
**   method, to iterate through all instances of a single query phrase within
**   the current row. This is the same information as is accessible via the
**   xInstCount/xInst APIs. While the xInstCount/xInst APIs are more convenient
**   to use, this API may be faster under some circumstances. To iterate
**   through instances of phrase iPhrase, use the following code:
**
**       Fts5PhraseIter iter;
**       int iCol, iOff;
**       for(pApi->xPhraseFirst(pFts, iPhrase, &iter, &iCol, &iOff);
**           iCol>=0;
**           pApi->xPhraseNext(pFts, &iter, &iCol, &iOff)
**       ){
**         // An instance of phrase iPhrase at offset iOff of column iCol
**       }
**
**   The Fts5PhraseIter structure is defined above. Applications should not
**   modify this structure directly - it should only be used as shown above
**   with the xPhraseFirst() and xPhraseNext() API methods (and by
**   xPhraseFirstColumn() and xPhraseNextColumn() as illustrated below).
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" or "detail=column" option. If the FTS5 table is created
**   with either "detail=none" or "detail=column" and "content=" option
**   (i.e. if it is a contentless table), then this API always iterates
**   through an empty set (all calls to xPhraseFirst() set iCol to -1).
**
** xPhraseNext()
**   See xPhraseFirst above.
**
** xPhraseFirstColumn()
**   This function and xPhraseNextColumn() are similar to the xPhraseFirst()
**   and xPhraseNext() APIs described above. The difference is that instead
**   of iterating through all instances of a phrase in the current row, these
**   APIs are used to iterate through the set of columns in the current row
**   that contain one or more instances of a specified phrase. For example:
**
**       Fts5PhraseIter iter;
**       int iCol;
**       for(pApi->xPhraseFirstColumn(pFts, iPhrase, &iter, &iCol);
**           iCol>=0;
**           pApi->xPhraseNextColumn(pFts, &iter, &iCol)
**       ){
**         // Column iCol contains at least one instance of phrase iPhrase
**       }
**
**   This API can be quite slow if used with an FTS5 table created with the
**   "detail=none" option. If the FTS5 table is created with either
**   "detail=none" "content=" option (i.e. if it is a contentless table),
**   then this API always iterates through an empty set (all calls to
**   xPhraseFirstColumn() set iCol to -1).
**
**   The information accessed using this API and its companion
**   xPhraseFirstColumn() may also be obtained using xPhraseFirst/xPhraseNext
**   (or xInst/xInstCount). The chief advantage of this API is that it is
**   significantly more efficient than those alternatives when used with
**   "detail=column" tables.
**
** xPhraseNextColumn()
**   See xPhraseFirstColumn above.
*/
struct Fts5ExtensionApi {
  int iVersion;                   /* Currently always set to 3 */

  void *(*xUserData)(Fts5Context*);

  int (*xColumnCount)(Fts5Context*);
  int (*xRowCount)(Fts5Context*, sqlite3_int64 *pnRow);
  int (*xColumnTotalSize)(Fts5Context*, int iCol, sqlite3_int64 *pnToken);

  int (*xTokenize)(Fts5Context*,
    const char *pText, int nText, /* Text to tokenize */
    void *pCtx,                   /* Context passed to xToken() */
    int (*xToken)(void*, int, const char*, int, int, int)       /* Callback */
  );

  int (*xPhraseCount)(Fts5Context*);
  int (*xPhraseSize)(Fts5Context*, int iPhrase);

  int (*xInstCount)(Fts5Context*, int *pnInst);
  int (*xInst)(Fts5Context*, int iIdx, int *piPhrase, int *piCol, int *piOff);

  sqlite3_int64 (*xRowid)(Fts5Context*);
  int (*xColumnText)(Fts5Context*, int iCol, const char **pz, int *pn);
  int (*xColumnSize)(Fts5Context*, int iCol, int *pnToken);

  int (*xQueryPhrase)(Fts5Context*, int iPhrase, void *pUserData,
    int(*)(const Fts5ExtensionApi*,Fts5Context*,void*)
  );
  int (*xSetAuxdata)(Fts5Context*, void *pAux, void(*xDelete)(void*));
  void *(*xGetAuxdata)(Fts5Context*, int bClear);

  int (*xPhraseFirst)(Fts5Context*, int iPhrase, Fts5PhraseIter*, int*, int*);
  void (*xPhraseNext)(Fts5Context*, Fts5PhraseIter*, int *piCol, int *piOff);

  int (*xPhraseFirstColumn)(Fts5Context*, int iPhrase, Fts5PhraseIter*, int*);
  void (*xPhraseNextColumn)(Fts5Context*, Fts5PhraseIter*, int *piCol);
};

/*
** CUSTOM AUXILIARY FUNCTIONS
*************************************************************************/

/*************************************************************************
** CUSTOM TOKENIZERS
**
** Applications may also register custom tokenizer types. A tokenizer
** is registered by providing fts5 with a populated instance of the
** following structure. All structure methods must be defined, setting
** any member of the fts5_tokenizer struct to NULL leads to undefined
** behaviour. The structure methods are expected to function as follows:
**
** xCreate:
**   This function is used to allocate and initialize a tokenizer instance.
**   A tokenizer instance is required to actually tokenize text.
**
**   The first argument passed to this function is a copy of the (void*)
**   pointer provided by the application when the fts5_tokenizer object
**   was registered with FTS5 (the third argument to xCreateTokenizer()).
**   The second and third arguments are an array of nul-terminated strings
**   containing the tokenizer arguments, if any, specified following the
**   tokenizer name as part of the CREATE VIRTUAL TABLE statement used
**   to create the FTS5 table.
**
**   The final argument is an output variable. If successful, (*ppOut)
**   should be set to point to the new tokenizer handle and SQLITE_OK
**   returned. If an error occurs, some value other than SQLITE_OK should
**   be returned. In this case, fts5 assumes that the final value of *ppOut
**   is undefined.
**
** xDelete:
**   This function is invoked to delete a tokenizer handle previously
**   allocated using xCreate(). Fts5 guarantees that this function will
**   be invoked exactly once for each successful call to xCreate().
**
** xTokenize:
**   This function is expected to tokenize the nText byte string indicated
**   by argument pText. pText may or may not be nul-terminated. The first
**   argument passed to this function is a pointer to an Fts5Tokenizer object
**   returned by an earlier call to xCreate().
**
**   The second argument indicates the reason that FTS5 is requesting
**   tokenization of the supplied text. This is always one of the following
**   four values:
**
**   <ul><li> <b>FTS5_TOKENIZE_DOCUMENT</b> - A document is being inserted into
**            or removed from the FTS table. The tokenizer is being invoked to
**            determine the set of tokens to add to (or delete from) the
**            FTS index.
**
**       <li> <b>FTS5_TOKENIZE_QUERY</b> - A MATCH query is being executed
**            against the FTS index. The tokenizer is being called to tokenize
**            a bareword or quoted string specified as part of the query.
**
**       <li> <b>(FTS5_TOKENIZE_QUERY | FTS5_TOKENIZE_PREFIX)</b> - Same as
**            FTS5_TOKENIZE_QUERY, except that the bareword or quoted string is
**            followed by a "*" character, indicating that the last token
**            returned by the tokenizer will be treated as a token prefix.
**
**       <li> <b>FTS5_TOKENIZE_AUX</b> - The tokenizer is being invoked to
**            satisfy an fts5_api.xTokenize() request made by an auxiliary
**            function. Or an fts5_api.xColumnSize() request made by the same
**            on a columnsize=0 database.
**   </ul>
**
**   For each token in the input string, the supplied callback xToken() must
**   be invoked. The first argument to it should be a copy of the pointer
**   passed as the second argument to xTokenize(). The third and fourth
**   arguments are a pointer to a buffer containing the token text, and the
**   size of the token in bytes. The 4th and 5th arguments are the byte offsets
**   of the first byte of and first byte immediately following the text from
**   which the token is derived within the input.
**
**   The second argument passed to the xToken() callback ("tflags") should
**   normally be set to 0. The exception is if the tokenizer supports
**   synonyms. In this case see the discussion below for details.
**
**   FTS5 assumes the xToken() callback is invoked for each token in the
**   order that they occur within the input text.
**
**   If an xToken() callback returns any value other than SQLITE_OK, then
**   the tokenization should be abandoned and the xTokenize() method should
**   immediately return a copy of the xToken() return value. Or, if the
**   input buffer is exhausted, xTokenize() should return SQLITE_OK. Finally,
**   if an error occurs with the xTokenize() implementation itself, it
**   may abandon the tokenization and return any error code other than
**   SQLITE_OK or SQLITE_DONE.
**
** SYNONYM SUPPORT
**
**   Custom tokenizers may also support synonyms. Consider a case in which a
**   user wishes to query for a phrase such as "first place". Using the
**   built-in tokenizers, the FTS5 query 'first + place' will match instances
**   of "first place" within the document set, but not alternative forms
**   such as "1st place". In some applications, it would be better to match
**   all instances of "first place" or "1st place" regardless of which form
**   the user specified in the MATCH query text.
**
**   There are several ways to approach this in FTS5:
**
**   <ol><li> By mapping all synonyms to a single token. In this case, using
**            the above example, this means that the tokenizer returns the
**            same token for inputs "first" and "1st". Say that token is in
**            fact "first", so that when the user inserts the document "I won
**            1st place" entries are added to the index for tokens "i", "won",
**            "first" and "place". If the user then queries for '1st + place',
**            the tokenizer substitutes "first" for "1st" and the query works
**            as expected.
**
**       <li> By querying the index for all synonyms of each query term
**            separately. In this case, when tokenizing query text, the
**            tokenizer may provide multiple synonyms for a single term
**            within the document. FTS5 then queries the index for each
**            synonym individually. For example, faced with the query:
**
**   <codeblock>
**     ... MATCH 'first place'</codeblock>
**
**            the tokenizer offers both "1st" and "first" as synonyms for the
**            first token in the MATCH query and FTS5 effectively runs a query
**            similar to:
**
**   <codeblock>
**     ... MATCH '(first OR 1st) place'</codeblock>
**
**            except that, for the purposes of auxiliary functions, the query
**            still appears to contain just two phrases - "(first OR 1st)"
**            being treated as a single phrase.
**
**       <li> By adding multiple synonyms for a single term to the FTS index.
**            Using this method, when tokenizing document text, the tokenizer
**            provides multiple synonyms for each token. So that when a
**            document such as "I won first place" is tokenized, entries are
**            added to the FTS index for "i", "won", "first", "1st" and
**            "place".
**
**            This way, even if the tokenizer does not provide synonyms
**            when tokenizing query text (it should not - to do so would be
**            inefficient), it doesn't matter if the user queries for
**            'first + place' or '1st + place', as there are entries in the
**            FTS index corresponding to both forms of the first token.
**   </ol>
**
**   Whether it is parsing document or query text, any call to xToken that
**   specifies a <i>tflags</i> argument with the FTS5_TOKEN_COLOCATED bit
**   is considered to supply a synonym for the previous token. For example,
**   when parsing the document "I won first place", a tokenizer that supports
**   synonyms would call xToken() 5 times, as follows:
**
**   <codeblock>
**       xToken(pCtx, 0, "i",                      1,  0,  1);
**       xToken(pCtx, 0, "won",                    3,  2,  5);
**       xToken(pCtx, 0, "first",                  5,  6, 11);
**       xToken(pCtx, FTS5_TOKEN_COLOCATED, "1st", 3,  6, 11);
**       xToken(pCtx, 0, "place",                  5, 12, 17);
**</codeblock>
**
**   It is an error to specify the FTS5_TOKEN_COLOCATED flag the first time
**   xToken() is called. Multiple synonyms may be specified for a single token
**   by making multiple calls to xToken(FTS5_TOKEN_COLOCATED) in sequence.
**   There is no limit to the number of synonyms that may be provided for a
**   single token.
**
**   In many cases, method (1) above is the best approach. It does not add
**   extra data to the FTS index or require FTS5 to query for multiple terms,
**   so it is efficient in terms of disk space and query speed. However, it
**   does not support prefix queries very well. If, as suggested above, the
**   token "first" is substituted for "1st" by the tokenizer, then the query:
**
**   <codeblock>
**     ... MATCH '1s*'</codeblock>
**
**   will not match documents that contain the token "1st" (as the tokenizer
**   will probably not map "1s" to any prefix of "first").
**
**   For full prefix support, method (3) may be preferred. In this case,
**   because the index contains entries for both "first" and "1st", prefix
**   queries such as 'fi*' or '1s*' will match correctly. However, because
**   extra entries are added to the FTS index, this method uses more space
**   within the database.
**
**   Method (2) offers a midpoint between (1) and (3). Using this method,
**   a query such as '1s*' will match documents that contain the literal
**   token "1st", but not "first" (assuming the tokenizer is not able to
**   provide synonyms for prefixes). However, a non-prefix query like '1st'
**   will match against "1st" and "first". This method does not require
**   extra disk space, as no extra entries are added to the FTS index.
**   On the other hand, it may require more CPU cycles to run MATCH queries,
**   as separate queries of the FTS index are required for each synonym.
**
**   When using methods (2) or (3), it is important that the tokenizer only
**   provide synonyms when tokenizing document text (method (2)) or query
**   text (method (3)), not both. Doing so will not cause any errors, but is
**   inefficient.
*/
typedef struct Fts5Tokenizer Fts5Tokenizer;
typedef struct fts5_tokenizer fts5_tokenizer;
struct fts5_tokenizer {
  int (*xCreate)(void*, const char **azArg, int nArg, Fts5Tokenizer **ppOut);
  void (*xDelete)(Fts5Tokenizer*);
  int (*xTokenize)(Fts5Tokenizer*,
      void *pCtx,
      int flags,            /* Mask of FTS5_TOKENIZE_* flags */
      const char *pText, int nText,
      int (*xToken)(
        void *pCtx,         /* Copy of 2nd argument to xTokenize() */
        int tflags,         /* Mask of FTS5_TOKEN_* flags */
        const char *pToken, /* Pointer to buffer containing token */
        int nToken,         /* Size of token in bytes */
        int iStart,         /* Byte offset of token within input text */
        int iEnd            /* Byte offset of end of token within input text */
      )
  );
};

/* Flags that may be passed as the third argument to xTokenize() */
#define FTS5_TOKENIZE_QUERY     0x0001
#define FTS5_TOKENIZE_PREFIX    0x0002
#define FTS5_TOKENIZE_DOCUMENT  0x0004
#define FTS5_TOKENIZE_AUX       0x0008

/* Flags that may be passed by the tokenizer implementation back to FTS5
** as the third argument to the supplied xToken callback. */
#define FTS5_TOKEN_COLOCATED    0x0001      /* Same position as prev. token */

/*
** END OF CUSTOM TOKENIZERS
*************************************************************************/

/*************************************************************************
** FTS5 EXTENSION REGISTRATION API
*/
typedef struct fts5_api fts5_api;
struct fts5_api {
  int iVersion;                   /* Currently always set to 2 */

  /* Create a new tokenizer */
  int (*xCreateTokenizer)(
    fts5_api *pApi,
    const char *zName,
    void *pContext,
    fts5_tokenizer *pTokenizer,
    void (*xDestroy)(void*)
  );

  /* Find an existing tokenizer */
  int (*xFindTokenizer)(
    fts5_api *pApi,
    const char *zName,
    void **ppContext,
    fts5_tokenizer *pTokenizer
  );

  /* Create a new auxiliary function */
  int (*xCreateFunction)(
    fts5_api *pApi,
    const char *zName,
    void *pContext,
    fts5_extension_function xFunction,
    void (*xDestroy)(void*)
  );
};

/*
** END OF REGISTRATION API
*************************************************************************/

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif /* _FTS5_H */

/******** End of fts5.h *********/
