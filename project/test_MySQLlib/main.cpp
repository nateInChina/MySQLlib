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
//	//1 mysql ��ʼ��
//	cout << "my.Init() = " << my.Init() << endl;
//
//	//2 ����mysql ���û�е���Init �ڲ����Զ�����
//	if (my.Connect("localhost", "root", "123456", "lipz31"))
//	{
//		cout << "my.Connect success��" << endl;
//	}
//
//	my.Close();
//	return 0;
//}