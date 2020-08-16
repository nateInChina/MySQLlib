#include "MysqlDB.h"
#include <iostream>
#include <mysql.h>

#define DEBUG
using namespace std;
namespace {
    void DEBUG_ERR(MYSQL *mysql, const char *err, const char* file, unsigned int line)
    {
        if (err == nullptr || mysql == nullptr)
            return;

        cout << "[ERR]<" << file << ":" << line
            << ">\n======>" << err << mysql_error(mysql) << endl;;
    }

    void DEBUG_STMT_ERR(MYSQL_STMT *stmt, const char *err, const char* file, unsigned int line)
    {
        if (err == nullptr)
            return;

        cout << "[ERR]<" << file << ":" << line
            << ">\n======>" << err << mysql_stmt_error(stmt) << endl;;
    }

    void DEBUG_INFO(const char *info)
    {
        if (info == nullptr)
            return;

        cout << "[INFO]: ======> " << info << endl;;
    }

    void DEBUG_ERRINFO(const char *info, const char* file, unsigned int line)
    {
        if (info == nullptr)
            return;

        cout << "[ERR]<" << file << ":" << line
            << ">\n======>" << info << endl;;
    }
}

namespace MYSQLCPP {

    bool MySQLDB::Init()
    {
        //若句柄不为空，则不能再init
        if (mysql != nullptr)
        {
            return false;
        }

        if ((mysql = mysql_init(mysql)) == NULL)
        {
#ifdef DEBUG
            DEBUG_ERR(mysql, "MySQLDB::Init():", __FILE__, __LINE__);
#endif// DEBUG
            return false;
        }

        return true;
    }

    //清理占用的所有资源(不考虑线程安全)
    bool MySQLDB::Close()
    {
        //释放结果集
        FreeResult();

        if (mysql)
        {
            //关闭mysql句柄
            mysql_close(mysql);
            mysql = nullptr;
        }

        return true;
    }

    //释放结果集所占空间，在原生API中如果释放了的MYSQ_RES指针二次释放会dump
    bool  MySQLDB::FreeResult()
    {
        if (result != nullptr)
        {
            mysql_free_result(result);
            result = nullptr;
        }

        return true;
    }

    //数据连接(不考虑线程安全)
    bool MySQLDB::Connect(const char *host,
        const char *user,
        const char *passwd,
        const char *db,
        unsigned int port,
        unsigned long clientflag)
    {
        if (host == nullptr ||
            user == nullptr ||
            passwd == nullptr ||
            db == nullptr
            )
        {
#ifdef DEBUG
            DEBUG_ERRINFO(" MySQLDB::Connect paraments error", __FILE__, __LINE__);
#endif // DEBUG
            return false;
        }

        if (mysql == nullptr)
        {
            if (mysql_init(mysql) == NULL)
            {
#ifdef DEBUG
                DEBUG_ERR(mysql, "MySQLDB::Init():", __FILE__, __LINE__);
#endif// DEBUG
                return false;
            }
        }

        if (!mysql_real_connect(mysql, host, user, passwd, db, port, 0, clientflag))
        {
#ifdef DEBUG
            DEBUG_ERR(mysql, "MySQLDB::Connect error", __FILE__, __LINE__);
#endif // DEBUG
            return false;
        }

        return true;
    }

    //MySQL参数设const char *unix_socket,定，在Connect之前调用
    bool MySQLDB::Options(enum DBMYSQL_OPTION option, const void *arg)
    {
        if (mysql_options(mysql, (mysql_option)option, arg))
        {
#ifdef DEBUG
            DEBUG_ERR(mysql, "MySQLDB::Options() error:", __FILE__, __LINE__);
#endif // DEBUG
            return false;
        }

        return true;
    }

    //设置超时时间
    bool MySQLDB::setTimeOut(unsigned int nTimeOut)
    {
        Options(DBMYSQL_OPT_CONNECT_TIMEOUT, &nTimeOut);

        return true;
    }

    //设置断线重连
    bool MySQLDB::setReconnect(bool isre)
    {
        Options(DBMYSQL_OPT_RECONNECT, &isre);
        return true;
    }

