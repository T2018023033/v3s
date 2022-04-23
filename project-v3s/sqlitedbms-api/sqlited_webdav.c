#include "sqlited.h"
#include "sqlited_internal.h"

int sqlited_content_put_file( sqlited *db, const char *remote_path, const char *local_path )
{
	extern int	errno;
	struct stat	stat;
	FILE		*src;
	size_t		sended;
	int		res;
	sqlited_result	*result;

	if ( db == NULL || db->send_buf == NULL || db->send_buf_size <= 0 )
		return SQLITED_ERROR;

	if ( lstat( local_path, &stat ) != 0 )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	src = fopen( local_path, "r" );
	if ( src == NULL )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

#if defined(__APPLE__)
	db->send_length = snprintf( db->send_buf, db->send_buf_size, "PUT %s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: %lld\r\nConnection: Keep-Alive\r\n\r\n", remote_path, db->session_key, stat.st_size );
#else
	db->send_length = snprintf( db->send_buf, db->send_buf_size, "PUT %s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: %ld\r\nConnection: Keep-Alive\r\n\r\n", remote_path, db->session_key, stat.st_size );
#endif

	if ( _sqlited_request( db, db->send_buf, db->send_length ) != SQLITED_OK )
	{
		fclose( src );
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	sended = 0;
	while ( sended < stat.st_size )
	{
		db->send_length = fread( db->send_buf, 1, MINVAL( db->send_buf_size, stat.st_size ), src );
		if ( db->send_length == 0 || _sqlited_request( db, db->send_buf, db->send_length ) != SQLITED_OK )
		{
			fclose( src );
			db->errmsg = strerror( errno );
			return SQLITED_ERROR;
		}
		sended += db->send_length;
	}
	fclose( src );

	res = _sqlited_response( db );
	if ( res == SQLITED_RESPONSE_END )
	{
		result = _sqlited_response_parse( db );
		if ( result == NULL )
		{
			db->errmsg = strerror( errno );
			return SQLITED_ERROR;
		}

		if ( result->status == 200 )
			return SQLITED_OK;
		else
		{
			db->errmsg = NULL;
			return SQLITED_ERROR;
		}
	}

	db->errmsg = strerror( errno );
	return SQLITED_ERROR;
}

int sqlited_content_put( sqlited *db, const char *remote_path, const char *buf, size_t size )
{
	extern int	errno;
	int		res;
	sqlited_result	*result;

	if ( db == NULL || db->send_buf == NULL || db->send_buf_size <= 0 )
		return SQLITED_ERROR;

	db->send_length = snprintf( db->send_buf, db->send_buf_size, "PUT %s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: %zu\r\nConnection: Keep-Alive\r\n\r\n", remote_path, db->session_key, size );

	if ( _sqlited_request( db, db->send_buf, db->send_length ) != SQLITED_OK || _sqlited_request( db, buf, size ) != SQLITED_OK )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	res = _sqlited_response( db );
	if ( res == SQLITED_RESPONSE_END )
	{
		result = _sqlited_response_parse( db );
		if ( result == NULL )
		{
			db->errmsg = strerror( errno );
			return SQLITED_ERROR;
		}

		if ( result->status == 200 )
			return SQLITED_OK;
		else
		{
			db->errmsg = NULL;
			return SQLITED_ERROR;
		}
	}

	db->errmsg = strerror( errno );
	return SQLITED_ERROR;
}

int sqlited_content_delete( sqlited *db, const char *remote_path )
{
	extern int	errno;
	int		res;
	sqlited_result	*result;

	if ( db == NULL || db->send_buf == NULL || db->send_buf_size <= 0 )
		return SQLITED_ERROR;

	db->send_length = snprintf( db->send_buf, db->send_buf_size, "DELETE %s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: 0\r\nConnection: Keep-Alive\r\n\r\n", remote_path, db->session_key );

	if ( _sqlited_request( db, db->send_buf, db->send_length ) != SQLITED_OK )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	res = _sqlited_response( db );
	if ( res == SQLITED_RESPONSE_END )
	{
		result = _sqlited_response_parse( db );
		if ( result == NULL )
		{
			db->errmsg = strerror( errno );
			return SQLITED_ERROR;
		}

		if ( result->status == 200 )
			return SQLITED_OK;
		else
		{
			db->errmsg = NULL;
			return SQLITED_ERROR;
		}
	}

	db->errmsg = strerror( errno );
	return SQLITED_ERROR;
}

int sqlited_content_get( sqlited *db, const char *remote_path, char **buf, size_t *size )
{
	extern int	errno;
	int		res;

	if ( db == NULL || db->send_buf == NULL || db->send_buf_size <= 0 )
		return SQLITED_ERROR;

	db->send_length = snprintf( db->send_buf, db->send_buf_size, "GET %s HTTP/1.1\r\nCookie: SQLSESSION=%s\r\nContent-length: 0\r\nConnection: Keep-Alive\r\n\r\n", remote_path, db->session_key );

	if ( _sqlited_request( db, db->send_buf, db->send_length ) != SQLITED_OK )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	res = _sqlited_response( db );
	if ( res == SQLITED_RESPONSE_END )
	{
		if ( db->response.status == 200 )
		{
			(*buf) = malloc( db->response.content_length+1 );
			memcpy( (*buf), db->response.body, db->response.content_length );
			(*buf)[db->response.content_length] = '\0';
			(*size) = db->response.content_length;
			return SQLITED_OK;
		}
		else
		{
			_sqlited_response_parse_msg( db );
			return SQLITED_ERROR;
		}
	}

	db->errmsg = strerror( errno );
	return SQLITED_ERROR;
}

void sqlited_content_free( char *buf )
{
	if ( buf != NULL )
		free( buf );
}
