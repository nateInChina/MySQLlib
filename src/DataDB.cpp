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

    DataDB::DataDB(int &&val)
    {

        data = (char*)&val;
        size = sizeof(int);
        FILE_TYPE = DBMYSQL_TYPE_LONG;
    }
    
    bool DataDB::LoadFile(const char *path)
    {
        if (nullptr == path)
            return false;

        fstream sf(path, iso::in);

        if (!sf)
        {
#ifdef DEBUG
            DEBUG_ERRINFO("fstream open fail error", __FILE__, __LINE__);
#endif //DEBUG
            return false;
        }
 
        sf.seekg(0, ios::end);
        size = sf.tellg();

        sf.seekg(0, ios::beg);

        //为文件内容的大小申请空间
        char *FileData = new[size];
        int read = 0;

        while (sf.read(FileData+read, size-read))
        {
            read += sf.gcount();
        }

        FILE_TYPE = DBMYSQL_TYPE_BLOB;
        return true;
    }
    
}