/*
 * 2006-04-16
 *
 * This header file defines the interface that the SQLiteDBMS
 * library presents to client programs.
 *
 */
#ifndef _SQLITED_H_
#define _SQLITED_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct sqlited sqlited;
typedef struct sqlited_result sqlited_result;
typedef struct sqlited_pstmt sqlited_pstmt;

typedef char sqlited_bool;


#define	SQLITED_OK		200

#define	SQLITED_CONNECTED	110
#define	SQLITED_CLOSED		120
#define	SQLITED_REQUEST_END	130
#define	SQLITED_RESPONSE_END	130

#define	SQLITED_MSG		290

#define	SQLITED_NOT_CONNECT	400
#define	SQLITED_AUTH		401
#define	SQLITED_BAD_FORMAT	403
#define	SQLITED_EMPTY		404
#define	SQLITED_NO_SSL		405

#define	SQLITED_ERROR		500
#define	SQLITED_NOMEM		510
#define	SQLITED_ERROR_SEND	520

#define	SQLITED_RESULT		0
#define	SQLITED_RESULT_MSG	1
#define	SQLITED_RESULT_EMTPY	-1


#if defined(_MSC_VER) || defined(__BORLANDC__)
  typedef __int64 sqlited_int64;
  typedef unsigned __int64 sqlited_uint64;
#else
  typedef long long int sqlited_int64;
  typedef unsigned long long int sqlited_uint64;
#endif


/* sqlite3lib, serverm, client version */
const char *sqlited_libversion(void);


/* connect, close db */
int sqlited_open( sqlited **db, const char *conninfo );
void sqlited_close( sqlited *db );

/* global error */
extern char *sqlited_error;
const char *sqlited_getmsg( sqlited *db );

/* execute, free result */
sqlited_result *sqlited_query( sqlited *db, const char *query );
void sqlited_free_result( sqlited_result *result );

/* result infomation */
short sqlited_result_type( sqlited_result *result );
sqlited_int64 sqlited_last_insert_rowid( sqlited *db );
sqlited_int64 sqlited_changes( sqlited *db );
sqlited_int64 sqlited_total_changes( sqlited *db );
sqlited_int64 sqlited_num_rows( sqlited_result *result );
unsigned int sqlited_num_fields( sqlited_result *result );
const char *sqlited_field_name( sqlited_result *result, unsigned int idx );

/* fetch data */
int sqlited_fetch_all( sqlited_result *result, char ****resultp, sqlited_int64 *nrow, unsigned int *nfield );
sqlited_int64 sqlited_row_seek( sqlited_result *result, sqlited_int64 seek );
const char **sqlited_fetch_row( sqlited_result *result );
const char *sqlited_fetch_result( sqlited_result *result, sqlited_int64 seek, unsigned int field );
const char *sqlited_fetch_msg( struct sqlited_result *result );

/* prepared statement */
int sqlited_prepare( sqlited *db, sqlited_pstmt **pstmt, const char *sql, int length );

int sqlited_bind_blob( sqlited_pstmt *pstmt, int seq, const void *value, int length );
int sqlited_bind_double( sqlited_pstmt *pstmt, int seq, double value );
int sqlited_bind_int( sqlited_pstmt *pstmt, int seq, int value );
int sqlited_bind_int64( sqlited_pstmt *pstmt, int seq, long long int value );
int sqlited_bind_null( sqlited_pstmt *pstmt, int seq );
int sqlited_bind_text( sqlited_pstmt *pstmt, int seq, const char *value, int length );

sqlited_result *sqlited_execute( sqlited_pstmt *pstmt );
int sqlited_reset( sqlited_pstmt *pstmt );
int sqlited_finalize( sqlited_pstmt *pstmt );

/* webdav */
int sqlited_content_put_file( sqlited *db, const char *remote_path, const char *local_path );
int sqlited_content_put( sqlited *db, const char *remote_path, const char *buf, size_t size );
int sqlited_content_delete( sqlited *db, const char *remote_path );
int sqlited_content_get( sqlited *db, const char *remote_path, char **buf, size_t *size );
void sqlited_content_free( char *buf );

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif
