#ifndef _ENTRY_H_
#define _ENTRY_H_


namespace MYSTL
{

// 词条模板类
template <typename K, typename V>
class Entry 
{
public:
    K key;      // 关键码
    V value;    // 数值

// 构造
    Entry(K k = K(), V v = V())
        : key(k), value(v)
    {

    }
    Entry(const Entry<K,V> & e)
        : key(e.key), value(v.value)
    {

    }
// 比较重载
    bool operator<(const Entry<K,V> & e)
    {
        return key < e.key;
    }
    bool operator>(const Entry<K,V> & e)
    {
        return key > e.key;
    }
    bool operator==(const Entry<K,V> & e)
    {
        return key == e.key;
    }
    bool operator!=(const Entry<K,V> & e)
    {
        return key != e.key;
    }
};

};

#endif