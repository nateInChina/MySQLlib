#pragma once
#include  <vector>
#include  "DataDB.h"

struct MYSQL;
struct MYSQL_RES;
namespace MYSQLCPP {
    //��������
    typedef std::vector<std::vector<DataDB>> Rows;
    
    //һ������
    typedef std::vector<DataDB> Row;

    class MYSQLAPI MySQLDB
    {
    public:
        //��ʼ��Mysql API(�������̰߳�ȫ)
        bool Init();

        //����ռ�õ�������Դ(�������̰߳�ȫ)
        bool Close();
        
        //��������(�������̰߳�ȫ)
        bool Connect(const char *host,
                     const char *user, 
                     const char *passwd,
                     const char *db, 
                     unsigned int port = 3306,
                     unsigned long clientflag = 0);

        //MySQL������const char *unix_socket,������Connect֮ǰ����
        bool Options(enum DBMYSQL_OPTION option, const void *arg);

        //���ó�ʱʱ��
        bool setTimeOut(unsigned int nTimeOut);

        //���ö�������
        bool setReconnect(bool isre);

        //ִ��SQL���ӿ�
        bool Query(const char* strSQL);

        //�ͷŽ������ռ�ռ�
        bool FreeResult();

        //ȡ�����
        int UseResult();

        //ȡ�����
        int StoreResult();

        //ȡһ�н��
        bool FetchRow(Row &vRow);

        //����ӿڣ�ֱ��ȡ�ؽ��
        Rows FetchRows();
        
        std::string GetInsertSql(DataKeyVal val, std::string TableName);

        bool Insert(DataKeyVal val, std::string TableName);

        bool InsertBin(DataKeyVal val, std::string TableName);

        bool Update(DataKeyVal &kv, std::string TableName, std::string where);
        
        bool UpdateBin(DataKeyVal &kv, std::string TableName, std::string where);

        bool StartTransaction();

        bool StopTransaction();

        bool Commit();

        bool Rollback();


    protected:
        
        MYSQL *mysql = nullptr; //C++11��֧��
        MYSQL_RES *result = nullptr; //C++11��֧��
    };

}