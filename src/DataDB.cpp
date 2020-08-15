#include "DataDB.h"
#include <fstream>
#include <iostream>
using namespace std;

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
           
    }

    DataDB::DataDB(int &&val)
    {
        data = (char*)&val;
        size = sizeof(int);
    }
    
    bool DataDB::LoadFile(const char *path)
    {



        return true;
    }
    
}