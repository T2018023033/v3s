#include "client_api.h"
#include "string.h"
/**
 * @brief 数据库连接
 * @param  filename     连接的数据库名字
 * @return SQL_HANDLE   成功：返回句柄 失败：NULL
 */
SQL_HANDLE sqlConn(const char *filename)
{
    SQL_HANDLE handle;
    int ret = -1;

    if(filename == NULL)
        filename = ":memory:";
    ret = sqlite3_open(filename, &handle);
    if(ret != SQLITE_OK){
        printf("Open database error ! \r\n");
        return NULL;
    }
    else
        printf("Open database succeeded ! \r\n");
    
    return handle;
}

/**
 * @brief 数据库关闭
 * @param  handle     数据库句柄
 */
void sqlClose(SQL_HANDLE handle)
{
    // if(sqlite3_close(handle) != SQLITE_OK)
    //     printf("Close database error ! \r\n");
    // else
    //     printf("Close database succeeded ! \r\n");
    sqlite3_close(handle);
}
/**
 * @brief 执行SQL语句
 * @param  handle          数据库句柄
 * @param  query           执行的命令
 * @return int             成功：0 失败：-1
 */
int sqlQuery_run(SQL_HANDLE handle, char *query)
{
#if 0
    int ret = -1;
    char *msg = NULL;

    ret = sqlite3_exec(handle, query, NULL, NULL, &msg);
    if(ret != SQLITE_OK){
        printf("Error: %s \r\n", msg);
        return -1;
    }

#endif

#if 1
    sqlite3_stmt *stmt = NULL;
    int rc = -1;

    sqlite3_prepare_v2(handle, query, strlen(query), &stmt, NULL);
    do{  
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_DONE) break; 
    }while(1);

#endif

    return 0;
}
/**
 * @brief 查询数据库的版本
 * @param  handle          传入的数据库句柄
 * @return const char*     返回的版本号
 */
const char *sqlVerGet(SQL_HANDLE handle)
{   
    return sqlite3_libversion();
}

/**
 * @brief 查询表中的数据
 * @param  handle           传入的数据库句柄
 * @param  querySql         查询sql语句
 * @param  tbInfo           传出的数据
 * @return int              成功：0  失败：-1
 */
int sqlQueryData(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo)
{
    int row=0,column=0, i=0;
    int rc = -1;
    char *value = {0};
    sqlite3_stmt *stmt = NULL;

    tbInfo->table = (char ***)sqlite3_malloc(4096*4096);
    sqlite3_prepare_v2(handle, querySql, strlen(querySql), &stmt, NULL);
    column = sqlite3_column_count(stmt);
    do{  
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_ROW) {
            tbInfo->table[row] = (char **)sqlite3_malloc(column*sizeof(char*)); 
            for(i = 0 ; i < column ; i++ )  {
                value = (char *)sqlite3_column_blob(stmt , i);
                tbInfo->table[row][i] = (char *)sqlite3_malloc(strlen(value)); 
                sprintf(tbInfo->table[row][i], "%s", value);
            }
        } 
        else if(rc == SQLITE_DONE) break; 
        else { 
            printf("Query data error !\r\n"); 
            sqlite3_finalize(stmt); 
            break;
        }
        row++;
    }while(1); 

    tbInfo->nrows = row;
    tbInfo->nfields = column;

    sqlite3_finalize(stmt);
    return 0;
}

/**
 * @brief 释放数据表的内存 注：每次查询完都要释放内存
 * @param  tbInfo          传入查询的数据表结构体
 * @return int             成功：0  失败：-1
 */
int sqlTbQueryFree(SQL_TABLE_S tbInfo)
{
    int i, j;

    for(i=0; i<tbInfo.nrows; i++){
        for(j=0; j<tbInfo.nfields; j++)
            sqlite3_free(tbInfo.table[i][j]);
        sqlite3_free(tbInfo.table[i]);
    }
    sqlite3_free(tbInfo.table);

    return 0;
}

/**
 * @brief 创建表
 * @param  handle           输入的数据库句柄
 * @param  tbName           表名字
 * @param  fields           创建表的字段
 * @return int              成功： 0 失败：-1
 */
int sqlTbCreate(SQL_HANDLE handle, char *tbName, SQL_TABLE_FIELD_S *fields)
{
    int i = 0;
    char query[512], desc[256];
    char str[128];

    memset(query, 0, 512);
    memset(desc, 0, 256);
    for(i=0; i<fields->nfields; i++){
        sprintf(str,"%s %s", fields->fieldName[i],fields->fieldType[i]);
        strcat(desc, str);
        if(i<fields->nfields-1) strcat(desc, ",");
        memset(str,0, 128);
    }
    snprintf(query, sizeof(query), "CREATE TABLE %s (%s);",tbName,desc);
    if(sqlQuery_run(handle, query) != 0){
        printf("Create table failed !!! \r\n");
        return -1;
    }
    
    return 0;
}

