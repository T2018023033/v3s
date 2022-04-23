#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "sqlitedbms_client.h"

#define SQLITED_DEF_LOGIN_USER  "admin:password"//"zmd:zmd"  
#define SQLITED_DEF_PORT        (6543)

/**
 * @brief 数据库连接
 * @param  remoteIp         数据库远程 IP, 字符串输入, 如 "10.5.0.111"
 * @param  dbName           数据库名, 如 "zmd.db", 可为空, 自动连接":memory:" 数据库
 * @return SQL_HANDLE       返回NULL, 表示失败; 成功返回连接句柄
 * @note   可对多个数据库进行连接
 */
SQL_HANDLE sqlConn(char *remoteIp, char *dbName)
{
    char conninfo[128] = {0};
    SQL_HANDLE handle;

    if(dbName == NULL) dbName = ":memory:";
    snprintf(conninfo, sizeof(conninfo), "%s@%s:%d/%s", SQLITED_DEF_LOGIN_USER, remoteIp, SQLITED_DEF_PORT, dbName);
    if (sqlited_open(&handle, conninfo) != SQLITED_OK){
        printf("sqlite open failed !!! \n");
        return NULL;
    }
    else printf("sqlite connection succeeded !!! \n");

    return handle;
}

/**
 * @brief 数据库关闭
 * @param  handle           待断开连接的句柄
 */
void sqlClose(SQL_HANDLE handle)
{
    sqlited_close(handle);
}

/**
 * @brief 执行sql语句
 * 
 * @param handle        传入的句柄
 * @param query         执行的sql语句
 * @param msg_result    返回的信息
 * @return int          0:succeed -1:failed
 */
int sqlQuery_run(SQL_HANDLE handle, char *query, char *msg_result) /*执行sql语句*/
{
    int ret = 0;
    sqlited_result *result;

    result = sqlited_query(handle, query);
    if(result != NULL){
        switch (sqlited_result_type(result))
        {
        case SQLITED_RESULT:  
            ret = 0; 
            msg_result = NULL; 
            break;
        case SQLITED_RESULT_MSG: 
            ret = 0;
            strcpy(msg_result, sqlited_fetch_msg(result));
            break;
        default: 
            ret = -1;
            msg_result = NULL;
            printf("sqlited_query error !!! %s\n", sqlited_getmsg(handle));
            break;
        }
        sqlited_free_result(result);
    }

    return ret;
}

/**
 * @brief        备份数据库
 * 
 * @param handle 传入的数据库句柄
 * @param dbName 数据库名字
 * @return int   备份成功:0  备份失败:-1
 */
int sqlDbBackUp(SQL_HANDLE handle, char *dbName)
{
    char query[128];
    char msg[32];
    
    snprintf(query, sizeof(query), "BACKUP DATABASE %s", dbName);
    if (sqlQuery_run(handle, query, msg) != 0) {
        printf("BACKUP Database: %s \r\n error !!! \r\n", msg);
        return -1;
    }

    return 0;
}

/**
 * @brief 当前连续数据库恢复为备份数据库
 * @param  handle           当前连接数据库句柄
 * @param  dbName           数据库名
 * @return int              0: 成功 -1: 失败
 */
int sqlDbRestore(SQL_HANDLE handle, char *dbName)
{
    char query[128];
    char msg[32];

    snprintf(query, sizeof(query), "RESTORE DATABASE %s", dbName);
    if(sqlQuery_run(handle,query,msg) != 0){
        printf("error!!! RESTORE DATABASE: %s  \r\n", msg);
        return -1;
    }

    return 0;
}

/**
 * @brief 创建一个数据库 
 * 
 * @param handle 当前数据库句柄
 * @param dbName 要创建的数据库名字
 * @return int   0:成功  -1：失败
 */
int sqlDbCreate(SQL_HANDLE handle, char *dbName)
{
    char query[64];
    char msg[32];

    snprintf(query, sizeof(query), "create database %s", dbName);
    if(sqlQuery_run(handle,query,msg) != 0){
        printf("CREATE DATABASE: %s \r\n error \r\n", msg);
        return -1;
    }

    return 0;
}

/**
 * @brief 当前连接数据库删除, 删除后 handle 默认连接到 "::memory::" 数据库
 * @param  handle           当前连接数据库句柄
 * @param  dbName           数据库名字
 * @return int              0: 成功 非0: 失败
 */
int sqlDbDrop(SQL_HANDLE handle,  char *dbName)
{
    char query[128];
    char msg[32];

    snprintf(query, sizeof(query), "DROP DATABASE %s", dbName);
    if(sqlQuery_run(handle,query,msg) != 0){
        printf("DROP DATABASE: %s \r\n error \r\n", msg);
        return -1;
    }

    return 0;
}

/**
 * @brief 获取远程sqlite版本
 * @param  handle           当前连接数据库句柄
 * @return char*            NULL: 获取失败 其他: 版本字符串    
 */
#if 0
char* sqlVerGetm(SQL_HANDLE handle)
{
    char str[32] = {0};

    if(sqlQuery_run(handle, "SHOW VERSION", str) != 0){
        printf("Get Version failed !!! \r\n");
        return NULL;
    }

    return str;
}
#endif

