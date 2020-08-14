#pragma once
#include  <vector>
#include  "DataDB.h"

struct MYSQL;
struct MYSQL_RES;
namespace MYSQLCPP {
    class MYSQLAPI MySQLDB
    {
    public:
        //初始化Mysql API(不考虑线程安全)
        bool Init();

        //清理占用的所有资源(不考虑线程安全)
        bool Close();
        
        //数据连接(不考虑线程安全)
        bool Connect(const char *host,
                     const char *user, 
                     const char *passwd,
                     const char *db, 
                     unsigned int port = 3306,
                     unsigned long clientflag = 0);

        //MySQL参数设const char *unix_socket,定，在Connect之前调用
        bool Options(enum DBMYSQL_OPTION option, const void *arg);

        //设置超时时间
        bool setTimeOut(unsigned int nTimeOut);

        //设置断线重连
        bool setReconnect(bool isre);

        //执行SQL语句接口
        bool Query(const char* strSQL);

        //释放结果集所占空间
        bool FreeResult();

        //取结果集
        //std::vector<std::vector<std::string>> vi;
    protected:
        MYSQL *mysql = nullptr; //C++11后支持
        MYSQL_RES *result = nullptr; //C++11后支持
    };

}