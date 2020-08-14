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

		//这里可能有坑，要求传进的都是要以\0结尾的字符串SQL语句
		if (mysql_real_query(mysql, strSQL, strlen(strSQL)))
		{
			#ifdef DEBUG
			DEBUG_ERR(mysql, "Query() error:", __FILE__, __LINE__);
			#endif // DEBUG

			return false;
		}

		return true;
	}
}