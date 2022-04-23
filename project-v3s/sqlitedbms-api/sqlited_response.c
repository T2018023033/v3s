#include "sqlited.h"
#include "sqlited_internal.h"

int _sqlited_response( sqlited *db )
{
	int	result;

	if ( db->recv_buf == NULL || db->recv_buf_size <= 0 )
	{
		db->recv_buf_size = SQLITED_DEF_RECV_BUF;
		db->recv_buf = (char *) malloc( db->recv_buf_size );
        if (!db->recv_buf) {
            perror("malloc error!");
            exit(-1);
        }
	}
	db->recv_length = 0;

	while ( 1 )
	{
#ifdef	ENABLE_SSL
		if ( db->use_ssl == 1 )
		{
			result = SSL_read( db->ssl, &(db->recv_buf[db->recv_length]), db->recv_buf_size-db->recv_length );
		}
		else
#endif
			result = read( db->sock, &(db->recv_buf[db->recv_length]), db->recv_buf_size-db->recv_length );

		if ( result < 0 )
			break;
		db->recv_length += result;
        result = _sqlited_response_pre_parse( db );

		if ( result == SQLITED_RESPONSE_END )
		{
			db->recv_buf[db->recv_length] = '\0';
			return result;
		}
		else if ( result == SQLITED_BAD_FORMAT )
			result = -1;

		if ( db->recv_buf_size - 4096 < db->recv_length )
		{
			db->recv_buf_size <<= 1;
			db->recv_buf = realloc( db->recv_buf, db->recv_buf_size ); /* 重新分配大小 */
            if (!db->recv_buf) {
                perror("realloc error!");
                exit(-1);
            }
		}
	}

	return SQLITED_ERROR;
}

int _sqlited_response_pre_parse( sqlited *db )
{
	int	header_len;
	char	*tmp;

	if ( db->response.header_end != 1 )
	{
		for ( header_len=1; header_len < db->recv_length; header_len++ )
		{
			if ( ( db->recv_buf[header_len-1] == '\n' && db->recv_buf[header_len] == '\n' )
				|| ( header_len > 2 && db->recv_buf[header_len-3] == '\r' && db->recv_buf[header_len-2] == '\n' && db->recv_buf[header_len-1] == '\r' && db->recv_buf[header_len] == '\n' ) )
			{
				header_len++;
				db->response.header_len = header_len;
				break;
			}
		}

		db->recv_buf[db->response.header_len-1] = '\0';

		db->response.status = atoi( &(db->recv_buf[9]) );
		db->response.msg = strchr( &(db->recv_buf[10]), ' ' );
		if ( db->response.msg == NULL )
		{
			db->response.msg = "";
			tmp = strchr( db->response.msg, '\n' ) + 1;
		}

		tmp = strchr( &(db->recv_buf[10]), '\n' );
		if ( tmp == NULL )
		{
			db->errmsg = "Bad protocol";
			return SQLITED_BAD_FORMAT;
		}

		db->response.content_length = 0;

		while ( tmp != NULL )
		{
			tmp += strspn( tmp, " \t\r\n" );

			switch ( tmp[0] )
			{
				case 't':
				case 'T':
					//Transfer-Encoding: trans_encoding
					if ( strncasecmp( &(tmp[1]), "ransfer-Encoding", 16 ) == 0 )
						db->response.trans_encoding = &(tmp[17+strspn( &(tmp[17]), " :" )]);
				break;
				case 'c':
				case 'C':
					//Content-length: content_length
					if ( strncasecmp( &(tmp[1]), "ontent-length", 13 ) == 0 )
						db->response.content_length = atol( &(tmp[14+strspn( &(tmp[14]), " :" )]) );
					//Content-type: -
					//Connection: -
				break;
				/*
				case 'w':
				case 'W':
					//WWW-Authenticate: -
				break;
				*/
				case 's':
				case 'S':
					//Set-Cookie: cookie
					if ( strncasecmp( &(tmp[1]), "et-Cookie", 9 ) == 0 )
					{
						db->response.cookie = &(tmp[10+strspn( &(tmp[10]), " :" )]);
						if ( memcmp( db->response.cookie, "SQLSESSION=", 11 ) == 0 )
							db->response.cookie += 11;
						else
							db->response.cookie = NULL;
					}
				break;
			}

			tmp = strchr( tmp, '\n' );
		}
		db->response.header_end = 1;
	}
    if (db->response.header_end == 1)
		db->response.body = &(db->recv_buf[db->response.header_len]);

	if ( db->response.trans_encoding != NULL && strncasecmp( db->response.trans_encoding, "Chunked", 7 ) == 0 )
	{
		if ( db->response.chunk_offset == NULL )
		{
			db->response.chunk_length = 0;
			db->response.chunk_offset = db->response.body;
		}
		_sqlited_chunk_decode( db );
	}
	else if ( db->response.content_length > 0 && db->recv_length - db->response.header_len >= db->response.content_length )
		db->response.end = 1;

	if ( db->response.end == 1 )
		return SQLITED_RESPONSE_END;

	return SQLITED_OK;
}

