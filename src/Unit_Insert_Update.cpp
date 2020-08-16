#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>
#include <fstream>

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
    istData["size"] = DataDB("26");

    CHECK(true == Insert(istData, "t_data"));

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

    DataKeyVal val;
    DataDB file;
    file.LoadFile("mysql.jpg");

    val["name"] = "binary_test";
    val["size"] = (int *)&file.size;
    val["data"] = file;

    CHECK(true == InsertBin(val, "t_data"));

    //һ��Ҫ�ǵð�LoadFile����Ŀռ��ͷŵ�
    file.Free();

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;

}

//���������ͼƬ����
TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "���Զ������������ݣ���������ȷ���", "[Insert_binary_read_and_compare]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //sql��ȡ����
    string sql = "select `data`,`size` from `t_data` where `name`='binary_test';";

    Query(sql.c_str());
    
    Row BinData;

    StoreResult();

    CHECK(true == FetchRow(BinData));

    //��ȡ��ϣ����бȽ�
    CHECK(BinData.size() == 2);

    fstream fs("mysq_read.jpg", ios::out | ios::binary);

    REQUIRE(fs);

    int size = atoi(BinData[1].data);
    
    CHECK(16181 == size);
    //fs << BinData[0];
    fs.write(BinData[0].data, size);

    fs.close();

    cout << "�����Ѿ�д��mysql_read.jpg���ļ��У��ڹ���Ŀ¼�д򿪿�һ��ͼƬ�Ƿ���ԭͼƬһ����" << endl;

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
