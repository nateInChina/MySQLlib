#include "catch.hpp"
#include "DataDB.h"
#include "MysqlDB.h"
#include <string>
#include <iostream>

using namespace std;
using namespace MYSQLCPP;

/*
���Է�����
    

   
*/

TEST_CASE_METHOD(MySQLDB, "�ַ�ת��", "[character_set]") {
    cout << "[INFO]:begin mysql!!" << endl;

    //��ʼ��
    Init();

    //�������ݿ�
    CHECK(true == Connect("127.0.0.1", "root", "123456", "lipz31"));

    string sql = "CREATE TABLE IF NOT EXISTS `t_CharacterSetTest` (\
            `id` INT AUTO_INCREMENT, \
            `name` VARCHAR(1024) CHARACTER SET gbk COLLATE gbk_bin, \
            `address` VARCHAR(2048) CHARACTER SET utf8 COLLATE utf8_bin,\
            PRIMARY KEY(`id`));";

    CHECK(true == Query(sql.c_str()));

    sql = "TRUNCATE t_CharacterSetTest;";
    CHECK(true == Query(sql.c_str()));

#ifdef _WIN32
    //////////////////// windows�²��� ///////////////////////
    /* vs2015����������ַ���gbk���룬����̨�������GBK���� */

    ///д��gbk��������
    CHECK(true == Query("set names gbk;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"��������˧";
        string gbk = tmp.Utf8ToGbk(); 
        data["name"] = gbk.c_str();
        Insert(data, "t_CharacterSetTest");
    }

    //����gbk��������
    //�Ա���������������������
    //1��ֱ�����                      Ԥ���еĽ��Ӧ���ǣ����Ĳ�����
    //2������GbkToUtf8()               Ԥ���еĽ��Ӧ���ǣ���������
    //3����2�Ļ������ٵ���Uft8ToGbk()  Ԥ���еĽ��Ӧ���ǣ����Ĳ�����
    
    sql = "SELECT name FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1��ֱ�����
        cout << read[0].data << endl;
        //2������GbkToUtf8()
        cout << read[0].GbkToUtf8().c_str() << endl;
        //3����2�Ļ������ٵ���Uft8ToGbk()
        cout << DataDB(read[0].GbkToUtf8().c_str()).Utf8ToGbk().c_str() << endl;
    }
    

    ///������ݣ�������һ������
    sql = "TRUNCATE t_CharacterSetTest;";
    CHECK(true == Query(sql.c_str()));
    
    
    ///д��utf8��������
    CHECK(true == Query("set names utf8;"));
    {
        DataKeyVal data;
        DataDB tmp = u8"�������˧��";
        data["address"] = tmp;
        Insert(data, "t_CharacterSetTest");
    }

    //����utf8��������
    //�Ա���������������������
    //1������Uft8ToGbk()    Ԥ���еĽ��Ӧ���ǣ����Ĳ�����
    //2��������Uft8ToGbk()  Ԥ���еĽ��Ӧ���ǣ���������
    sql = "SELECT address FROM t_CharacterSetTest where id = 1";
    Query(sql.c_str());
    StoreResult();
    {
        Row read;
        FetchRow(read);
        //1������Utf8ToGbk()
        cout << read[0].Utf8ToGbk().c_str() << endl;
        //2��������Uft8ToGbk()
        cout << read[0].data << endl;
    }
    
    //////////////////////////////////////////////////////////

#else
    //////////////////// linux�²��� /////////////////////////
    /* gcc�������ַ���utf8���룬һ�㶼����������xshell
     * ���ֿ����նˣ�������ַ������ǿ������õģ�����
     * �ǰ��ն����ó�UTF8����
     */

    ///д��gbk��������



    //����gbk��������
    //�Ա���������������������
    //1������GbkToUtf8()    Ԥ���еĽ��Ӧ���ǣ����Ĳ�����
    //2��������GbkToUtf8()  Ԥ���еĽ��Ӧ���ǣ���������



    ///д��utf8��������

    //����utf8��������
    //�Ա���������������������
    //1��ֱ�����                      Ԥ���еĽ��Ӧ���ǣ����Ĳ�����
    //2������Uft8ToGbk()               Ԥ���еĽ��Ӧ���ǣ���������
    //3����2�Ļ������ٵ���GbkToUtf8()  Ԥ���еĽ��Ӧ���ǣ����Ĳ�����



    //////////////////////////////////////////////////////////
#endif //_WIN32

    //�ر�����
    Close();

    cout << "[INFO]:close mysql!!\n" << endl;
}