void _sqlited_chunk_decode( sqlited *db )
{
	sqlited_int64	idx;
	sqlited_int64	chunk;
	int		chunk_len;
	char		*tmp;

	if ( db->response.chunk_offset == NULL )
		return;

	if ( memcmp( &(db->recv_buf[db->recv_length-7]), "\r\n0\r\n\r\n", 7 ) == 0 )
	{
		db->response.content_length = idx = 0;
		db->recv_buf[db->recv_length] = '\0';
		chunk = strtoll( db->response.body, &tmp, 16 );
		while ( chunk > 0 && tmp != NULL && idx < db->recv_length )
		{
			memmove( &(db->response.body[db->response.content_length]), &(tmp[2]), chunk );
			for ( chunk_len=0; &(db->response.body[idx+chunk_len]) != tmp ; chunk_len++)
			{}
			db->response.content_length += chunk;
			idx += (chunk+chunk_len+2);
			chunk = strtoll( &(db->response.body[idx]), &tmp, 16 );
		}
		db->response.body[db->response.content_length] = '\0';
		db->response.end = 1;
		return;
	}

	db->response.chunk_length = db->recv_length;
	return;
}

struct sqlited_result *_sqlited_response_parse( sqlited *db )
{
	struct sqlited_result	*result;

	_sqlited_request_session_init( db );

	if ( _sqlited_response_parse_msg( db ) == SQLITED_OK )
	{
		db->msg.status = db->response.status;
		return &(db->msg);
	}

	db->msg.msg = NULL;
	if ( db->response.status != 200 )
	{
		db->errmsg = db->response.msg;//"Unknown error.";
		return NULL;
	}

	if ( (result = _sqlited_response_parse_header( db )) == NULL )
	{
		db->errmsg = "Response is not valid.";
		return NULL;
	}

	_sqlited_response_parse_column( result );

	/* need to free */
	if ( result->rows > 0 )
	{
		if ( _sqlited_response_parse_data( result ) == SQLITED_ERROR )
		{
			sqlited_free_result( result );
			db->errmsg = "Response is not valid.";
			return NULL;
		}
	}

	return result;
}

int _sqlited_response_parse_msg( sqlited *db )
{
	char	*msg;
	char	*tmp;

	msg = strstr( db->response.body, "<info>" );
	if ( msg == NULL )
		msg = strstr( db->response.body, "<error>" );
	if ( msg == NULL )
		return SQLITED_EMPTY;

	msg = strstr( msg, "<message>" );
	if ( msg == NULL )
		return SQLITED_EMPTY;

	msg += 9;
	tmp = strstr( msg, "</message>" );
	if ( tmp == NULL )
		return SQLITED_EMPTY;

	tmp[0] = '\0';

	db->msg.msg = _sqlited_html_entity_decode( msg );
	return SQLITED_OK;
}

struct sqlited_result *_sqlited_response_parse_header( sqlited *db )
{
	char			*msg;
	struct sqlited_result	*result;
	sqlited_result		*find = NULL;

	result = malloc( sizeof(struct sqlited_result) );
	memset( result, 0, sizeof(struct sqlited_result) );
	result->db = db;
	result->status = db->response.status;

	if ( db->result == NULL )
		db->result = result;
	else
	{
		for ( find=db->result; find; find=find->next )
		{
			if ( find->next == NULL )
			{
				find->next = result;
				break;
			}
		}
	}

	result->raw = malloc( db->response.content_length+1 );
	if ( result->raw == NULL )
	{
		free( result );
        if (find)
            find->next = NULL;
		return NULL;
	}
	memcpy( result->raw, db->response.body, db->response.content_length );
	result->raw[db->response.content_length] = '\0';

	msg = strstr( result->raw, "<response>" );
	if ( msg == NULL )
	{
		free( result->raw );
		free( result );
        if (find)
            find->next = NULL;
		return NULL;
	}
	msg += 10 + strspn( &(msg[10]), " \r\n\t" );

