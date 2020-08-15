#pragma once
#ifdef MYSQLLIB_EXPORTS
//��̬���ӿ���Ŀ����
#define MYSQLAPI  __declspec(dllexport)
#else
//ִ�г������
#define MYSQLAPI  __declspec(dllimport)
#endif

#include <map>
#include <string>

namespace MYSQLCPP
{
    enum DBMYSQL_OPTION {
        DBMYSQL_OPT_CONNECT_TIMEOUT,
        DBMYSQL_OPT_COMPRESS,
        DBMYSQL_OPT_NAMED_PIPE,
        DBMYSQL_INIT_COMMAND,
        DBMYSQL_READ_DEFAULT_FILE,
        DBMYSQL_READ_DEFAULT_GROUP,
        DBMYSQL_SET_CHARSET_DIR,
        DBMYSQL_SET_CHARSET_NAME,
        DBMYSQL_OPT_LOCAL_INFILE,
        DBMYSQL_OPT_PROTOCOL,
        DBMYSQL_SHARED_MEMORY_BASE_NAME,
        DBMYSQL_OPT_READ_TIMEOUT,
        DBMYSQL_OPT_WRITE_TIMEOUT,
        DBMYSQL_OPT_USE_RESULT,
        DBMYSQL_REPORT_DATA_TRUNCATION,
        DBMYSQL_OPT_RECONNECT,
        DBMYSQL_PLUGIN_DIR,
        DBMYSQL_DEFAULT_AUTH,
        DBMYSQL_OPT_BIND,
        DBMYSQL_OPT_SSL_KEY,
        DBMYSQL_OPT_SSL_CERT,
        DBMYSQL_OPT_SSL_CA,
        DBMYSQL_OPT_SSL_CAPATH,
        DBMYSQL_OPT_SSL_CIPHER,
        DBMYSQL_OPT_SSL_CRL,
        DBMYSQL_OPT_SSL_CRLPATH,
        DBMYSQL_OPT_CONNECT_ATTR_RESET,
        DBMYSQL_OPT_CONNECT_ATTR_ADD,
        DBMYSQL_OPT_CONNECT_ATTR_DELETE,
        DBMYSQL_SERVER_PUBLIC_KEY,
        DBMYSQL_ENABLE_CLEARTEXT_PLUGIN,
        DBMYSQL_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
        DBMYSQL_OPT_MAX_ALLOWED_PACKET,
        DBMYSQL_OPT_NET_BUFFER_LENGTH,
        DBMYSQL_OPT_TLS_VERSION,
        DBMYSQL_OPT_SSL_MODE,
        DBMYSQL_OPT_GET_SERVER_PUBLIC_KEY,
        DBMYSQL_OPT_RETRY_COUNT,
        DBMYSQL_OPT_OPTIONAL_RESULTSET_METADATA,
        DBMYSQL_OPT_SSL_FIPS_MODE
    };

    enum DBMYSQL_FIELD_TYPE {
        DBMYSQL_TYPE_DECIMAL,
        DBMYSQL_TYPE_TINY,
        DBMYSQL_TYPE_SHORT,
        DBMYSQL_TYPE_LONG,
        DBMYSQL_TYPE_FLOAT,
        DBMYSQL_TYPE_DOUBLE,
        DBMYSQL_TYPE_NULL,
        DBMYSQL_TYPE_TIMESTAMP,
        DBMYSQL_TYPE_LONGLONG,
        DBMYSQL_TYPE_INT24,
        DBMYSQL_TYPE_DATE,
        DBMYSQL_TYPE_TIME,
        DBMYSQL_TYPE_DATETIME,
        DBMYSQL_TYPE_YEAR,
        DBMYSQL_TYPE_NEWDATE, /**< Internal to MySQL. Not used in protocol */
        DBMYSQL_TYPE_VARCHAR,
        DBMYSQL_TYPE_BIT,
        DBMYSQL_TYPE_TIMESTAMP2,
        DBMYSQL_TYPE_DATETIME2, /**< Internal to MySQL. Not used in protocol */
        DBMYSQL_TYPE_TIME2,     /**< Internal to MySQL. Not used in protocol */
        DBMYSQL_TYPE_JSON = 245,
        DBMYSQL_TYPE_NEWDECIMAL = 246,
        DBMYSQL_TYPE_ENUM = 247,
        DBMYSQL_TYPE_SET = 248,
        DBMYSQL_TYPE_TINY_BLOB = 249,
        DBMYSQL_TYPE_MEDIUM_BLOB = 250,
        DBMYSQL_TYPE_LONG_BLOB = 251,
        DBMYSQL_TYPE_BLOB = 252,
        DBMYSQL_TYPE_VAR_STRING = 253,
        DBMYSQL_TYPE_STRING = 254,
        DBMYSQL_TYPE_GEOMETRY = 255
    };

    struct MYSQLAPI DataDB
    {
        DataDB(){}
        DataDB(const char *val); //��Ӧ������������Ϊ��DBMYSQL_TYPE_STRING
        DataDB(int &&val);         //��Ӧ�������������ࣺDBMYSQL_TYPE_LONG
        bool LoadFile(const char *path);

        DBMYSQL_FIELD_TYPE FILE_TYPE;
        const char *data = nullptr;
        unsigned long size;
    };

    typedef std::map<std::string, DataDB> DataKeyVal;
}

