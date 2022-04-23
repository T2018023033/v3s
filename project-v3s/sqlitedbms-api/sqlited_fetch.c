#include "sqlited.h"
#include "sqlited_internal.h"

short sqlited_result_type( sqlited_result *result )
{
	if ( result == NULL )
		return SQLITED_RESULT_EMTPY;
	return result->type;
}

sqlited_int64 sqlited_last_insert_rowid( sqlited *db )
{
	if ( db == NULL )
		return 0;
	return db->last_row_id;
}

sqlited_int64 sqlited_changes( sqlited *db )
{
	if ( db == NULL )
		return 0;
	return db->changes;
}

sqlited_int64 sqlited_total_changes( sqlited *db )
{
	if ( db == NULL )
		return 0;
	return db->total_changes;
}

sqlited_int64 sqlited_num_rows( sqlited_result *result )
{
	if ( result == NULL )
		return 0;
	return result->rows;
}

unsigned int sqlited_num_fields( sqlited_result *result )
{
	if ( result == NULL )
		return 0;
	return result->fields;
}

const char *sqlited_field_name( sqlited_result *result, unsigned int idx )
{
	if ( result == NULL )
		return "";
	if ( idx >= result->fields )
		return "";
	return result->field[idx];
}

int sqlited_fetch_all( sqlited_result *result, char ****resultp, sqlited_int64 *nrow, unsigned int *nfield )
{
	if ( result == NULL )
		return SQLITED_ERROR;
	if ( resultp != NULL )
		*resultp = result->table;
	if ( nrow != NULL )
		(*nrow) = result->rows;
	if ( nfield != NULL )
		(*nfield) = result->fields;
	return SQLITED_OK;
}

sqlited_int64 sqlited_row_seek( sqlited_result *result, sqlited_int64 seek )
{
	result->seek_row = seek;
	return result->seek_row;
}

const char **sqlited_fetch_row( sqlited_result *result )
{
	const char	**data;

	if ( result->seek_row >= result->rows )
		return NULL;
	data = (const char **) (result->table[result->seek_row]);
	result->seek_row++;
	return data;
}

const char *sqlited_fetch_msg( struct sqlited_result *result )
{
	if ( result == NULL )
		return "";
	return result->msg;
}

const char *sqlited_fetch_result( sqlited_result *result, sqlited_int64 seek, unsigned int field )
{
	if ( seek >= result->rows || field >= result->fields )
		return NULL;
	return result->table[seek][field];
}

void sqlited_free_result( struct sqlited_result *result )
{
	sqlited_result	*res;
	sqlited_result	*prev = NULL;

	if ( result == NULL || result->type == SQLITED_RESULT_MSG )
		return;

	if ( result->sql != NULL )
		free( result->sql );

	if ( result->field != NULL )
		free( result->field );

	if ( result->table != NULL )
	{
		while ( result->rows > 0 )
		{
			result->rows--;
			if ( result->table[result->rows] != NULL )
				free( result->table[result->rows] );
		}
		free( result->table );
	}

	if ( result->raw != NULL )
		free( result->raw );

	for ( res = result->db->result; res; res=res->next )
	{
		if ( res == result )
		{
			if ( prev )
				prev->next = res->next;
			else
				result->db->result = res->next;
			free( res );
            break;
		}
		prev = res;
		/* Patched by http://sourceforge.net/tracker/index.php?func=detail&aid=1593750&group_id=106881&atid=645895 */
		if(!result->db->result) break;
	}
}
