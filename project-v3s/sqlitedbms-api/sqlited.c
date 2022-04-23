#include "sqlited.h"
#include "sqlited_internal.h"

char *sqlited_error;
char _sqlited_error[1024];

void sqlited_PrintDataBlock(const char* pTitle, const uint8_t *pBuf, int32_t Size)
{
#define LINE_LEN 16
	int32_t i = 0, j = 0;

	printf("\n[%s]----------------------%d\n", pTitle, Size);
	for (i=0; i<Size; i++)
	{
		if (i == 0)
			printf("%08xh: ", i);
		else if (i % LINE_LEN == 0)
		{
			printf("; ");	
			for (j=LINE_LEN; j>0; j--)
			{
				if (isascii(pBuf[i - j]) && !iscntrl(pBuf[i - j]))
					printf("%c", pBuf[i - j]);
				else
					printf(".");
			}
			printf("\r\n%08xh: ", i);
		}
		printf("%02x ", pBuf[i]);
	}
	if (i != 0)
	{
		int32_t rem = (i - 1) % LINE_LEN + 1;

		for (j=0; j<(LINE_LEN - rem); j++)
		{
			printf("   ");	
		}
		printf("; ");	
		for (j=rem; j>0; j--)
		{
			if (isascii(pBuf[i - j]) && !iscntrl(pBuf[i - j]))
				printf("%c", pBuf[i - j]);
			else
				printf(".");
		}
	}

	printf("\r\n-----------------------\n\n");
}

const char *sqlited_libversion(void)
{
	return SQLITED_LIB_VERSION;
}

const char *sqlited_getmsg( sqlited *db )
{
	char	*msg;
	if ( db == NULL )
		return NULL;
	if ( db->msg.msg )
	{
		msg = db->msg.msg;
		db->msg.msg = NULL;
		return msg;
	}

	msg = db->errmsg;
	db->errmsg = NULL;
	return msg;
}

void _sqlited_seterr( char *errmsg )
{
	strncpy( _sqlited_error, errmsg, 1024 );
	sqlited_error = _sqlited_error;
}

void _sqlited_buf_init( sqlited *db, size_t max )
{
	if ( db->send_buf == NULL || db->send_buf_size <= 0 )
	{
		db->send_buf_size = MAXVAL( SQLITED_DEF_SEND_BUF, (max+1) * 3 );
		db->send_buf = (char *) malloc( db->send_buf_size );
	}

	if ( db->send_buf_size <= max * 3 )
	{
		db->send_buf_size = (max+1) * 3;
		db->send_buf = (char *) realloc( db->send_buf, db->send_buf_size );
	}
}
