#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

/*
    ���Է�����
        1���״�Ҫ��֤MYSQL�������򿪣�
        2�����б��β�������
        3���ڽ���ڶ���TEST_CASE_METHOD�󣬹رշ�����
        ��ʱTEST_CASE_METHOD����ֶ��failed,ÿ��failed������3��,��֤�����ó�ʱ
        ʱ������Ч��
        4��һ��Ҫ������ִ�е�3�������10�����ٴδ�mysql����
        ��ʱ�ڶ���TEST_CASE_METHOD���failedת��passed��ֱ����������֤�˶���������
        ��Ч��
*/
TEST_CASE_METHOD (MYSQLCPP::MySQLDB, "����MysqlDB init,connect��close��������"){
    
    //��ʼ��
    Init();

    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));
    
    //�ر�����
    Close();

}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "����MysqlDB init,connect��close��������") {

    //��ʼ��
    Init();
    setTimeOut(3);
    setReconnect(true);

    //�������ݿ⣬��������
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));
    
    chrono::seconds value(1);
    int i = 0;
    while (i<=15)
    {
        CHECK(true == Query("CREATE TABLE IF NOT EXISTS `t_image`( \
                              `id` int AUTO_INCREMENT, \
                              `name` varchar(1024), \
                              `path` varchar(2046), \
                              `size` int ,\
                              PRIMARY KEY(`id`) \
                              ) ENGINE=InnoDB;"));
        this_thread::sleep_for(value);
        i++;
    }

    //�ر�����
    Close();
}