    //执行SQL语句接口
    bool MySQLDB::Query(const char* strSQL)
    {
        if (strSQL == nullptr)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("Query() input parament error:", __FILE__, __LINE__);
#endif // DEBUG
            return false;
        }

        if (mysql == nullptr)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("Query() mysql == NULL", __FILE__, __LINE__);
#endif // DEBUG
            return false;
        }

        //这里可能有坑，要求传进的都是要以\0结尾的字符串SQL语句,不然strlen会出错
        if (mysql_real_query(mysql, strSQL, (unsigned long)strlen(strSQL)))
        {
#ifdef DEBUG
            DEBUG_ERR(mysql, "Query() error:", __FILE__, __LINE__);
#endif // DEBUG

            return false;
        }

        return true;
    }

    //取结果集，使用UseResult()时，最好在使用完后FreeResult
    //不保证多线程安全，所以在多线程程序中，要保证不会干扰到上一次的结果集
    //返回0，取回结果集成功
    //返回-1，失败
    //返回1，代表非select语句
    int MySQLDB::UseResult()
    {
        //检查参数mysql和result是否为空
        if (mysql == nullptr)
        {
#ifdef _DEBUG
            DEBUG_ERRINFO("mysql == nullptr", __FILE__, __LINE__);
#endif // _DEBUG 
            return -1;
        }

        if (result != nullptr)
        {
            FreeResult();
        }

        result = mysql_store_result(mysql);

        if (!result)
        {
            //非SELECT语句
            if (0 == mysql_field_count(mysql))
            {
                return 1;
            }
            //SELECT语句
            else
            {
#ifdef _DEBUG
                DEBUG_ERR(mysql, "mysql_use_result error", __FILE__, __LINE__);
#endif // _DEBUG    
                return -1;
            }
        }

        return 0;
    }

    //取结果集
    //不保证多线程安全，所以在多线程程序中，要保证不会干扰到上一次的结果集
    //返回0，取回结果集成功
    //返回-1，失败
    //返回1，代表非select语句
    int MySQLDB::StoreResult()
    {
        //检查参数mysql和result是否为空
        if (mysql == nullptr)
        {
#ifdef _DEBUG
            DEBUG_ERRINFO("mysql == nullptr", __FILE__, __LINE__);
#endif // _DEBUG 
            return -1;
        }

        if (result != nullptr)
        {
            FreeResult();
        }

        result = mysql_store_result(mysql);

        if (!result)
        {
            //非SELECT语句
            if (0 == mysql_field_count(mysql))
            {
                return 1;
            }
            //SELECT语句
            else
            {
#ifdef _DEBUG
                DEBUG_ERR(mysql, "mysql_store_result error", __FILE__, __LINE__);
#endif // _DEBUG    
                return -1;
            }
        }

        return 0;
    }

    //取一行结果
    // vRow最好是reserve一个足够大的空间再传入，可以避免频繁分配内存空间导致效率低下
    //返回：true 取到row；false 没有查询到数据 或者出错
    bool MySQLDB::FetchRow(Row &vRow)
    {
        //检查参数result是否为空
        if (result == nullptr)
        {
            return false;
        }

        MYSQL_ROW row = nullptr;
        row = mysql_fetch_row(result);

        if (row)
        {
            int nNum = mysql_num_fields(result);

            for (int i = 0; i < nNum; ++i)
            {
                DataDB ret;
                ret.data = row[i];
                
                vRow.push_back(ret);
            }

            return true;
        }
        else
        {
            return false;
        }
    }


    //极简接口，直接取回结果
    // 返回：
    //      若返回的vector大小为零，要么出错，要么是没有数据返回。
    Rows MySQLDB::FetchRows()
    {
        Rows rtn;
        //检查参数result是否为空
        if (result == nullptr)
        {
            return rtn;
        }

        MYSQL_ROW row;

        //预留足够的空间
        rtn.reserve(512);

        my_ulonglong nRowNum = mysql_num_rows(result);

        for (int i = 0; i < nRowNum; ++i)
        {
            row = mysql_fetch_row(result);

            //取回成功
            if (row)
            {
                int nNum = mysql_num_fields(result);
                vector<string> tmp;
                for (int j = 0; j < nNum; ++j)
                {  
                    tmp.push_back(string(row[j]));
                }

                rtn.push_back(tmp);
            }
            //取回失败，要么是出错，要么是没数据，就不作处理了
            /*else
            {
            */
        }
    
        return rtn;
    }

    string MySQLDB::GetInsertSql(DataKeyVal val, string TableName)
    {
        string sql;

        //INSERT INTO ... (`XXX`, `YYY`) VALUES (xxx, yyy);
        sql = "INSERT INTO `" + TableName + "`";
        string key = "";
        string value = "";
        for (auto ptr = val.begin(); ptr != val.end(); ++ptr)
        {
            key += "`";
            key += ptr->first;
            key += "`,";

            value += "'";
            value += ptr->second.data;
            value += "',";
        }

        //去掉最后一个逗号
        key[key.size() - 1] = ' ';
        value[value.size() - 1] = ' ';

        sql += " (" + key + ") VALUES (" + value + ");";
        
        return sql;
    }
    
    bool MySQLDB::Insert(DataKeyVal val, string TableName)
    {
        if (nullptr == mysql || 0 == TableName.size() || 0 == val.size())
            return false;

        //生成insert sql语句
        string sql = GetInsertSql(val, TableName);
       

        return Query(sql.c_str()) ? (mysql_affected_rows(mysql)>0 ? true : false): false;
    }

    bool MySQLDB::InsertBin(DataKeyVal val, std::string TableName)
    {
        if (nullptr == mysql || 0 == TableName.size() || 0 == val.size())
            return false;
        
        string sql;

        //拼接sql语句
        //INSERT INTO `t_data` (`XXX`, `YYY`) VALUES (?, ?, ?)
        sql = "INSERT INTO `" + TableName + "` ";
        string key;
        string value;

        int i = 0;

        //一定要初始化
        //这里可能有坑，只支持100个字段，如果大于100个字段可能就不够用了。
        //但大于100个字段也少见吧！
        MYSQL_BIND bind[100] = {0};

        for (auto ptr = val.begin(); ptr != val.end(); ++ptr)
        {
            //拼写列的名称
            key += "`";
            key += ptr->first;
            key += "`,";

            //拼写列的值，只不过在mysql中，二进制值的插入用?代替
            value += "?,";
            
            bind[i].buffer = const_cast<char *>(ptr->second.data);
            bind[i].buffer_type = (enum_field_types)ptr->second.FILE_TYPE;
            bind[i].buffer_length = ptr->second.size;
            i++;
        }

        //去掉尾部多余的逗号
        key[key.size() - 1] = ' ';
        value[value.size() - 1] = ' ';

        sql += "(" + key + ") VALUES (" + value + ");";

        MYSQL_STMT *stmt = mysql_stmt_init(mysql);
        if (!stmt)
        {
            #ifdef DEBUG
            DEBUG_STMT_ERR(stmt, "mysql_stmt_init eror", __FILE__, __LINE__);
            #endif // DEBUG
            return false;
        }

        if (mysql_stmt_prepare(stmt, sql.c_str(), sql.size()) != 0)
        {
            #ifdef DEBUG
            DEBUG_STMT_ERR(stmt, "mysql_stmt_prepare error", __FILE__, __LINE__);
            #endif // DEBUG
            return false;
        }

        if (mysql_stmt_bind_param(stmt, bind) != 0)
        {
            #ifdef DEBUG
            DEBUG_STMT_ERR(stmt, "mysql_stmt_bind_param error", __FILE__, __LINE__);
            #endif // DEBUG

            return false;
        }

        if (mysql_stmt_execute(stmt) != 0)
        {
            #ifdef DEBUG
            DEBUG_STMT_ERR(stmt, "mysql_stmt_execute error", __FILE__, __LINE__);
            #endif // DEBUG

            return false;
        }

        mysql_stmt_close(stmt);
        return true;
    }
}