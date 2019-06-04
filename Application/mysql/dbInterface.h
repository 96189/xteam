
//

#ifndef _DB_INTERFACE_HPP_
#define _DB_INTERFACE_HPP_

#include <string>

class DbParam 
{
public:
    std::string host_;
    std::string user_;
    std::string passwd_;
    std::string dbName_;
    unsigned int port_;
    std::string unixSocket_;
    unsigned int clientFlag_;

    DbParam(std::string host="",std::string user="",std::string passwd="",std::string dbName="",unsigned int port=0,
            std::string unixSocket="",unsigned int clientFlag=0)
        : host_(host), user_(user), passwd_(passwd), dbName_(dbName), port_(port), 
          unixSocket_(unixSocket), clientFlag_(clientFlag)
    {

    }
    ~DbParam()
    {

    }
};

class CConnect;
class CDataStore;
class DbInterface 
{
private:
    CConnect *connPtr_;     // 接口类型
    CDataStore *storePtr_;  // 接口类型

public:
    // 构造 析构
    DbInterface();
    virtual ~DbInterface();

    // CConnect操作相关
    bool ConnectDb(DbParam& dbParam);
    bool DisConnect();
    void* GetConnect();
    std::string ConnErrorMsg();
    bool CallProcedure(const std::string& sql);
    bool UpdateSql(const std::string& sql);

    // CDataStore操作相关
    std::string StoreErrorMsg();
    bool Query(std::string& dml);
    unsigned long RowCount();
    std::string GetItemString(unsigned long row, unsigned int index);
    std::string GetItemString(unsigned long row, std::string& fieldName);
    float GetItemFloat(unsigned long row, unsigned int index);
    float GetItemFloat(unsigned long row, std::string& fieldName);
    long GetItemLong(unsigned long row, unsigned int index);
    long GetItemLong(unsigned long row, std::string& fieldName);
    // long long GetItemLongLong(unsigned long row, unsigned int index);
    // long long GetItemLongLong(unsigned long row, std::string& fieldName);
};

#endif