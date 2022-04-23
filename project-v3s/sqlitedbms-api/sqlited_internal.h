#ifndef _SQLITED_INTERNAL_H_
#define _SQLITED_INTERNAL_H_

#define DEBUG			printf("----- DEBUG: %s: %d\n", __FILE__, __LINE__);
//#define TRACE(fmt,...) fprintf(stderr, "[%s: %d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); 
#define TRACE(fmt,...) 
#define	MAXVAL(x,y)		(x>y?x:y)
#define	MINVAL(x,y)		(x<y?x:y)


#define	SQLITED_LIB_VERSION	"0.2.0"

#define SQLITED_DEF_PORT	6543
#define SQLITED_DEF_SEND_BUF	32768
#define SQLITED_DEF_RECV_BUF	32768

#define	SQLITED_METHOD_POST	1
#define	SQLITED_METHOD_GET	2
#define	SQLITED_METHOD_HEAD	3
#define	SQLITED_METHOD_OPTION	4

#include "config.h"

#ifdef	ENABLE_SSL
#include <openssl/ssl.h>
#endif

#if USE_REWORKS
#include "netinet/in.h" /*支持reworks平台*/
#endif

#if USE_VXWORKS 
#include "in.h"
#endif

struct sqlited_response
{
	short			header_end;
	short			end;

	int			header_len;

	short			status;
	char			*msg;

	sqlited_int64		content_length;
	char			*trans_encoding;
	char			*cookie;

	char			*body;

	char			*chunk_offset;
	sqlited_int64		chunk_length;
};

struct sqlited_result
{
	sqlited_result		*next;

	sqlited			*db;

	short			status;
	short			type;

	char			*sql;

	sqlited_int64		changes;
	sqlited_int64		last_row_id;
	sqlited_int64		rows;
	unsigned int		fields;

	float			time;

	char			*msg;

	char			**field;
	char			***table;

	sqlited_int64		seek_row;
	unsigned int		seek_field;

	char			*raw;
	char			*raw_offset;
};

struct sqlited_pstmt
{
	sqlited_pstmt		*next;

	struct sqlited		*db;
	unsigned long		id;
	struct sqlited_result	*result;
};

struct sqlited
{
	int			sock;
	short			use_ssl;
#ifdef	ENABLE_SSL
	SSL_CTX			*ssl_ctx;
	SSL			*ssl;
#endif
	int			conn_stat;

	char			*protocol;
	char			*host;
	unsigned int		port;
	char			*username;
	char			*passwd;
	char			*dbname;

	char			*auth_str;
	char			*session_key;

	struct hostent		*hostent;
	struct sockaddr_in	server_addr;

	char			*conninfo;

	char			send_header[1024];

	char			*send_buf;
	sqlited_int64		send_buf_size;
	sqlited_int64		send_offset;
	sqlited_int64		send_length;

	char			*recv_buf;
	sqlited_int64		recv_buf_size;
	sqlited_int64		recv_length;

	struct sqlited_response	response;

	sqlited_int64		last_row_id;
	sqlited_int64		changes;
	sqlited_int64		total_changes;

	struct sqlited_result	msg;

	struct sqlited_result	*result;

	struct sqlited_pstmt	*pstmt;

	char			version_sqlite3[16];

	char			*errmsg;
};


/* sqlited.c */
void _sqlited_seterr( char *errmsg );
void _sqlited_buf_init( sqlited *db, size_t max );

/* sqlited_conn.c */
int _sqlited_parse_conninfo( const char *conninfo, sqlited *db );
int _sqlited_make_auth_str( sqlited *db );


/* sqlited_request.c */
sqlited_int64 _sqlited_make_header( sqlited *db, short method, char *extend );
sqlited_int64 _sqlited_make_body( sqlited *db, const char *query, size_t query_len );

int _sqlited_request( sqlited *db, const char *buf, sqlited_int64 length );
void _sqlited_request_session_init( sqlited *db );


/* sqlited_response.c */
int _sqlited_response( sqlited *db );
int _sqlited_response_pre_parse( sqlited *db );
void _sqlited_chunk_decode( sqlited *db );
struct sqlited_result *_sqlited_response_parse( sqlited *db );
int _sqlited_response_parse_msg( sqlited *db );
struct sqlited_result *_sqlited_response_parse_header( sqlited *db );
int _sqlited_response_parse_column( struct sqlited_result *result );
int _sqlited_response_parse_data( struct sqlited_result *result );

char *_sqlited_html_entity_decode( char *buf );

/* sqlited_fetch.c */
void _sqlited_free_result( struct sqlited_result *result );

/* sqlited_pstmt.c */
int _sqlited_pstmt_request( sqlited_pstmt *pstmt );

void sqlited_PrintDataBlock(const char* pTitle, const uint8_t *pBuf, int32_t Size);

#endif
