#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

/*
    测试方法：
        1、首次要保证MYSQL服务器打开；
        2、运行本次测试例程
        3、在进入第二个TEST_CASE_METHOD后，关闭服务器
        此时TEST_CASE_METHOD会出现多个failed,每个failed间隔差不多3秒,验证了设置超时
        时间是生效的
        4、一定要控制在执行第3个步骤后10秒内再次打开mysql服务
        此时第二个TEST_CASE_METHOD会从failed转成passed，直到结束，验证了断线重连是
        生效的
*/
TEST_CASE_METHOD (MYSQLCPP::MySQLDB, "测试MysqlDB init,connect和close，无重连", "[init]"){
    
    //初始化
    Init();

    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));
    
    //关闭连接
    Close();

}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试MysqlDB init,connect和close，有重连", "[init_and_reconnect]") {

    //初始化
    Init();
    setTimeOut(3);
    setReconnect(true);

    //连接数据库，设置重连
    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));
    
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

    //关闭连接
    Close();
}

