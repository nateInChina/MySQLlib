#include "DataDB.h"
#include <fstream>
#include <iostream>

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
    
    //将文件内容加载到data中，注意，本接口使用完，要调用Free把申请的空间释放
    //TODO：利用RAII再将封装这个接口，就不需要使用时小心翼翼了。
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

        //为文件内容的大小申请空间
        char *FileData = new char[size]; //一定要记得释放
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
}