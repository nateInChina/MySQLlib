#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>
using namespace std;
using namespace MYSQLCPP;

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "���Բ���Ƕ���������", "[Insert_not_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //����
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";
    Query(sql.c_str());

    //sql = "INSERT INTO t_data (`name`, `size`) VALUES ('insert_test', 26);";

    //map<string, string> istData;
    //istData["name"] = "insert_test";
    //istData.insert(make_pair(string("size"), string("26")));

    DataKeyVal istData;
    
    istData["name"] = DataDB("insert_test");
    istData["size"] = DataDB(26);


    Insert(istData, "t_data");

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "���Բ������������", "[Insert_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));


    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;

}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "���Ը��·Ƕ���������", "[Update_not_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));


    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;

}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "���Ը��¶���������", "[Update_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //����
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";



    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;

}

