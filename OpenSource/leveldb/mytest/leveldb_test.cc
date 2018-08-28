#include <iostream>
#include <cassert>
#include <string>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

using namespace std;

int main(int argc, char *argv[])
{
// open
    leveldb::DB *db;
    leveldb::Options options;
    options.error_if_exists = true;
    options.create_if_missing = true;
    leveldb::Status s = leveldb::DB::Open(options, "/tmp/testdb", &db);
    
// status
    if (!s.ok())
        cerr << s.ToString() << endl;

// set value
    string key1 = "bigtable";
    string val1 = "leveldb";
    s = db->Put(leveldb::WriteOptions(), key1, val1);
    if (!s.ok())
        cerr << s.ToString() << endl;    

// read and write
    string value;
    string key2 = "single bigtable";
    s = db->Get(leveldb::ReadOptions(), key1, &value);
    if (s.ok()) 
    {
        cout << key1 << " : " << value << endl;
        s = db->Put(leveldb::WriteOptions(), key2, value);
    }
    if (s.ok())
    {
        s = db->Delete(leveldb::WriteOptions(), key1);
    }

// atomic updates

// sync write
// async write

// iteration
    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }
    assert(it->status().ok());
    delete it;

// snapshots
    leveldb::ReadOptions roptions;
    // snapshot非空表示在特定的版本状态上读取
    // 空表示读取操作将在当前状态上进行隐式的快照操作
    roptions.snapshot = db->GetSnapshot();
    // 更新key2 
    s = db->Put(leveldb::WriteOptions(), key2, "google");
    leveldb::Iterator *iter = db->NewIterator(roptions);
    // 读取 读到快照版本的数据
    for (iter->SeekToFirst(); iter->Valid(); iter->Next())
    {
        cout << iter->key().ToString() << " : " << iter->value().ToString() << endl;
    }
    assert(iter->status().ok());
    delete iter;
    db->ReleaseSnapshot(roptions.snapshot);
    // 再读取 读到已经修改过的数据
    leveldb::Iterator *riter = db->NewIterator(leveldb::ReadOptions());
    for (riter->SeekToFirst(); riter->Valid(); riter->Next())
    {
        cout << riter->key().ToString() << " : " << riter->value().ToString() << endl;
    }
    delete riter;

    // 写入大量文件 生成日志
    for (int i = 3; i < 1024*1024; ++i)
    {
        s = db->Put(leveldb::WriteOptions(), "key" + to_string(i), "value" + to_string(i));
    }
// close
    delete db;
    return 0;
}