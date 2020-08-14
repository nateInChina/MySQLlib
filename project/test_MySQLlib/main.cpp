#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include "datadb.h"
//#include "mysqldb.h"
//#include <iostream>
//#include <thread>
//#include <chrono>
//
//using namespace mysqlcpp;
//using namespace std;
//
//int main()
//{
//    mysqldb my;
//
//    //1 mysql 初始化
//    cout << "my.init() = " << my.init() << endl;
//
//    //2 连接mysql 如果没有调用init 内部会自动调用
//    if (my.connect("localhost", "root", "123456", "lipz31"))
//    {
//        cout << "my.connect success！" << endl;
//    }
//
//    my.close();
//    return 0;
//}