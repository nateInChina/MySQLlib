#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>
using namespace std;
using namespace MYSQLCPP;


TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "����ȡ�ؽ�����ӿ�StoreResult", "[StoreResult]") {   
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

    REQUIRE(true == Query(sql.c_str()));

    //
    SECTION("��SELECT���")
    {
        //���
        sql = "truncate t_data";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(1 == StoreResult()); 
    }
    
    //����û����ʱ SELECT
    SECTION("SELECT��䣬������")
    {
        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == StoreResult());
    }


    //����������ʱ SELECT
    SECTION("SELECT��䣬������")
    {
        //����10������
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2�в������ݳ�����[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == StoreResult());
    }

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl; 
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "����ȡ�ؽ�����ӿ�UseResult", "[UseResult]") {
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

    REQUIRE(true == Query(sql.c_str()));

    //
    SECTION("��SELECT���")
    {
        //���
        sql = "truncate t_data";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(1 == UseResult());
    }

    //����û����ʱ SELECT
    SECTION("SELECT��䣬������")
    {
        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == UseResult());
    }


    //����������ʱ SELECT
    SECTION("SELECT��䣬������")
    {
        //����10������
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2�в������ݳ�����[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == UseResult());
    }

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "ȡ��һ������", "[FetchRow]") {
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

    //���
    sql = "truncate t_data";
    Query(sql.c_str());

    //�н��������û����(û�з�������������)
    SECTION("�н��������û����")
    {
        //id name data(null) size
        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Row getData;
        CHECK(false == FetchRow(getData));
    }

    //�н������Ҳ�����ݣ�����ȡ��һ������
    SECTION("�н������Ҳ�����ݣ���ȡ��һ��")
    {
        //����10������
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2�в������ݳ�����[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        //id name data(null) size
        Row getData;
        Row Cmp;
        CHECK(true == FetchRow(getData));

        Cmp.push_back(string("1"));
        Cmp.push_back(string("test0.jpg"));
        Cmp.push_back(string("1024"));
        
        CHECK(Cmp == getData);

    }

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "ȡ����������", "[FetchRows]") {
    cout << "[INFO]:begin mysql!!\n" << endl;

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

    //���
    sql = "truncate t_data";
    Query(sql.c_str());

    //�н��������û����(û�з�������������)
    SECTION("�н��������û����")
    {
        //id name data(null) size
        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Rows getData;
        getData = FetchRows();

        CHECK(0 == getData.size());

    }

    //�н��������ֻ��һ�����ݣ�����ȡ��һ������
    SECTION("�н��������ֻ��һ�����ݣ�����")
    {
        //id name data(null) size
        //����һ������
        sql = "INSERT INTO `t_data` (`name`, `size`) VALUES ('lipz31', 10086); ";
        Query(sql.c_str());

        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Rows getData;
        getData = FetchRows();

        CHECK(1 == getData.size());

        Row tmp;
        Rows Cmp;
        tmp.push_back(string("1"));
        tmp.push_back(string("lipz31"));
        tmp.push_back(string("10086"));
        Cmp.push_back(tmp);

        CHECK(Cmp == getData);
    }

    //�н������Ҳ�ж������ݣ�ȫ������
    SECTION("�н������Ҳ�����ݣ�ȫ������")
    {
        //���
        sql = "truncate t_data";
        Query(sql.c_str());

        //id name data(null) size
        //����10������
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2�в������ݳ�����[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Rows getData;
        getData = FetchRows();

        CHECK(10 == getData.size());

        Rows Cmp;
        Row tmp;
        for (int i = 0; i < 10; ++i)
        {
            tmp.clear();

            tmp.push_back(to_string(i+1)); //id
            string name = "test" + to_string(i) + ".jpg";
            tmp.push_back(name); //name
            tmp.push_back(string("1024"));//size

            Cmp.push_back(tmp);
        }

        CHECK(Cmp == getData);

    }


    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}
