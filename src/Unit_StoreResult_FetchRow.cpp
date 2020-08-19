#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <string.h>
#include <iostream>

using namespace std;
using namespace MYSQLCPP;
/*
    测试方法：
        前提：首次要保证MYSQL服务器打开！！（相关的库、用户名按你自己的安装的数据库来修改下面的代码）
        下面的例程按顺序标号，称为第1个例程、第2个例程，依此类推
    
    第1个例程：
        测试了StoreResult接口，测试的情景有：
        非SELECT语句是否能调用成功
        SELECT语句但没符合的数据时能否调用成功；
        SELECT语句且有符合的数据时能否调用成功；
        看结果是否显示PASS即可。
    第2个例程：
        和第1个例程一样，只是UseResult取回结果集时，不会马上把数据拿回来，而是返回结果而已，
        之后需要一条一条地FetchRow/FetchRows回来；这跟StoreResult是不一样的，虽然StoreResult
        最后也需要一条一条Fetch回来，但是它返回的结果集本身就从服务端把所有数据取回来了，而
        使用UseResult后的每一条Fetch反而是要从服务器端取回来。
    第3个例程：
        测试FetchRow接口，分“有结果集，但没有数据”和“有结果集，且有数据，并取回数据”；
        在第二个情景里，模拟插入了10条数据，将取回的数据跟这10条的第一条做比作。
    第4个例程：
        测试FetchRows接口,FetchRows是可以一次性取回所有行的，所以分以下几种情景测试：
            1、有结果集，但没数据(没有符合条件的数据)，返回的数据是空的；
            2、有结果集，且只有一条数据，取回这条数据；
                模拟插入了一条数据，并将取回的数据跟这条数据作对比。
            3、有结果集，且有多条数据，取回全部数据；
                模拟插入了多条数据，并将取回的数据跟取回的数据作对比。
        
*/
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

        FreeResult();

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
        
        string str;
        string name;
        for (int i = 0; i < 10; ++i)
        {
            Row nRow;
            DataDB item;
            nRow.clear();

            str = to_string(i+1);
            char *id = new char[10]{0};
            memcpy(id, str.c_str(), str.size());
            item.data = id;
            nRow.push_back(item); //id

            name = "test" + to_string(i) + ".jpg";
            char *szName = new char[100]{0};
            memcpy(szName, name.c_str(), name.size());
            item.data = szName;
            nRow.push_back(item); //name

            char *szSize = new char[100]{ 0 };
            memcpy(szSize, "1024", sizeof("1024"));
            item.data = szSize;
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
                delete[] Cmp[i][j].data;
            }
        }
    }

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}