/**
 * @brief 获取数据库版本信息
 * 
 * @param handle 当前数据库句柄
 * @param verMsg 数据库版本信息
 * @return int   0：成功  -1：失败
 */
int sqlVerGet(SQL_HANDLE handle, char *verMsg)
{
    if(sqlQuery_run(handle, "SHOW VERSION", verMsg) != 0){
        printf("Get Version failed !!! \r\n");
        return -1;
    }
    return 0;
}

/**
 * @brief 创建数据表
 * @param  handle           当前连接数据库句柄
 * @param  tbName           待创建数据库表名
 * @param  fields           待创建数据库字段信息
 * @return int              0: 成功 非0: 失败
 */
int sqlTbCreate(SQL_HANDLE handle, char *tbName, SQL_TABLE_FIELD_S *fields)
{
    int i = 0;
    char query[512], desc[256];
    char str[128], msg[32];

    memset(query, 0, sizeof(query));
    memset(desc, 0, sizeof(desc));
    for(i=0; i<fields->nfields; i++){
        sprintf(str,"%s %s", fields->fieldName[i],fields->fieldType[i]);
        strcat(desc, str);
        if(i<fields->nfields-1) strcat(desc, ",");
        memset(str,0, sizeof(str));
    }
    snprintf(query, sizeof(query), "CREATE TABLE %s (%s);",tbName,desc);
    if(sqlQuery_run(handle, query, msg) != 0){
        printf("CREATE TABLE:%s\r\n Create table failed !!! \r\n",msg);
        return -1;
    }
    
    return 0;
}

/**
 * @brief 删除数据表
 * @param  handle           当前连接数据库句柄
 * @param  tbName           待删除数据库表名
 * @return int              0: 成功 非0: 失败
 */
