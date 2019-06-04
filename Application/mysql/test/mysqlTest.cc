
//

#include <iostream>
#include "../dbInterface.h"
#include <assert.h>

int main(int argc, char const *argv[])
{
    DbParam dbParam("127.0.0.1", "root", "111111", "chess", 3306);
    DbInterface* db = new DbInterface();
    bool res = db->ConnectDb(dbParam);
    assert(res);
    std::string sql = "select * from staff;";
    // db->CallProcedure(sql);
    std::string idF = "id";
    std::string nameF = "name";
    std::string entryF = "entrytime";
    if (db->Query(sql))
    {
        unsigned int count = db->RowCount();
        for (unsigned int i = 0; i < count; ++i)
        {
            long id = db->GetItemLong(i, idF);
            std::string name = db->GetItemString(i, nameF);
            std::string timeV = db->GetItemString(i, entryF);
            std::cout << "| " << id << " | " << name << " | " << timeV << " |"  << std::endl;
        }
    }
    return 0;
}
