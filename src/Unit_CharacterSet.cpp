#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>

using namespace std;
using namespace MYSQLCPP;

/*
测试方法：
    

   
*/

TEST_CASE_METHOD(MySQLDB, "字符转码", "[character_set]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //初始化
    Init();

    //连接数据库
    CHECK(true == Connect("192.168.2.154", "root", "123456", "lipz31"));

    string sql = "CREATE TABLE IF NOT EXISTS `t_CharacterSetTest` (\
            `id` INT AUTO_INCREMENT, \
            `name` VARCHAR(1024) CHARACTER SET gbk COLLATE gbk_bin, \
            `address` VARCHAR(2048) CHARACTER SET utf8 COLLATE utf8_bin,\
            PRIMARY KEY(`id`));";

    CHECK(true == Query(sql.c_str()));

    sql = "TRUNCATE t_CharacterSetTest;";
    CHECK(true == Query(sql.c_str()));

#ifdef _WIN32
    //////////////////// windows下测试 ///////////////////////
    /* vs2015本身对中文字符是gbk编码，控制台的输出是GBK编码 */

    ///写入gbk中文数据
    CHECK(true == Query("set names gbk;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"你三哥最帅";
        string gbk = tmp.Utf8ToGbk(); 
        data["name"] = gbk.c_str();
        Insert(data, "t_CharacterSetTest");
    }

    //读出gbk中文数据
    //对比下面三种情况输出的区别
    //1、直接输出                      预测中的结果应该是：中文不乱码
    //2、调用GbkToUtf8()               预测中的结果应该是：中文乱码
    //3、在2的基础上再调用Uft8ToGbk()  预测中的结果应该是：中文不乱码
    
    sql = "SELECT name FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1、直接输出
        cout << read[0].data << endl;
        //2、调用GbkToUtf8()
        cout << read[0].GbkToUtf8().c_str() << endl;
        //3、在2的基础上再调用Uft8ToGbk()
        cout << DataDB(read[0].GbkToUtf8().c_str()).Utf8ToGbk().c_str() << endl;
    }
    

    ///清除数据，继续下一个测试
    sql = "TRUNCATE t_CharacterSetTest;";
    CHECK(true == Query(sql.c_str()));
    
    
    ///写入utf8中文数据
    CHECK(true == Query("set names utf8;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"你三哥吼帅的";
        data["address"] = tmp;
        Insert(data, "t_CharacterSetTest");
    }

    //读出utf8中文数据
    //对比下面两种情况输出的区别
    //1、调用Uft8ToGbk()    预测中的结果应该是：中文不乱码
    //2、不调用Uft8ToGbk()  预测中的结果应该是：中文乱码
    sql = "SELECT address FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1、调用Utf8ToGbk()
        cout << read[0].Utf8ToGbk().c_str() << endl;
        //2、不调用Uft8ToGbk()
        cout << read[0].data << endl;
    }
    
    //////////////////////////////////////////////////////////

#else
    //////////////////// linux下测试 /////////////////////////
    /* gcc对中文字符是utf8编码，一般都是用类似于xshell
     * 这种控制终端，输出的字符编码是可以设置的，作者
     * 是把终端设置成UTF8编码
     */

    ///写入gbk中文数据
    CHECK(true == Query("set names gbk;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"你三哥最帅";
        string gbk = tmp.Utf8ToGbk();
        data["name"] = gbk.c_str();
        Insert(data, "t_CharacterSetTest");
    }


    //读出gbk中文数据
    //对比下面两种情况输出的区别
    //1、调用GbkToUtf8()    预测中的结果应该是：中文不乱码
    //2、不调用GbkToUtf8()  预测中的结果应该是：中文乱码
    sql = "SELECT name FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1、调用GbkToUtf8()
        cout << read[0].GbkToUtf8().c_str() << endl;
        //2、不调用GbkToUtf8()
        cout << read[0].data << endl;
    }

    ///清除数据，继续下一个测试
    sql = "TRUNCATE t_CharacterSetTest;";
    CHECK(true == Query(sql.c_str()));

    ///写入utf8中文数据
    CHECK(true == Query("set names utf8;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"你三哥吼帅的";
        data["address"] = tmp;
        Insert(data, "t_CharacterSetTest");
    }

    //读出utf8中文数据
    //对比下面三种情况输出的区别
    //1、直接输出                      预测中的结果应该是：中文不乱码
    //2、调用Uft8ToGbk()               预测中的结果应该是：中文乱码
    //3、在2的基础上再调用GbkToUtf8()  预测中的结果应该是：中文不乱码
    sql = "SELECT address FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1、直接输出   
        cout << read[0].data << endl;
        //2、调用Uft8ToGbk()
        cout << read[0].Utf8ToGbk().c_str() << endl;
        //3、在2的基础上再调用GbkToUtf8()
        cout << DataDB(read[0].Utf8ToGbk().c_str()).GbkToUtf8().c_str() << endl;

        
    }


    //////////////////////////////////////////////////////////
#endif //_WIN32

    //关闭连接
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}