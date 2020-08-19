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
    测试方法：
        前提：保证你的mysql服务要开启！！！（相关的库名称、用户名根据你自己数据库的设置来修改下面的代码）
        下面的例程按顺序标号，称为第1个例程、第2个例程，依此类推
    第1个例程，也只有1个啦：
        测试事务开启与不开启的情况下，插入数据的时间对比，可以从输出结果直观看出，效率差了20多倍（在你的
        电脑上不一定是这个结果，取决你电脑的硬件配置，如硬盘、CPU等）
*/

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "事务插入与非事务插入的速度对比", "[transaction_insert]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    CHECK(true == Connect("192.168.2.154", "root", "123456", "lipz31"));

    //表不存在就建表
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

    ////////////////////// 非事务插入数据 ///////////////////////////
    //开始计时
    auto start = system_clock::now();

    //插入多条数据对比
    for (unsigned long i = 0; i < TEST_NUM; ++i)
    {
        sql = "INSERT INTO t_data (`name`, `size`) VALUES ('test";
        sql += to_string(i);
        sql += ".jpg', '111');";
        Query(sql.c_str());
    }

    auto end = system_clock::now();
    //转换为毫秒 1000
    auto dur = duration_cast<milliseconds> (end - start);
    cout << "事务插入" << TEST_NUM << "千条数据" << dur.count() / 1000. << "秒" << endl;

    //////////////////////////////////////////////////////////////////

    sql = "truncate t_data;";
    CHECK(true == Query(sql.c_str()));
    ////////////////////// 事务插入数据 //////////////////////////////

    //开始计时
    start = system_clock::now();
    StartTransaction();
    //插入多条数据对比
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
    //转换为毫秒 1000
    dur = duration_cast<milliseconds> (end - start);
    cout << "事务插入" << TEST_NUM << "千条数据" << dur.count() / 1000. << "秒" << endl;

    //////////////////////////////////////////////////////////////////

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}
