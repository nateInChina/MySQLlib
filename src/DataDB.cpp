#include "DataDB.h"
#include <fstream>
#include <iostream>
#include <string.h>
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <iconv.h>
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
    //linux下字符编码转换函数
    static bool Convert(const char *tocode, const char *fromcode, 
                        char *inbuf, size_t inbytesleft,
                        char *outbuf, size_t outbytesleft)
    {
        if (nullptr == tocode || nullptr == fromcode || 
            nullptr == tocode || nullptr == fromcode)
        {
            #ifdef _DEBUG   
            DEBUG_ERRINFO("Convert input param error", __FILE__, __LINE__);
            #endif // _DEBUG

            return false;
        }

        //iconv_t iconv_open(const char *tocode, const char *fromcode);
        iconv_t cv = iconv_open(tocode, fromcode);

        if ((iconv_t)-1 == cv)
        {
            #ifdef _DEBUG   
            DEBUG_ERRINFO("iconv_open error", __FILE__, __LINE__);
            #endif // _DEBUG

            return false;
        }

        memset(outbuf, 0, outbytesleft);

        size_t rtn = iconv(cv, &inbuf, &inbytesleft, &outbuf, &outbytesleft);

        if ((size_t)-1 == rtn)
        {
            #ifdef _DEBUG   
            DEBUG_ERRINFO("iconv error", __FILE__, __LINE__);
            #endif // _DEBUG

            return false;
        }

        iconv_close(cv);

        return true;
    }


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

    //成功，返回转码后的字符串
    //失败，返回的字符的size为0
    string DataDB::Utf8ToGbk()
    {
        string re;

#ifdef _WIN32

        //1、转成宽字符(UTF16 编码),win32 API是接受UNICODE类开的参数，转化它们的编码。
        //第一次调用MultiByteToWideChar算出需要的字符串长度
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
        //第二次调用MultiByteToWideChar才是真正的转化成宽字符串
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
#if 1
        re.resize(1024);
        if (!Convert("GBK", "UTF8", (char*)data, size, (char*)re.data(), re.size()))
        {
            return false;
        }
        //这里看可不可以优化
        string rtn(re.data());
#endif
#endif // _WIN32

        return re;
    }

    //成功，返回转码后的字符串
    //失败，返回的字符的size为0
    string DataDB::GbkToUtf8()
    {
        string re;

#ifdef _WIN32

        //1、转成宽字符(UTF16 编码),win32 API是接受UNICODE类开的参数，转化它们的编码。
        //第一次调用MultiByteToWideChar算出需要的字符串长度
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
        //第二次调用MultiByteToWideChar才是真正的转化成宽字符串
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
#if 1
        /*
        bool Convert(const char *tocode, const char *fromcode,
                        char *inbuf, size_t inbytesleft,
                        char *outbuf, size_t outbytesleft)
        */
        re.resize(1024);
        if (!Convert("UTF8", "GBK", (char*)data, size, (char*)re.data(), re.size()))
        {
            return false;
        }
        //这里看可不可以优化
        string rtn(re.data());
        re = rtn;
#endif
#endif // _WIN32
        return re;
    }
}