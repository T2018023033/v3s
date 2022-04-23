#include "sqlited.h"
#include "sqlited_internal.h"

sqlited_result *sqlited_query( sqlited *db, const char *query )
{
	extern int	errno;

	size_t		query_len;
	int		result;

	query_len = strlen( query );

	_sqlited_buf_init( db, query_len );

	db->send_length = _sqlited_make_body( db, query, query_len );
	db->send_length += _sqlited_make_header( db, SQLITED_METHOD_POST, NULL );

	result = _sqlited_request( db, &(db->send_buf[db->send_offset]), db->send_length );
	if ( result != SQLITED_OK )
	{
		db->errmsg = strerror( errno );
		return NULL;
	}

	result = _sqlited_response( db );
	if ( result == SQLITED_RESPONSE_END )
		return _sqlited_response_parse( db );

	//printf("%s\n", db->recv_buf);

	db->errmsg = strerror( errno );
	return NULL;
}

int _sqlited_request( sqlited *db, const char *buf, sqlited_int64 length )
{
	sqlited_int64	offset = 0;
	int		res;

	while ( offset < length )
	{
#ifdef	ENABLE_SSL
		if ( db->use_ssl == 1 )
		{
			res = SSL_write( db->ssl, &(buf[offset]), length-offset );
		}
		else
#endif
			res = write( db->sock, &(buf[offset]), length-offset );
		if ( res < 0 )
			return SQLITED_ERROR_SEND;
		offset += res;
	}

	return SQLITED_OK;
}

const unsigned char url_encode_hex[16] = { "0123456789ABCDEF" };
sqlited_int64 _sqlited_make_body( sqlited *db, const char *query, size_t query_len )
{
	size_t	length;
	int	i;
	int	c;
	char	*tmp;

	length = query_len;

	if ( query == '\0' )
		return 0;

	tmp = (char *) query;
	i = db->send_length = 1024;
	c = 0;

	while ( c < query_len )
	{
		if ( isalnum( tmp[0] ) || tmp[0] == '-' || tmp[0] == '=' )
			db->send_buf[i++] = tmp[0];
		else if ( tmp[0] == ' ' )
			db->send_buf[i++] = '+';
		else
		{
			db->send_buf[i++] = '%';
			/* Patched by http://sourceforge.net/tracker/index.php?func=detail&aid=1594037&group_id=106881&atid=645895 */
			//db->send_buf[i++] = url_encode_hex[ tmp[0]/16 ];
			//db->send_buf[i++] = url_encode_hex[ tmp[0]%16 ];
			db->send_buf[i++] = url_encode_hex[ (unsigned char)tmp[0] >> 4 ];
			db->send_buf[i++] = url_encode_hex[ (unsigned char)tmp[0] & 15 ];
		}
		c++;
		tmp++;
	}

	db->send_buf[i] = '\0';

	return i-1024;
}

sqlited_int64 _sqlited_make_header( sqlited *db, short method, char *extend )
{
	size_t	length;

	if ( method == SQLITED_METHOD_POST )
	{
		if ( db->session_key != NULL )
			length = snprintf( db->send_header, 1024, "POST /%s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: %lld\r\nConnection: Keep-Alive\r\n\r\n", db->dbname, db->session_key, db->send_length );
		else
			length = snprintf( db->send_header, 1024, "POST /%s HTTP/1.1\r\nAuthorization: Basic %s\r\nContent-length: %lld\r\nConnection: Keep-Alive\r\n\r\n", db->dbname, db->auth_str, db->send_length );
		db->send_offset = 1024-length;
		db->send_length += length;
		memcpy( &(db->send_buf[db->send_offset]), db->send_header, length );
		return length;
	}
	else if ( method == SQLITED_METHOD_OPTION )
	{
		if ( db->session_key != NULL )
			length = snprintf( db->send_header, 1024, "OPTION /%s%s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: %lld\r\nConnection: Keep-Alive\r\n\r\n", db->dbname, (extend != NULL?extend:""), db->session_key, db->send_length );
		else
			length = snprintf( db->send_header, 1024, "OPTION /%s%s HTTP/1.1\r\nAuthorization: Basic %s\r\nContent-length: %lld\r\nConnection: Keep-Alive\r\n\r\n", db->dbname, (extend != NULL?extend:""), db->auth_str, db->send_length );
		db->send_offset = 1024-length;
		db->send_length += length;
		memcpy( &(db->send_buf[db->send_offset]), db->send_header, length );
		return length;
	}

	return -1;
}

void _sqlited_request_session_init( sqlited *db )
{
	db->send_length = db->recv_length = 0;
	db->response.header_end = db->response.end = 0;
	db->response.chunk_offset = NULL;
}
