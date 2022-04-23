#include "sqlited.h"
#include "sqlited_internal.h"

int sqlited_open( sqlited **db, const char *conninfo )
{
	extern int	errno;
	extern char	*sqlited_error;

	sqlited		*pDb;
	int		result;
	sqlited_result	*version;
	char		*tmp;
	int		len;

	(*db) = (sqlited *) malloc( sizeof( sqlited ) );
	if ( (*db) == NULL )
		return SQLITED_NOMEM;

	pDb = (*db);
	memset( pDb, 0, sizeof( sqlited ) );
	pDb->msg.type = SQLITED_RESULT_MSG;

	result = _sqlited_parse_conninfo( conninfo, pDb );
	switch ( result )
	{
		case SQLITED_OK:
		break;
		case SQLITED_NO_SSL:
			errno = EFAULT;
			sqlited_error = "libsqlited client library is not support for SSL";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		break;
		case SQLITED_BAD_FORMAT:
			errno = EFAULT;
			sqlited_error = "Connection information is not valid";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		break;
		case SQLITED_NOMEM:
			errno = ENOMEM;
			sqlited_error = strerror( errno );
			sqlited_close( pDb );
			return SQLITED_ERROR;
		break;
	}

	if ( _sqlited_make_auth_str( pDb ) != SQLITED_OK )
	{
		errno = EFAULT;
		sqlited_error = "Connection information is not valid";
		sqlited_close( pDb );
		return SQLITED_ERROR;
	}

	if ( ( pDb->hostent = gethostbyname( pDb->host ) ) == NULL )
	{
		sqlited_close( pDb );
		sqlited_error = strerror( errno );
		return SQLITED_ERROR;
	}

	if ( ( pDb->sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		sqlited_close( pDb );
		sqlited_error = strerror( errno );
		return SQLITED_ERROR;
	}

	pDb->server_addr.sin_family = AF_INET;
	pDb->server_addr.sin_port = htons( pDb->port );
	pDb->server_addr.sin_addr = *((struct in_addr *)pDb->hostent->h_addr);

	if ( connect( pDb->sock, (struct sockaddr *)&pDb->server_addr, sizeof(struct sockaddr) ) == -1 )
	{
		sqlited_close( pDb );
		sqlited_error = strerror( errno );
		return SQLITED_ERROR;
        }

#ifdef	ENABLE_SSL
	if ( pDb->use_ssl == 1 )
	{
		SSL_library_init();
		pDb->ssl_ctx = SSL_CTX_new( SSLv23_client_method() );
		if ( pDb->ssl_ctx == NULL )
		{
			sqlited_error = "SSL init failed(1)";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		}
		
		pDb->ssl = SSL_new( pDb->ssl_ctx );
		if ( pDb->ssl == NULL )
		{
			sqlited_error = "SSL init failed(2)";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		}

		SSL_use_certificate_file( pDb->ssl, "/sw/local/sqlitedbms/conf/cert.pem", SSL_FILETYPE_PEM );
		SSL_use_PrivateKey_file( pDb->ssl, "/sw/local/sqlitedbms/conf/key.pem", SSL_FILETYPE_PEM );

		if ( SSL_set_fd( pDb->ssl, pDb->sock ) == 0 )
		{
			sqlited_error = "SSL init failed(3)";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		}

		result = SSL_connect( pDb->ssl );
		if ( result != 1 )
		{
			sqlited_error = "SSL connect failed";
			sqlited_close( pDb );
			return SQLITED_ERROR;
		}
	}
#endif

	version = sqlited_query( pDb, "SHOW VERSION" );
	if ( version == NULL )
	{
		_sqlited_seterr( pDb->errmsg );
		sqlited_close( pDb );
		return SQLITED_ERROR;
	}

	if ( version->status != 200 || version->type != SQLITED_RESULT_MSG )
	{
		if ( version->msg != NULL )
			_sqlited_seterr( version->msg );
		else
			sqlited_error = "Unknown error.";
		sqlited_close( pDb );
		return SQLITED_ERROR;
	}

	strncpy( pDb->version_sqlite3, sqlited_fetch_msg( version ), 16 );
	if ( pDb->response.cookie != NULL )
	{
		tmp = strstr( pDb->response.cookie, "; expire=" );
		if ( tmp == NULL )
			pDb->response.cookie = NULL;
		else
		{
			tmp[0] = '\0';
			len = strlen( pDb->response.cookie );
			pDb->session_key = malloc( len + 1 );
			strncpy( pDb->session_key, pDb->response.cookie, len );
		}
	}

	return SQLITED_OK;
}

void sqlited_close( sqlited *db )
{
	sqlited_result	*res;
	sqlited_result	*next;

	if ( db == NULL )
		return;

	if ( db->recv_buf != NULL )
		free( db->recv_buf );
	if ( db->send_buf != NULL )
		free( db->send_buf );
	if ( db->conninfo != NULL )
		free( db->conninfo );
	if ( db->auth_str != NULL )
		free( db->auth_str );
	if ( db->sock > 0 )
		close( db->sock );
	if ( db->result != NULL )
	{
		res = db->result;
		while ( (next = res->next) != NULL )
		{
			free( res );
			res = next;
		}
		free( res );
	}
	free( db );
	db = NULL;
}

int _sqlited_parse_conninfo( const char *conninfo, sqlited *db )
{
	char	*tmp;
	size_t	length;

	length = strlen( conninfo );
	db->conninfo = (char *) malloc( length+1 );
	if ( db->conninfo == NULL )
		return SQLITED_NOMEM;

	tmp = strstr( conninfo, "://" );
	if ( tmp != NULL )
	{
		if ( strncasecmp( conninfo, "https://", 7 ) == 0 )
		{
			db->use_ssl = 1;
			db->protocol = "HTTPS";
		}
		conninfo = tmp+3;
	}

#ifndef	ENABLE_SSL	
	if ( db->use_ssl == 1 )
	{
		free( db->conninfo );
		db->conninfo = NULL;
		return SQLITED_NO_SSL;
	}
#endif

	memcpy( db->conninfo, conninfo, length );
	db->conninfo[length] = '\0';

	db->host = strchr( db->conninfo, '@' );
	if ( db->host == NULL )
	{
		free( db->conninfo );
		db->conninfo = NULL;
		return SQLITED_BAD_FORMAT;
	}
	db->host[0] = '\0';
	db->host++;

	tmp = strchr( db->host, ':' );
	if ( tmp == NULL )
	{
		db->port = SQLITED_DEF_PORT;
		db->dbname = strchr( db->host, '/' );
	}
	else
	{
		db->dbname = strchr( tmp, '/' );
		tmp[0] = '\0';
		db->port = atoi( ++tmp );
	}

	if ( db->dbname == NULL )
	{
		free( db->conninfo );
		db->conninfo = NULL;
		return SQLITED_BAD_FORMAT;
	}
	db->dbname[0] = '\0';
	db->dbname++;

	db->username = db->conninfo;
	db->passwd = strchr( db->username, ':' );
	if ( db->passwd == NULL )
	{
		free( db->conninfo );
		db->conninfo = NULL;
		return SQLITED_BAD_FORMAT;
	}
	db->passwd[0] = '\0';
	db->passwd++;

	return SQLITED_OK;
}

static char base64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

int _sqlited_make_auth_str( sqlited *db )
{
	char	*tmp;
	char	*src;
	size_t	length;
	int	i = 0;

	length = strlen( db->username ) + 2 + strlen( db->passwd );
	tmp = (char *) malloc( length );
	if ( tmp == NULL )
		return SQLITED_ERROR;

	snprintf( tmp, length, "%s:%s", db->username, db->passwd );
	src = tmp;

	db->auth_str = (char *) malloc( ((length + 3 - length % 3) * 4 / 3 + 1) );
	if ( db->auth_str == NULL )
	{
		free( tmp );
		return SQLITED_ERROR;
	}

	length--;

	while (length > 2) {
		db->auth_str[i++] = base64_table[src[0] >> 2];
		db->auth_str[i++] = base64_table[((src[0] & 0x03) << 4) + (src[1] >> 4)];
		db->auth_str[i++] = base64_table[((src[1] & 0x0f) << 2) + (src[2] >> 6)];
		db->auth_str[i++] = base64_table[src[2] & 0x3f];

		src += 3;
		length -= 3;
	}

	switch ( length )
	{
		case 2:
			db->auth_str[i++] = base64_table[src[0] >> 2];
			db->auth_str[i++] = base64_table[((src[0] & 0x03) << 4) + (src[1] >> 4)];
			db->auth_str[i++] = base64_table[(src[1] & 0x0f) << 2];
			db->auth_str[i++] = '=';
		break;
		case 1:
			db->auth_str[i++] = base64_table[src[0] >> 2];
			db->auth_str[i++] = base64_table[(src[0] & 0x03) << 4];
			db->auth_str[i++] = '=';
			db->auth_str[i++] = '=';
		break;
	}

	db->auth_str[i] = '\0';
	free( tmp );

	return SQLITED_OK;
}
