#include "DataDB.h"
#include <fstream>
#include <iostream>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


using namespace std;

#define DEBUG

namespace {
    void DEBUG_INFO(const char *info)
    {
        if (info == nullptr)
            return;

        cout << "[INFO]: ======> " << info << endl;;
    }

    void DEBUG_ERRINFO(const char *info, const char* file, unsigned int line)
    {
        if (info == nullptr)
            return;

        cout << "[ERR]<" << file << ":" << line
            << ">\n======>" << info << endl;;
    }
}

namespace MYSQLCPP
{
#ifndef _WIN32
    //linux���ַ�����ת������



#endif

    DataDB::DataDB(const char *val)
    {
        if (nullptr == val)
        {
            return;
        }

        data = val;
        size = (unsigned long)strlen(val);
        FILE_TYPE = DBMYSQL_TYPE_STRING;
    }

    DataDB::DataDB(int *val)
    {

        data = (const char*)val;
        size = sizeof(int);
        FILE_TYPE = DBMYSQL_TYPE_LONG;
    }
    
    //���ļ����ݼ��ص�data�У�ע�⣬���ӿ�ʹ���꣬Ҫ����Free������Ŀռ��ͷ�
    //TODO������RAII�ٽ���װ����ӿڣ��Ͳ���Ҫʹ��ʱС�������ˡ�
    bool DataDB::LoadFile(const char *path)
    {
        if (nullptr == path)
            return false;

        fstream sf(path, ios::in | ios::binary);

        if (!sf)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("fstream open fail error", __FILE__, __LINE__);
#endif //DEBUG
            return false;
        }
 
        sf.seekg(0, ios::end);
        size = (unsigned long)sf.tellg();

        sf.seekg(0, ios::beg);

        //Ϊ�ļ����ݵĴ�С����ռ�
        char *FileData = new char[size]; //һ��Ҫ�ǵ��ͷ�
        int read = 0;

        while (!sf.eof())
        {
            sf.read(FileData + read, size - read);
            if (sf.gcount() > 0)
                read += sf.gcount();
            else
                break;
        }

        data = FileData;
        FILE_TYPE = DBMYSQL_TYPE_BLOB;

        sf.close();   
        return true;
    }

    bool DataDB::Free()
    {
        delete[] data;
        data = nullptr;

        return true;
    }
    
    bool DataDB::operator == (const DataDB &another) const
    {
        if (strcmp(data, another.data) == 0)
            return true;

        return false;
    }

    //�ɹ�������ת�����ַ���
    //ʧ�ܣ����ص��ַ���sizeΪ0
    string DataDB::Utf8ToGbk()
    {
        string re;

#ifdef _WIN32

        //1��ת�ɿ��ַ�(UTF16 ����),win32 API�ǽ���UNICODE�࿪�Ĳ�����ת�����ǵı��롣
        //��һ�ε���MultiByteToWideChar�����Ҫ���ַ�������
        int nBufSize = MultiByteToWideChar(CP_UTF8, 0, data, -1, nullptr, 0);
        if (!nBufSize)
        {
            #ifdef DEBUG
            DEBUG_ERRINFO("MultiByteToWideChar first time error", __FILE__, __LINE__);
            #endif //DEBUG
            return re;
        }

        wstring buf;
        buf.resize(nBufSize);
        //�ڶ��ε���MultiByteToWideChar����������ת���ɿ��ַ���
        if (!MultiByteToWideChar(CP_UTF8, 0, data, -1, (LPWSTR)buf.c_str(), nBufSize))
        {
            #ifdef DEBUG
            DEBUG_ERRINFO("MultiByteToWideChar second time error", __FILE__, __LINE__);
            #endif //DEBUG
            return re;
        }

        int ReStrSize = WideCharToMultiByte(CP_ACP, 0, buf.c_str(), nBufSize, 0, 0, 0, 0);
        if (!ReStrSize)
        {
            #ifdef DEBUG
            DEBUG_ERRINFO("WideCharToMultiByte first time error", __FILE__, __LINE__);
            #endif //DEBUG
            return re;
        }

        re.resize(ReStrSize);

        if (!WideCharToMultiByte(CP_ACP, 0, buf.c_str(), nBufSize, (LPSTR)re.c_str(), ReStrSize, 0, 0))
        {
            #ifdef DEBUG
            DEBUG_ERRINFO("WideCharToMultiByte first time error", __FILE__, __LINE__);
            #endif //DEBUG
            re.shrink_to_fit();
            return re;
        }   
#else
        

#endif // _WIN32

        return re;
    }

    //�ɹ�������ת�����ַ���
    //ʧ�ܣ����ص��ַ���sizeΪ0
    string DataDB::GbkToUtf8()
    {
        string re;

#ifdef _WIN32

        //1��ת�ɿ��ַ�(UTF16 ����),win32 API�ǽ���UNICODE�࿪�Ĳ�����ת�����ǵı��롣
        //��һ�ε���MultiByteToWideChar�����Ҫ���ַ�������
        int nBufSize = MultiByteToWideChar(CP_ACP, 0, data, -1, nullptr, 0);
        if (!nBufSize)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("MultiByteToWideChar first time error", __FILE__, __LINE__);
#endif //DEBUG
            return re;
        }

        wstring buf;
        buf.resize(nBufSize);
        //�ڶ��ε���MultiByteToWideChar����������ת���ɿ��ַ���
        if (!MultiByteToWideChar(CP_ACP, 0, data, -1, (LPWSTR)buf.c_str(), nBufSize))
        {
#ifdef DEBUG
            DEBUG_ERRINFO("MultiByteToWideChar second time error", __FILE__, __LINE__);
#endif //DEBUG
            return re;
        }

        int ReStrSize = WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), nBufSize, 0, 0, 0, 0);
        if (!ReStrSize)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("WideCharToMultiByte first time error", __FILE__, __LINE__);
#endif //DEBUG
            return re;
        }

        re.resize(ReStrSize);

        if (!WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), nBufSize, (LPSTR)re.c_str(), ReStrSize, 0, 0))
        {
#ifdef DEBUG
            DEBUG_ERRINFO("WideCharToMultiByte first time error", __FILE__, __LINE__);
#endif //DEBUG
            re.shrink_to_fit();
            return re;
        }
#else



#endif // _WIN32
        return re;
    }
}