int sqlTbDrop(SQL_HANDLE handle, char *tbName)
{
    char query[128];
    char msg[32];

    snprintf(query, sizeof(query), "DROP TABLE %s",tbName);
    if(sqlQuery_run(handle,query,msg) != 0){
        printf("DROP Tale:%s \r\n error !!! \r\n", msg);
        return -1; 
    }
    printf("DROP Table succeeded !!! \r\n");

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
        memset(query, 0, sizeof(query));
        memset(desc, 0,sizeof(desc)); 
        for(j=0; j<tbInfo->nfields; j++){
            memset(src,0,sizeof(src));
            snprintf(src, sizeof(src), "' %s'", tbInfo->table[i][j]);
            strcat(desc,src);
            if(j<tbInfo->nfields-1) strcat(desc,",");
        }
        sprintf(query, "INSERT INTO %s VALUES(%s);",tbName,desc);
        if(sqlQuery_run(handle,query,NULL) != 0) return -1;
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

    memset(resSql, 0, sizeof(resSql));
    memset(desc, 0,sizeof(desc)); 
    for(i=0; i<tbInfo->nfields; i++){
        memset(src,0,sizeof(src));
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
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF", NULL) != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)calloc(1024*step+1,1);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str, 0, sizeof(str));
    sqlQuery_run(handle, "begin", NULL);
    k=0;
    for(i=0; i<tbInfo->nrows; i++){
        sqlTbInsertTemp(tbName, tbInfo, i,  resSql);
        k++;
        if(k<step){
            sprintf(str+ strlen(str),"%s", resSql);
            if(i>(rows - rows % step)) sqlQuery_run(handle, str, NULL);
        }
        if(k == step){
            sprintf(str+ strlen(str),"%s",resSql);
            sqlQuery_run(handle, str, NULL);
            memset(str,0, sizeof(str));
            k=0;
        }
    }
    sqlQuery_run(handle, "commit", NULL);
    free(str);
    return 0;
}

/**
 * @brief 数据表内容普通修改
 * @param  handle           当前连接数据库句柄
 * @param  modifySql        普通修改 sql 语句
 * @return int              0: 成功 非0: 失败
 */
int sqlTbMonify(SQL_HANDLE handle, char *modifySql)
{
    char msg[32];

    if(sqlQuery_run(handle, modifySql, msg) != 0){
        printf("Update failed: %s !!! \r\n", msg);
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
int sqlTbBatchMonify(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step)
{
    char *str;
    int i, j;

    if(modifyCnt < step){
        printf("step is bigger than modifyCnt error !!! \r\n");
        return -1;
    } 
    if(isSync){
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF", NULL) != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)calloc(512*step,1);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str, 0, sizeof(str));
    sqlQuery_run(handle, "begin", NULL);
    j=0;
    for(i=0; i<modifyCnt; i++){
        j++;
        if(j<step){
            sprintf(str+ strlen(str),"%s;",modifySql[i]);
            if(i>(modifyCnt-modifyCnt%step)){
                sqlQuery_run(handle, str, NULL);
                memset(str,0, sizeof(str));    
            }
        }
        if(j == step){
            sprintf(str+ strlen(str),"%s;",modifySql[i]);
            sqlQuery_run(handle, str, NULL);
            memset(str,0, sizeof(str));
            j=0;
        }
        
    }
    sqlQuery_run(handle, "commit", NULL);
    free(str);

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
    char msg[32];

    if(sqlQuery_run(handle, delSql, msg) != 0){
        printf("Delete failed: %s !!! \r\n", msg);
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
        if(sqlQuery_run(handle, "PRAGMA synchronous=OFF", NULL) != 0){
            printf("PRAGMA synchronous=OFF error !!! \r\n");
            return -1;
        }
    }
    str = (char *)calloc(128*step,1);
    if(!str){
        printf("malloc failed !!! \r\n");
        return -1;
    }
    else memset(str, 0, sizeof(str));
    sqlQuery_run(handle, "begin", NULL);
    j=0;
    for(i=0; i<delCnt; i++){
        j++;
        if(j<step){
            sprintf(str+ strlen(str),"%s;",delSql[i]);
            if(i>(delCnt-delCnt%step)){
                sqlQuery_run(handle, str, NULL);
                memset(str,0, sizeof(str));    
            }
        }
        if(j == step){
            sprintf(str+ strlen(str),"%s;",delSql[i]);
            sqlQuery_run(handle, str, NULL);
            memset(str,0, sizeof(str));
            j=0;
        } 
    }
    sqlQuery_run(handle, "commit", NULL);

    free(str);
    return 0;
}

/**
 * @brief 数据表内容查询   (不推荐使用)
 * @param  handle           当前连接数据库句柄
 * @param  querySql         查询 sql 语句
 * @param  tbInfo           返回查询到的数据内容
 * @return int              0: 成功 非0: 失败
 * @note   查询后, 必须执行查询 sqlTbQueryFree() 销毁malloc的内容
 */
int sqlTbQuery(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo)
{
    sqlited_result *result;

    result = sqlited_query(handle, querySql);
    if(result == NULL){
        printf("table query failed !!! \r\n");
        return -1;
    }
    if(sqlited_fetch_all(result, &tbInfo->table, &tbInfo->nrows, &tbInfo->nfields) != SQLITED_OK){
        printf("table query failed !!! \r\n");
        return -1;
    }
    sqlited_free_result(result);

    return 0;
}

/**
 * @brief 数据库查询  (推荐使用)   注:查询完成要调用:sqlTbQueryFree释放内存
 * @param handle    数据库句柄
 * @param querySql  查询语句
 * @param tbInfo    传出的数据
 * @return int      0 ：成功   -1：失败
 */
int sqlQueryData(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo)
{
    sqlited_result *result;
    const char **str;
    int i, j; 

    result = sqlited_query(handle, querySql);
    tbInfo->nrows = sqlited_num_rows(result);
    tbInfo->nfields = sqlited_num_fields(result);
    tbInfo->table = (char ***)malloc(sizeof(char **)*tbInfo->nrows); 
    for(i=0; i<tbInfo->nrows; i++){
        str = sqlited_fetch_row(result);
        tbInfo->table[i] = (char **)malloc(sizeof(char *)*tbInfo->nfields);
        for(j=0; j<tbInfo->nfields; j++){
            tbInfo->table[i][j] = (char *)malloc(1024);
            sprintf(tbInfo->table[i][j], "%s", str[j]); 
        }
    }
    
    sqlited_free_result(result);

    return 0;
}

/**
 * @brief 数据表查询后需要执行的一次销毁命令 释放内存
 * @param  handle           数据库句柄
 * @param  tbInfo           释放内存
 * @return int              0: 成功 非0: 失败
 */
int sqlTbQueryFree(SQL_HANDLE handle, SQL_TABLE_S tbInfo)
{
    int i, j;

    for(i=0; i<tbInfo.nrows; i++){
        for(j=0; j<tbInfo.nfields; j++)
            free(tbInfo.table[i][j]);
        free(tbInfo.table[i]);
    }
    free(tbInfo.table);
    //sqlited_close(handle);

    return 0;
}

//count:10000 data, time: 1779091 us
//TODO:  test
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

    sqlQuery_run(handle, "begin ", NULL); /* 启动事务, 插入操作先进行到内存 */
    for (j = 0; j < (modifyCnt / 100); j++) {
        memset(plCmd, 0, 100 * 128);
        snprintf(plStr, sizeof(plStr), "UPDATE infoTb SET randStr = 'test !' where SerialNum in (select SerialNum from infoTb limit %d offset %d)", ((j-1) * 100), j * 100);
        sprintf(plCmd + strlen(plCmd), "%s;", plStr);
        sqlQuery_run(handle, plCmd, NULL);
    }
    if (modifyCnt % 100) {
        memset(plCmd, 0, 100 * 128);
        snprintf(plStr, sizeof(plStr), "UPDATE infoTb SET randStr = 'test !' where SerialNum in (select SerialNum from infoTb limit %d offset %d)", ((j-1) * 100), j * 100);
        sprintf(plCmd + strlen(plCmd), "%s;", plStr);
        sqlQuery_run(handle, plCmd, NULL);
    }
    sqlQuery_run(handle, "commit ", NULL); /* 结束事务, commit 的时候内存再同步到磁盘 */
    
    free(plCmd);

    return 0;
}