	if ( sscanf( msg, "<changes>%llu</changes><rowid>%llu</rowid><rows>%llu</rows><time>%f</time>", &(result->changes), &(result->last_row_id), &(result->rows), &(result->time) ) != 4 )
	{
		free( result->raw );
		free( result );
        if (find)
            find->next = NULL;
		return NULL;
	}

	db->last_row_id = result->last_row_id;
	db->changes = result->changes;
	db->total_changes += result->changes;

	return result;
}

int _sqlited_response_parse_column( struct sqlited_result *result )
{
	char		*start;
	char		*end;
	char		*column;

	result->fields = 0;

	start = strstr( result->raw, "<columns>" );
	if ( start == NULL )
	{
		result->raw_offset = result->raw;
		return SQLITED_EMPTY;
	}
	start += 9;

	end = strstr( start, "</columns>" );
	if ( end == NULL )
	{
		result->raw_offset = result->raw;
		return SQLITED_EMPTY;
	}
	end[0] = '\0';
	result->raw_offset = end+10;

	result->field = malloc( sizeof(char *) * 20 );

	while ( (column = strstr( start, "<columnname>" )) != NULL )
	{
		result->fields++;
		if ( result->fields >= 20 ) {
			result->field = realloc( result->field, sizeof(char *) * result->fields );
        }
		end = strstr( &(column[12]), "</columnname>" );
		if ( end == NULL )
		{
			start = &(column[12]);
			continue;
		}
		end[0] = '\0';
		result->field[result->fields-1] = _sqlited_html_entity_decode( &(column[12]) );
		start = &(end[13]);
	}

	return SQLITED_OK;
}

int _sqlited_response_parse_data( struct sqlited_result *result )
{
	char		*start;
	char		*row;
	char		*row_end;
	char		*data;
	char		*data_end;
	sqlited_int64	rows = 0;
	unsigned int	fields;

	if ( result->raw_offset == NULL )
		return SQLITED_ERROR;

	start = result->raw_offset;
	result->table = malloc( sizeof(char **) * result->rows );
    memset(result->table, 0, sizeof(char **) * result->rows);
	while ( (row = strstr( start, "<row>" )) != NULL && rows < result->rows )
	{
		row_end = strstr( row, "</row>" );
		if ( row_end == NULL )
		{
			rows++;
			continue;
		}
		row_end[0]='\0';
		start = &(row_end[6]);

		fields = 0;
		data = &(row[5]);
		result->table[rows] = malloc( sizeof(char *) * result->fields );
		while ( (data = strstr( data, "<data>" )) != NULL && fields < result->fields )
		{
			data_end = strstr( data, "</data>" );
			if ( data_end == NULL )
			{
				data += 6;
				continue;
			}
			data_end[0] = '\0';

			result->table[rows][fields] = _sqlited_html_entity_decode( &(data[6]) );

			data = &(data_end[7]);
			fields++;
		}
		rows++;
	}

	return SQLITED_OK;
}

char *_sqlited_html_entity_decode( char *buf )
{
	sqlited_int64	len;
	int		idx;
	int		i;

	if ( buf == NULL )
		return buf;

	len = strlen( buf );
	for ( i=idx=0; i < len; i++, idx++ )
	{
		if ( buf[i] == '&' && i < len-3 )
		{
			switch ( buf[i+1] )
			{
				case 'l':
					if ( buf[i+2] == 't' && buf[i+3] == ';' )
					{
						buf[idx] = '<';
						i += 3;
						goto next_loop;
					}
				break;
				case 'g':
					if ( buf[i+2] == 't' && buf[i+3] == ';' )
					{
						buf[idx] = '>';
						i += 3;
						goto next_loop;
					}
				break;
				case 'q':
					if ( i < len-5 && buf[i+2] == 'u' && buf[i+3] == 'o' && buf[i+4] == 't' && buf[i+5] == ';' )
					{
						buf[idx] = '"';
						i += 5;
						goto next_loop;
					}
				break;
				case '#':
					if ( i < len-5 && buf[i+2] == '0' && buf[i+3] == '3' && buf[i+4] == '9' && buf[i+5] == ';' )
					{
						buf[idx] = '\'';
						i += 5;
						goto next_loop;
					}
				break;
			}
		}
		if ( i != idx )
			buf[idx] = buf[i];
next_loop:
		continue;
	}

	buf[idx] = '\0';

	return buf;
}
