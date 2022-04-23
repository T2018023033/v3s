#ifndef __SQLITEDBMS_CLIENT_H
#define __SQLITEDBMS_CLIENT_H

#include "sqlited.h"

#ifdef __cplusplus
extern "C"
{
#endif

//typedef void *SQL_HANDLE;
typedef sqlited *SQL_HANDLE;
 

typedef struct
{
    char ***table;
    sqlited_int64 nrows;
    unsigned int nfields;  
} SQL_TABLE_S;

typedef struct
{
    char **fieldType;
    char **fieldName;
    int nfields;
} SQL_TABLE_FIELD_S;

/**
 * @brief 数据库连接
 * @param  remoteIp         数据库远程 IP, 字符串输入, 如 "10.5.0.111"
 * @param  dbName           数据库名, 如 "zmd.db", 可为空, 自动连接":memory:" 数据库
 * @return SQL_HANDLE       返回NULL, 表示失败; 成功返回连接句柄
 * @note   可对多个数据库进行连接
 */
SQL_HANDLE sqlConn(char *remoteIp, char *dbName);

/**
 * @brief 数据库关闭
 * @param  handle           待断开连接的句柄
 */
void sqlClose(SQL_HANDLE handle);

/**
 * @brief 执行sql语句
 * 
 * @param handle        传入的句柄
 * @param query         执行的sql语句
 * @param msg_result    返回的信息
 * @return int          0:succeed -1:failed
 */
int sqlQuery_run(SQL_HANDLE handle, char *query, char *msg_result); 

/**
 * @brief 当前连接数据库备份
 * @param  handle           当前连接数据库句柄
 * @param  dbName           要备份的数据库名字
 * @return int              0: 成功 非0: 失败
 */
int sqlDbBackUp(SQL_HANDLE handle, char *dbName); 

/**
 * @brief 当前连续数据库恢复为备份数据库
 * @param  handle           当前连接数据库句柄
 * @param  dbName           数据库名
 * @return int              0: 成功 非0: 失败
 */
int sqlDbRestore(SQL_HANDLE handle, char *dbName);

/**
 * @brief 创建一个数据库 
 * 
 * @param handle 当前数据库句柄
 * @param dbName 要创建的数据库名字
 * @return int   0:成功  -1：失败
 */
int sqlDbCreate(SQL_HANDLE handle, char *dbName);

/**
 * @brief 当前连接数据库删除, 删除后 handle 默认连接到 "::memory::" 数据库
 * @param  handle           当前连接数据库句柄
 * @param  dbName           数据库名
 * @return int              0: 成功 非0: 失败
 */
int sqlDbDrop(SQL_HANDLE handle,  char *dbName); 

#if 0
/**
 * @brief 获取远程sqlite版本
 * @param  handle           当前连接数据库句柄
 * @return char*            NULL: 获取失败 其他: 版本字符串    
 */
char *sqlVerGetm(SQL_HANDLE handle);
#endif
/**
 * @brief 获取数据库版本信息
 * 
 * @param handle 当前数据库句柄
 * @param verMsg 数据库版本信息
 * @return int   0：成功  -1：失败
 */
int sqlVerGet(SQL_HANDLE handle, char *verMsg); 

/**
 * @brief 创建数据表
 * @param  handle           当前连接数据库句柄
 * @param  tbName           待创建数据库表名
 * @param  fields           待创建数据库字段信息
 * @return int              0: 成功 非0: 失败
 */
int sqlTbCreate(SQL_HANDLE handle, char *tbName, SQL_TABLE_FIELD_S *fields);

/**
 * @brief 删除数据表
 * @param  handle           当前连接数据库句柄
 * @param  tbName           待删除数据库表名
 * @return int              0: 成功 非0: 失败
 */
int sqlTbDrop(SQL_HANDLE handle, char *tbName); 

/**
 * @brief 数据表内容普通插入
 * @param  handle           当前连接数据库句柄
 * @param  tbName           数据表名
 * @param  tbInfo           插入数据信息, 包括行和列
 * @return int              0: 成功 非0: 失败
 */
int sqlTbInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo);

/**
 * @brief 数据表内容批量插入(事务型插入)
 * @param  handle           当前连接数据库句柄
 * @param  tbName           数据表名
 * @param  tbInfo           插入数据信息, 包括行和列
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchInsert(SQL_HANDLE handle, char *tbName, SQL_TABLE_S *tbInfo, int isSync, int step);

/**
 * @brief 数据表内容普通修改
 * @param  handle           当前连接数据库句柄
 * @param  modifySql        普通修改 sql 语句
 * @return int              0: 成功 非0: 失败
 */
int sqlTbMonify(SQL_HANDLE handle, char *modifySql);

/**
 * @brief 数据表内容批量修改(事务型修改)
 * @param  handle           当前连接数据库句柄
 * @param  modifySql        修改 sql 语句
 * @param  modifyCnt        修改 sql 语句条数
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchMonify(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step); 

/**
 * @brief 数据表内容普通删除
 * @param  handle           当前连接数据库句柄
 * @param  delSql           删除 sql 语句
 * @return int              0: 成功 非0: 失败
 */
int sqlTbDel(SQL_HANDLE handle, char *delSql);

/**
 * @brief 数据表内容批量删除(事务型删除)
 * @param  handle           当前连接数据库句柄
 * @param  delSql           删除 sql 语句
 * @param  delCnt           删除 sql 语句条数
 * @param  isSync           是否同步, 0 不同步, 1 同步
 * @param  step             一条事务执行条数
 * @return int              0: 成功 非0: 失败
 */
int sqlTbBatchDel(SQL_HANDLE handle, char **delSql, int delCnt, int isSync, int step);

/**
 * @brief 数据表内容查询
 * @param  handle           当前连接数据库句柄
 * @param  querySql         查询 sql 语句
 * @param  tbInfo           返回查询到的数据内容
 * @return int              0: 成功 非0: 失败
 * @note   查询后, 必须执行查询 sqlTbQueryFree() 销毁malloc的内容
 */
int sqlTbQuery(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo);

/**
 * @brief 数据库查询  (推荐使用)   注:查询完成要释放内存
 * @param handle    数据库句柄
 * @param querySql  查询语句
 * @param tbInfo    传出的数据
 * @return int      0 ：成功   -1：失败
 */
int sqlQueryData(SQL_HANDLE handle, char *querySql, SQL_TABLE_S *tbInfo);

/**
 * @brief 数据表查询后需要执行的一次销毁命令
 * @param  handle           当前连接数据库句柄
 * @param  tbInfo           释放内存
 * @return int              0: 成功 非0: 失败
 */
int sqlTbQueryFree(SQL_HANDLE handle, SQL_TABLE_S tbInfo);



//TODO:  test
/*****************************************************************/
/*测试*/
/*****************************************************************/
int sqlTbBatchModify_test(SQL_HANDLE handle, char **modifySql, int modifyCnt, int isSync, int step);

#ifdef __cplusplus
} /* End of the 'extern "C"' block */
#endif

#endif /* ZMD_SQLITED_CLIENT_H */
