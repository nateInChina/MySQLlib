#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>

using namespace std;
using namespace MYSQLCPP;

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试取回结果集接口StoreResult", "[StoreResult]") {   
    cout << "[INFO]:begin mysql!!" << endl;
    
    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";

    REQUIRE(true == Query(sql.c_str()));

    //
    SECTION("非SELECT语句")
    {
        //清表
        sql = "truncate t_data";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(1 == StoreResult()); 
    }
    
    //表里没数据时 SELECT
    SECTION("SELECT语句，无数据")
    {
        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == StoreResult());
    }


    //表里有数据时 SELECT
    SECTION("SELECT语句，有数据")
    {
        //插入10条数据
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2中插入数据出错！！[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == StoreResult());
    }

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl; 
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试取回结果集接口UseResult", "[UseResult]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";

    REQUIRE(true == Query(sql.c_str()));

    //
    SECTION("非SELECT语句")
    {
        //清表
        sql = "truncate t_data";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(1 == UseResult());
    }

    //表里没数据时 SELECT
    SECTION("SELECT语句，无数据")
    {
        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == UseResult());
    }


    //表里有数据时 SELECT
    SECTION("SELECT语句，有数据")
    {
        //插入10条数据
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2中插入数据出错！！[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT count(*) from t_data;";
        REQUIRE(true == Query(sql.c_str()));
        REQUIRE(0 == UseResult());
    }

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "取回一行数据", "[FetchRow]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";
    Query(sql.c_str());

    //清表
    sql = "truncate t_data";
    Query(sql.c_str());

    //有结果集，但没数据(没有符合条件的数据)
    SECTION("有结果集，但没数据")
    {
        //id name data(null) size
        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Row getData;
        CHECK(false == FetchRow(getData));
    }

    //有结果集，也有数据，并且取回一行数据
    SECTION("有结果集，也有数据，并取回一条")
    {
        //插入10条数据
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2中插入数据出错！！[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        //id name data(null) size
        Row getData;
        Row Cmp;
        CHECK(true == FetchRow(getData));

        DataDB tmp;

        tmp.data = "1";
        Cmp.push_back(tmp);
        tmp.data = "test0.jpg";
        Cmp.push_back(tmp);
        tmp.data = "1024";
        Cmp.push_back(tmp);

        //DataDB要重载运算符== 才能进行比较
        CHECK(Cmp == getData);

        //也可以使用下面逐条比较，就可以看到每个字段的内容了
        //for (int i = 0; i < Cmp.size(); ++i)
        //{
        //    CHECK(Cmp[i] == getData[i]);
        //    
        //    cout << "Cmp: " << Cmp[i].data << " == getData: " << getData[i].data << endl;
        //}
        
    }

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "取回所有数据", "[FetchRows]") {
    cout << "[INFO]:begin mysql!!\n" << endl;

    //初始化
    Init();
    
    //连接数据库
    REQUIRE(true == Connect("127.0.0.1", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";
    Query(sql.c_str());

    //清表
    sql = "truncate t_data";
    Query(sql.c_str());

    //有结果集，但没数据(没有符合条件的数据)
    SECTION("有结果集，但没数据")
    {
        //id name data(null) size
        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Rows getData;
        getData = FetchRows();

        CHECK(0 == getData.size());

    }

    //有结果集，但只有一条数据，并且取回一行数据
    SECTION("有结果集，但只有一条数据，读回")
    {
        //id name data(null) size
        //插入一条数据
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
        DataDB item;

        item.data = "1";
        tmp.push_back(item);
        item.data = "lipz31";
        tmp.push_back(item);
        item.data = "10086";
        tmp.push_back(item);
        
        Cmp.push_back(tmp);

        CHECK(Cmp == getData);
    }

    //有结果集，也有多条数据，全部读回
    SECTION("有结果集，也有数据，全部读回")
    {
        //清表
        sql = "truncate t_data";
        Query(sql.c_str());

        //id name data(null) size
        //插入10条数据
        for (int i = 0; i < 10; ++i)
        {
            sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
            sql += to_string(i);
            sql += ".jpg', '1024');";

            if (!Query(sql.c_str()))
            {
                cout << "[ERR]:Catch2中插入数据出错！！[" << __FILE__ << "," << __LINE__ << "]" << endl;
            }
        }

        sql = "SELECT id,name,size from t_data;";
        Query(sql.c_str());
        StoreResult();

        Rows getData;
        getData = FetchRows();

        CHECK(10 == getData.size());

        Rows Cmp;
        Row nRow;
        DataDB item;
        string str;
        string name;
        for (int i = 0; i < 10; ++i)
        {
            nRow.clear();

            str = to_string(i+1);
            item.data = str.c_str();
            nRow.push_back(item); //id

            name = "test" + to_string(i) + ".jpg";
            item.data = name.c_str();
            nRow.push_back(item); //name

            item.data = "1024";
            nRow.push_back(item);//size

            Cmp.push_back(nRow);
        }

        CHECK(Cmp == getData);

        for (int i = 0; i < getData.size(); ++i)
        {
            for (int j = 0; j < getData[i].size(); ++j)
            {
                CHECK(Cmp[i][j] == getData[i][j]);
                cout << "Cmp: " << Cmp[i][j].data << " == getData: " << getData[i][j].data << endl;
            }
        }
        

    }

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}