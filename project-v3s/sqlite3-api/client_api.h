#ifndef __CLIENT_API__
#define __CLIENT_API__

#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
/*兼容c++*/
#ifdef __cplusplus
extern "C"
{
#endif

/* 数据库句柄 */
typedef sqlite3 *SQL_HANDLE;

/* 查询数据的数据结构 */
typedef struct
{
    char ***table;
    int nrows;
    int nfields;  
} SQL_TABLE_S;

/* 创建表的数据结构 */
typedef struct
{
    char **fieldType;
    char **fieldName;
    int nfields;
} SQL_TABLE_FIELD_S;

/**
 * @brief 数据库连接
 * @param  filename         要连接的数据库名字(可指定绝对路径, 为空连接:memory:)
 * @return SQL_HANDLE       传出的数据库句柄 
 */
SQL_HANDLE sqlConn(const char *filename);

/**
 * @brief 数据库关闭
 * @param  handle           传入数据库句柄
 */
void sqlClose(SQL_HANDLE handle);


/**
 * @brief 执行sql语句
 * @param  handle           传入数据库句柄
 * @param  query            sql语句
 * @return int             成功：0   失败：-1
 */
int sqlQuery_run(SQL_HANDLE handle, char *query);

/**
 * @brief 获取版本号
 * @param  handle           传入数据库句柄
 * @return const char*      传出的版本号
 */
const char *sqlVerGet(SQL_HANDLE handle); 

/**
 * @brief 查询表中的数据
 * @param  handle           传入的数据库句柄
 * @param  querySql         查询语句
 * @param  tbInfo           传出的数据
 * @return int              成功：0  失败：-1
 */
int sqlQueryData(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo);


/**
 * @brief 释放内存  注：查询完成后要释放数据表的内存
 * @param  tbInfo           存储数据的结构体
 * @return int              成功：0  失败：-1
 */
int sqlTbQueryFree(SQL_TABLE_S tbInfo);

/**
 * @brief 创建表
 * @param  handle           传入当前数据库句柄
 * @param  tbName           要创建表的名字
 * @param  fields           表的字段数据
 * @return int              成功：0  失败：-1
 */
int sqlTbCreate(SQL_HANDLE handle, char *tbName, SQL_TABLE_FIELD_S *fields);

/**
 * @brief 删除表
 * @param  handle           传入当前数据库的句柄
 * @param  tbName           要删除的表的名字
 * @return int              成功：0  失败：-1
 */
int sqlTbDrop(SQL_HANDLE handle, char *tbName); 

/**
 * @brief 插入数据
 * @param  handle           传入当前数据库的句柄
 * @param  tbName           表的名字
 * @param  tbInfo           插入数据内容
 * @return int              成功：0  失败：-1
 */
int sqlTbInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo);

/**
 * @brief 事务 批量插入数据
 * @param  handle           传入当前数据库句柄
 * @param  tbName           要插入表的名字
 * @param  tbInfo           插入数据信息
 * @param  isSync           是否同步 1：是   0：否
 * @param  step             一次插入的数据条数
 * @return int              成功： 0  失败：-1
 */
int sqlTbBatchInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo, int isSync, int step);

/**
 * @brief 修改表中的数据
 * @param  handle           传入当前数据库句柄
 * @param  modifySql        修改的sql语句
 * @return int              成功：0  失败：-1
 */
int sqlTbModify(SQL_HANDLE handle, char *modifySql);

/**
 * @brief 事务  批量修改数据
 * @param  handle           传入当前数据库句柄
 * @param  modifySql        修改语句
 * @param  modifyCnt        修改语句的条数
 * @param  isSync           是否同步 1：是  0：否
 * @param  step             一次执行的条数
 * @return int              成功：0  失败：-1
 */
int sqlTbBatchModify(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step);

/**
 * @brief 删除表中的数据
 * @param  handle           传入当前数据库的句柄
 * @param  delSql           删除的sql语句
 * @return int              成功：0  失败：-1
 */
int sqlTbDel(SQL_HANDLE handle, char *delSql);

/**
 * @brief 事务 批量删除
 * @param  handle           传入当前数据库的句柄
 * @param  delSql           删除的sql语句
 * @param  delCnt           删除语句的条数
 * @param  isSync           是否同步  1：是  0：否
 * @param  step             一次执行的条数
 * @return int              成功：0   失败：-1
 */
int sqlTbBatchDel(SQL_HANDLE handle, char **delSql, int delCnt, int isSync, int step);


/* 数据库 备份 还原 删除 */
/* TODO: 不能使用 */
int sqlDbBackUp(SQL_HANDLE handle, char *dbName); 

/* TODO: 不能使用 */
int sqlDbRestore(SQL_HANDLE handle, char *dbName);

/* TODO: 不能使用 */
int sqlDbCreate(SQL_HANDLE handle, char *dbName);

/* TODO: 不能使用 */
int sqlDbDrop(SQL_HANDLE handle,  char *dbName); 


/* Test */
int sqlTbBatchModify_test(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step);

#ifdef __cplusplus
}
#endif

#endif


