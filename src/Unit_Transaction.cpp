#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;
using namespace MYSQLCPP;

#define TEST_NUM 10000

/*
    ���Է�����
        ǰ�᣺��֤���mysql����Ҫ��������������صĿ����ơ��û����������Լ����ݿ���������޸�����Ĵ��룩
        ��������̰�˳���ţ���Ϊ��1�����̡���2�����̣���������
    ��1�����̣�Ҳֻ��1������
        �����������벻����������£��������ݵ�ʱ��Աȣ����Դ�������ֱ�ۿ�����Ч�ʲ���20�౶�������
        �����ϲ�һ������������ȡ������Ե�Ӳ�����ã���Ӳ�̡�CPU�ȣ�
*/

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "�������������������ٶȶԱ�", "[transaction_insert]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    CHECK(true == Connect("127.0.0.1", "root", "123456", "lipz31"));

    //�����ھͽ���
    string sql = "CREATE TABLE IF NOT EXISTS `t_data` ( \
        `id` INT AUTO_INCREMENT, \
        `name` VARCHAR(1024), \
        `data` BLOB,\
        `size` INT,\
        PRIMARY KEY(`id`)\
    )ENGINE=InnoDB";
    CHECK(true == Query(sql.c_str()));
    
    sql = "truncate t_data;";
    CHECK(true == Query(sql.c_str()));

    ////////////////////// ������������� ///////////////////////////
    //��ʼ��ʱ
    auto start = system_clock::now();

    //����������ݶԱ�
    for (unsigned long i = 0; i < TEST_NUM; ++i)
    {
        sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
        sql += to_string(i);
        sql += ".jpg', '111');";
        Query(sql.c_str());
    }

    auto end = system_clock::now();
    //ת��Ϊ���� 1000
    auto dur = duration_cast<milliseconds> (end - start);
    cout << "�������" << TEST_NUM << "ǧ������" << dur.count() / 1000. << "��" << endl;

    //////////////////////////////////////////////////////////////////

    sql = "truncate t_data;";
    CHECK(true == Query(sql.c_str()));
    ////////////////////// ����������� //////////////////////////////

    //��ʼ��ʱ
    start = system_clock::now();
    StartTransaction();
    //����������ݶԱ�
    for (unsigned long i = 0; i < TEST_NUM; ++i)
    {
        sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
        sql += to_string(i);
        sql += ".jpg', '111');";
        Query(sql.c_str());
    }

    Commit();
    StopTransaction();
    
    end = system_clock::now();
    //ת��Ϊ���� 1000
    dur = duration_cast<milliseconds> (end - start);
    cout << "�������" << TEST_NUM << "ǧ������" << dur.count() / 1000. << "��" << endl;

    //////////////////////////////////////////////////////////////////

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}
