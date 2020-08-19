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
    前提：首次要保证MYSQL服务器打开！！（相关的库、用户名按你自己的安装的数据库来修改下面的代码）
    下面的例程按顺序标号，称为第1个例程、第2个例程，依此类推
    
    第1个例程：
        这个例程简单，只要运行完后 **显示全部成功** 后，在数据库中查找是否有添加了相关的记录。
    第2个例程：
        这个例程是把一个叫mysql.jpg的图片读取入库，所以你要在项目根目录下拷贝进一张图片，
        可以改名为mysql.jpg，也可以直接修改代码成你图片的文件名。
        运行 **显示全部成功** 后，在数据库中查找是否有添加了相关的记录。
    第3个例程：
        从数据库中读出mysql.jpg的数据，并保存工程根目录下，命名为mysql_read.jpg;
        运行 **显示全部成功** 后，在工程根目录下打开mysql_read.jpg，看图片是否显示正常。
*/

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试插入非二进制数据", "[Insert_not_binary]")
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

    //sql = "INSERT INTO t_data (`name`, `size`) VALUES ('insert_test', 26);";

    //map<string, string> istData;
    //istData["name"] = "insert_test";
    //istData.insert(make_pair(string("size"), string("26")));

    DataKeyVal istData;
    
    istData["name"] = DataDB("insert_test");
    istData["size"] = DataDB("26");

    CHECK(true == Insert(istData, "t_data"));

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试插入二进制数据", "[Insert_binary]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));

    DataKeyVal val;
    DataDB file;
    file.LoadFile("mysql.jpg");

    val["name"] = "binary_test";
    val["size"] = (int *)&file.size;
    val["data"] = file;

    CHECK(true == InsertBin(val, "t_data"));

    //一定要记得把LoadFile申请的空间释放掉
    file.Free();

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}

//读出插入的图片数据
TEST_CASE_METHOD(MYSQLCPP::MySQLDB, "测试读出二进制数据，看数据正确与否", "[Insert_binary_read_and_compare]")
{
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    REQUIRE(true == Connect("192.168.2.154", "root", "123456", "lipz31", 3306, 0));

    //sql读取数据
    string sql = "select `data`,`size` from `t_data` where `name`='binary_test';";

    Query(sql.c_str());
    
    Row BinData;

    StoreResult();

    CHECK(true == FetchRow(BinData));

    //读取完毕，进行比较
    CHECK(BinData.size() == 2);

    fstream fs("mysq_read.jpg", ios::out | ios::binary);

    REQUIRE(fs);

    int size = atoi(BinData[1].data);
    
    CHECK(16181 == size);
    //fs << BinData[0];
    fs.write(BinData[0].data, size);

    fs.close();

    cout << "数据已经写到mysql_read.jpg，文件中，在工程目录中打开看一下图片是否与原图片一样！" << endl;

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}
