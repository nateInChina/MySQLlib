#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include "DataDB.h"
//#include "MysqlDB.h"
//#include <iostream>
//#include <thread>
//#include <chrono>
//
//using namespace MYSQLCPP;
//using namespace std;
//
//int main()
//{
//	MySQLDB my;
//
//	//1 mysql 初始化
//	cout << "my.Init() = " << my.Init() << endl;
//
//	//2 连接mysql 如果没有调用Init 内部会自动调用
//	if (my.Connect("localhost", "root", "123456", "lipz31"))
//	{
//		cout << "my.Connect success！" << endl;
//	}
//
//	my.Close();
//	return 0;
//}