/**
 * @brief 删除表
 * @param  handle           传入的数据库句柄
 * @param  tbName           删除的表名字
 * @return int              成功：0 失败：-1
 */
int sqlTbDrop(SQL_HANDLE handle, char *tbName)
{
    char msg[128];

    sprintf(msg, "drop table %s;", tbName);
    if(sqlQuery_run(handle, msg) != 0)
        return -1;

    return 0;
}

/**
 * @brief 数据表内容普通插入
 * @param  handle           当前连接数据库句柄
 * @param  tbName           数据表名
 * @param  tbInfo           插入数据信息, 包括行和列
 * @return int              0: 成功 非0: 失败
 */
int sqlTbInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo)
{   
    int i, j;
    char query[512];
    char src[64], desc[256];

    for(i=0; i<tbInfo->nrows; i++){
        memset(query, 0, 512);
        memset(desc, 0,256); 
        for(j=0; j<tbInfo->nfields; j++){
            memset(src,0,64);
            snprintf(src, 64, "' %s'", tbInfo->table[i][j]);
            strcat(desc,src);
            if(j<tbInfo->nfields-1) strcat(desc,",");
        }
        sprintf(query, "INSERT INTO %s VALUES(%s);",tbName,desc);
        if(sqlQuery_run(handle,query) != 0) return -1;
    }

    return 0;
}

/**
 * @brief        返回一句完整的插入语句 仅供内部调用，不开放
 * @param tbName  数据库表名字
 * @param tbInfo  传入的数据
 * @param rows    传入的行号
 * @param resSql  传出的一句完整的插入语句
 */
void sqlTbInsertTemp(char *tbName, SQL_TABLE_S *tbInfo, int rows,  char *resSql)
{
    int i;
    char src[1024], desc[1024];

    memset(desc, 0,1024); 
    for(i=0; i<tbInfo->nfields; i++){
        memset(src,0,1024);
        snprintf(src, sizeof(src), "'%s'", tbInfo->table[rows][i]);
        strcat(desc,src);
        if(i<tbInfo->nfields-1) strcat(desc,",");
    }
    sprintf(resSql, "INSERT INTO %s VALUES(%s);",tbName,desc);
}

/**
 * @brief 数据表内容批量插入(事务型插入)
 * @param  handle           当前连接数据库句柄
 * @param  tbName           数据表名
 * @param  tbInfo           插入数据信息, 包括行和列
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo, int isSync, int step) 
{
    char *str;
    int i, k;
    char resSql[1024];
    int rows = tbInfo->nrows;

    if(tbInfo->nrows < step){
        printf("step is bigger than nrows error !!! \r\n");
        return -1;
    } 
    if(isSync){
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF") != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)sqlite3_malloc(1024*step+1);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str,0, (1024*step+1));
    sqlQuery_run(handle, "begin");
    k=0;
    for(i=0; i<tbInfo->nrows; i++){
        sqlTbInsertTemp(tbName, tbInfo, i,  resSql);
        k++;
        if(k<step){
            sprintf(str+ strlen(str),"%s", resSql);
            if(i>(rows - rows % step)) sqlQuery_run(handle, str);
        }
        if(k == step){
            sprintf(str+ strlen(str),"%s",resSql);
            sqlQuery_run(handle, str);
            memset(str, 0, (1024*step+1));
            k=0;
        }
    }
    sqlQuery_run(handle, "commit");

    sqlite3_free(str);

    return 0;
}

/**
 * @brief 数据表内容普通修改
 * @param  handle           当前连接数据库句柄
 * @param  modifySql        普通修改 sql 语句
 * @return int              0: 成功 非0: 失败
 */
int sqlTbModify(SQL_HANDLE handle, char *modifySql)
{

    if(sqlQuery_run(handle, modifySql) != 0){
        printf("Update failed  !!! \r\n");
        return -1;
    }

    return 0;
}

/**
 * @brief 数据表内容批量修改(事务型修改)
 * @param  handle           当前连接数据库句柄
 * @param  modifySql        修改 sql 语句
 * @param  modifyCnt        修改 sql 语句条数
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchModify(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step)
{
    char *str;
    int i, j;

    if(modifyCnt < step){
        printf("step is bigger than modifyCnt error !!! \r\n");
        return -1;
    } 
    if(isSync){
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF") != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)sqlite3_malloc(512*step);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str,0, (512*step));
    sqlQuery_run(handle, "begin");
    j=0;
    for(i=0; i<modifyCnt; i++){
        j++;
        if(j<step){
            sprintf(str+ strlen(str),"%s;",modifySql[i]);
            if(i>(modifyCnt-modifyCnt%step)){
                sqlQuery_run(handle, str);
                memset(str,0, (512*step));    
            }
        }
        if(j == step){
            sprintf(str+ strlen(str),"%s;",modifySql[i]);
            sqlQuery_run(handle, str);
            memset(str,0, (512*step));
            j=0;
        }
        
    }
    sqlQuery_run(handle, "commit");
    sqlite3_free(str);

    return 0;
}

/**
 * @brief 数据表内容普通删除
 * @param  handle           当前连接数据库句柄
 * @param  delSql           删除 sql 语句
 * @return int              0: 成功 非0: 失败
 */
