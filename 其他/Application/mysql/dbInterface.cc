
//

#include "dbInterface.h"
#include "connectpool.h"
#include <assert.h>
#include <iostream>

DbInterface::DbInterface()
    : connPtr_(NULL), storePtr_(NULL)
{

}
DbInterface::~DbInterface()
{
    DisConnect();
}

//
bool DbInterface::ConnectDb(DbParam &dbParam)
{
    try
    {
        connPtr_ = new CMysqlConnect();
        if (!connPtr_->Connect(dbParam.host_, dbParam.user_, dbParam.passwd_, 
             dbParam.dbName_, dbParam.port_, dbParam.unixSocket_, "utf8"))
        {
            // log
            std::cout << "ConnectDB! ERROR : " << ConnErrorMsg() << std::endl;
            return false;            
        }
        storePtr_ = new CMysqlStore();
        storePtr_->SetTransAction(connPtr_);
        return true;
    }
    catch (...)
    {
        // log
        std::cout << "ConnectDB exception : " << ConnErrorMsg() << std::endl;
        return false;
    }
    return false;
}
bool DbInterface::DisConnect()
{
    if (connPtr_)
    {
        delete connPtr_;
        connPtr_ = NULL;
    }
    if (storePtr_)
    {
        delete storePtr_;
        storePtr_ = NULL;
    }
    return true;
}
void *DbInterface::GetConnect()
{
    return connPtr_->GetConnect();
}
std::string DbInterface::ConnErrorMsg()
{
    return connPtr_->What();
}
bool DbInterface::CallProcedure(const std::string& sql)
{
    std::string errMsg;
    if (sql.empty()) return true;
    if (!GetConnect())
    {
        errMsg = ConnErrorMsg();
        return false;
    }
    if (storePtr_->Exec(sql)) return true;
    errMsg = "Exec Err: ";
    errMsg += StoreErrorMsg();
    // log
    return false;
}
bool DbInterface::UpdateSql(const std::string& sql)
{
    return CallProcedure(sql);
}



std::string DbInterface::StoreErrorMsg()
{
    return storePtr_->What();
}
bool DbInterface::Query(std::string &dml)
{
    return storePtr_->Query(dml);
}
unsigned long DbInterface::RowCount()
{
    return storePtr_->RowCount();
}
std::string DbInterface::GetItemString(unsigned long row, unsigned int index)
{
    return storePtr_->GetItemString(row, index);
}
std::string DbInterface::GetItemString(unsigned long row, std::string &fieldName)
{
    return storePtr_->GetItemString(row, fieldName);
}
float DbInterface::GetItemFloat(unsigned long row, unsigned int index)
{
    return storePtr_->GetItemFloat(row, index);
}
float DbInterface::GetItemFloat(unsigned long row, std::string &fieldName)
{
    return storePtr_->GetItemFloat(row, fieldName);
}
long DbInterface::GetItemLong(unsigned long row, unsigned int index)
{
    return storePtr_->GetItemLong(row, index);
}
long DbInterface::GetItemLong(unsigned long row, std::string &fieldName)
{
    return storePtr_->GetItemLong(row, fieldName);
}
// long long DbInterface::GetItemLongLong(unsigned long row, unsigned int index)
// {
//     return storePtr_->GetItemLongLong(row, index);
// }
// long long DbInterface::GetItemLongLong(unsigned long row, std::string &fieldName)
// {
//     return storePtr_->GetItemLongLong(row, fieldName);
// }
