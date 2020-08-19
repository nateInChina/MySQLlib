#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace MYSQLCPP;

/*
    测试方法：
        前提：保证你的mysql服务要开启！！！（相关的库名称、用户名根据你自己数据库的设置来修改下面的代码）
        下面的例程按顺序标号，称为第1个例程、第2个例程，依此类推
    第1个例程：
        测试Update接口能否成功更新表里一行数据中指定的非二进制字段的值，显示passed后，进入mysql查看是否被正确更改了。
    第2个例程：
        测试UpdateBin接口能否成功更新表里一行数据中指定的二进制字段的值，本例程是以读取图片为例，把一张图片读取更新进
        数据库，然后再从数据库读取这个图片数据，重新生成一个文件(文件名为avatar.jpz)，所以显示passed后还要打开工程目录
        下生成的avatar_read.jpg，看是否和更新进的图片是一样的。
*/

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试更新非二进制数据", "[Update_not_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";
    Query(sql.c_str());

    // UPDATE `t_data` set `size`=10024 where `id`=1;
    DataKeyVal kv;
    kv["size"] = "1211";
    CHECK(true == Update(kv, "t_data", "where `id`=1;"));

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试更新二进制数据", "[Update_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));

    //建表
    string sql = "CREATE TABLE IF NOT EXISTS `t_data`( \
		`id` int AUTO_INCREMENT, \
		`name` varchar(1024), \
		`data` blob, \
		`size` int , \
		PRIMARY KEY(`id`) \
	)ENGINE=InnoDB;";
    Query(sql.c_str());

    // UPDATE `t_data` set `size`=10024 where `id`=1;
    DataKeyVal val;
    DataDB file;
    file.LoadFile("avatar.jpg");

    val["size"] = (int *)&file.size;
    val["data"] = file;
    
    CHECK(true == UpdateBin(val, "t_data", "where `id`=1;"));
    file.Free();

#if 1
    //sql读取数据
    sql = "select `data` from `t_data` where `id`=1;";

    Query(sql.c_str());

    Row BinData;

    StoreResult();

    CHECK(true == FetchRow(BinData));

    fstream fs("avatar_read.jpg", ios::out | ios::binary);

    REQUIRE(fs);

    int size = BinData[0].size;

    fs.write(BinData[0].data, size);

    fs.close();

    cout << "数据已经写到avatar_read.jpg，文件中，在工程目录中打开看一下图片是否与原图片一样！" << endl;
#endif //0

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;

}