int sqlTbDel(SQL_HANDLE handle, char *delSql)
{

    if(sqlQuery_run(handle, delSql) != 0){
        printf("Delete table data failed !!! \r\n");
        return -1;
    }

    return 0;
}

/**
 * @brief 数据表内容批量删除(事务型删除)
 * @param  handle           当前连接数据库句柄
 * @param  delSql           删除 sql 语句
 * @param  delCnt           删除 sql 语句条数
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchDel(SQL_HANDLE handle, char **delSql, int delCnt, int isSync, int step)
{
    char *str;
    int i, j;

    if(delCnt < step){
        printf("step is bigger than delCnt error !!! \r\n");
        return -1;
    } 
    if(isSync){
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF") != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)sqlite3_malloc(128*step);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str, 0, 128*step);
    sqlQuery_run(handle, "begin");
    j=0;
    for(i=0; i<delCnt; i++){
        j++;
        if(j<step){
            sprintf(str+ strlen(str),"%s;",delSql[i]);
            if(i>(delCnt-delCnt%step)){
                sqlQuery_run(handle, str);
                memset(str,0, 128*step);    
            }
        }
        if(j == step){
            sprintf(str+ strlen(str),"%s;",delSql[i]);
            sqlQuery_run(handle, str);
            memset(str,0, 128*step);
            j=0;
        } 
    }
    sqlQuery_run(handle, "commit");

    sqlite3_free(str);
    return 0;
}

/**
 * @brief 备份数据库
 * @param  handle           传入当前的数据库句柄
 * @param  dbName           备份的数据库名字
 * @return int              成功：0   失败：-1
 */
int sqlDbBackUp(SQL_HANDLE handle, char *dbName)
{
    char msg[128];

    sprintf(msg, "BACKUP DATABASE %s", dbName);
    if(sqlQuery_run(handle, msg) != 0)
        return -1;

    return 0;
}

/**
 * @brief 还原数据库
 * @param  handle           传入当前数据库的句柄
 * @param  dbName           还原的数据库名字
 * @return int              成功： 0  失败：-1
 */ 
int sqlDbRestore(SQL_HANDLE handle, char *dbName)
{
    char msg[128];

    sprintf(msg, "RESTORE DATABASE %s", dbName);
    if(sqlQuery_run(handle, msg) != 0)
        return -1;
    
    return 0;
}

int sqlDbCreate(SQL_HANDLE handle, char *dbName)
{
    char msg[128];

    sprintf(msg, "CREATE  DATABASE %s;", dbName);
    if(sqlQuery_run(handle, msg) != 0)
        return -1;
    
    return 0;
}

int sqlDbDrop(SQL_HANDLE handle,  char *dbName)
{
    char msg[128];

    sprintf(msg, "DROP DATABASE %s;", dbName);
    if(sqlQuery_run(handle, msg) != 0)
        return -1;
    
    return 0;
} 

/*****************************************************************/
/*测试*/
/*****************************************************************/
int sqlTbBatchModify_test(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step)
{
    char plStr[128];
    int j;
    char *plCmd;

    plCmd = (char *)calloc(128 * 100, 1);
    if (!plCmd) {
        perror("malloc error");
        return -1;
    }

    sqlQuery_run(handle, "begin "); /* 启动事务, 插入操作先进行到内存 */
    for (j = 0; j < (modifyCnt / 100); j++) {
        memset(plCmd, 0, 100 * 128);
        
        snprintf(plStr, sizeof(plStr), "UPDATE infoTb SET randStr = 'test !' where SerialNum in (select SerialNum from infoTb limit %d offset %d)", (j-1)*100, j * 100);
        sprintf(plCmd + strlen(plCmd), "%s;", plStr);

        sqlQuery_run(handle, plCmd);
    }
    if (modifyCnt % 100) {
        memset(plCmd, 0, 100 * 128);
        
        snprintf(plStr, sizeof(plStr), "UPDATE infoTb SET randStr = 'test !' where SerialNum in (select SerialNum from infoTb limit %d offset %d)", (j-1)*100, j * 100);
        sprintf(plCmd + strlen(plCmd), "%s;", plStr);
       
        sqlQuery_run(handle, plCmd);
    }
    sqlQuery_run(handle, "commit "); /* 结束事务, commit 的时候内存再同步到磁盘 */
    
    free(plCmd);

    return 0;
}
