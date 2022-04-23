#include "sqlited.h"
#include "sqlited_internal.h"

#define	REQUEST_EXTEND_LEN	256

int sqlited_prepare( sqlited *db, sqlited_pstmt **pstmt, const char *sql, int length )
{
	int			result;
	struct sqlited_result	*response;
	char			extend[REQUEST_EXTEND_LEN];
	struct timeval		time;
	unsigned long		id;
	sqlited_pstmt		*find;

	_sqlited_buf_init( db, length );

	gettimeofday( &time, NULL );
	id = time.tv_usec += (1000000 * time.tv_sec%10);

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/INIT", id );
	db->send_length = _sqlited_make_body( db, sql, length );
	db->send_length += _sqlited_make_header( db, SQLITED_METHOD_OPTION, extend );

	result = _sqlited_request( db, &(db->send_buf[db->send_offset]), db->send_length );
	if ( result != SQLITED_OK )
	{
		db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	result = _sqlited_response( db );
	if ( result == SQLITED_RESPONSE_END )
		response = _sqlited_response_parse( db );

	if ( db->response.status == 200 )
	{
		(*pstmt) = malloc( sizeof(struct sqlited_pstmt) );
		if ( db->pstmt == NULL )
			db->pstmt = (*pstmt);
		else
		{
			for ( find=db->pstmt; find; find=find->next )
			{
				if ( find->next == NULL )
				{
					find->next = (*pstmt);
					break;
				}
			}
		}
		(*pstmt)->id = id;
		(*pstmt)->db = db;
		(*pstmt)->next = NULL;
		(*pstmt)->result = NULL;
		return SQLITED_OK;
	}
	else
		return SQLITED_ERROR;
}

int sqlited_bind_blob( sqlited_pstmt *pstmt, int seq, const void *value, int length )
{
	char	extend[REQUEST_EXTEND_LEN];

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/BLOB", pstmt->id, seq );
    _sqlited_buf_init( pstmt->db, length );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, value, length );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_bind_double( sqlited_pstmt *pstmt, int seq, double value )
{
	char	extend[REQUEST_EXTEND_LEN];
	char	data[64];
	size_t	data_len;

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/DOUBLE", pstmt->id, seq );
	data_len = snprintf( data, 64, "%f", value );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, data, data_len );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_bind_int( sqlited_pstmt *pstmt, int seq, int value )
{
	char	extend[REQUEST_EXTEND_LEN];
	char	data[64];
	size_t	data_len;

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/INT", pstmt->id, seq );
	data_len = snprintf( data, 64, "%d", value );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, data, data_len );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_bind_int64( sqlited_pstmt *pstmt, int seq, long long int value )
{
	char	extend[REQUEST_EXTEND_LEN];
	char	data[64];
	size_t	data_len;

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/INT64", pstmt->id, seq );
	data_len = snprintf( data, 64, "%lld", value );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, data, data_len );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_bind_null( sqlited_pstmt *pstmt, int seq )
{
	char	extend[REQUEST_EXTEND_LEN];

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/NULL", pstmt->id, seq );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, NULL, 0 );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_bind_text( sqlited_pstmt *pstmt, int seq, const char *value, int length )
{
	char	extend[REQUEST_EXTEND_LEN];

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/%d/TEXT", pstmt->id, seq );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, value, length );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

sqlited_result *sqlited_execute( sqlited_pstmt *pstmt )
{
	int	result;
	char	extend[REQUEST_EXTEND_LEN];

	if ( pstmt == NULL )
		return NULL;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/EXECUTE", pstmt->id );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, NULL, 0 );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	result = _sqlited_pstmt_request( pstmt );
	if ( result == SQLITED_ERROR )
		return NULL;

	return pstmt->result;
}

int sqlited_reset( sqlited_pstmt *pstmt )
{
	char	extend[REQUEST_EXTEND_LEN];

	if ( pstmt == NULL )
		return SQLITED_ERROR;

	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/RESET", pstmt->id );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, NULL, 0 );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	return _sqlited_pstmt_request( pstmt );
}

int sqlited_finalize( sqlited_pstmt *pstmt )
{
	int			result;
	char			extend[REQUEST_EXTEND_LEN];
	sqlited_pstmt		*res;
	sqlited_pstmt		*prev = NULL;

	if ( pstmt == NULL )
		return SQLITED_ERROR;


	snprintf( extend, REQUEST_EXTEND_LEN, "/%lu/RELEASE", pstmt->id );
	pstmt->db->send_length = _sqlited_make_body( pstmt->db, "", 0 );
	pstmt->db->send_length += _sqlited_make_header( pstmt->db, SQLITED_METHOD_OPTION, extend );

	result = _sqlited_pstmt_request( pstmt );

	for ( res = pstmt->db->pstmt; res; res=res->next )
	{
		if ( res == pstmt )
		{
			if ( prev )
				prev->next = res->next;
			else
				pstmt->db->pstmt = res->next;
			free( res );
            break;
		}
		prev = res;
	}

	return result;
}

int _sqlited_pstmt_request( sqlited_pstmt *pstmt )
{
	int			result;
	struct sqlited_result	*response = NULL;

	result = _sqlited_request( pstmt->db, &(pstmt->db->send_buf[pstmt->db->send_offset]), pstmt->db->send_length );
	if ( result != SQLITED_OK )
	{
		pstmt->db->errmsg = strerror( errno );
		return SQLITED_ERROR;
	}

	result = _sqlited_response( pstmt->db );
	if ( result == SQLITED_RESPONSE_END )
	{
		response = _sqlited_response_parse( pstmt->db );
	}
	pstmt->result = response;

	if ( pstmt->db->response.status == 200 )
		return SQLITED_OK;
	else
		return SQLITED_ERROR;
}
