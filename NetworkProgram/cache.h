#ifndef _CACHE_H_
#define _CACHE_H_

// https://deepakvadgama.com/blog/lfu-cache-in-O(1)/

#include <unordered_map>
#include <map>
#include <list>
#include <iostream>

template <typename K, typename V>
class Cache 
{
public:
    K key;
    V value;
public:
    Cache(K k, V v)
        : key(k), value(v)
    {

    }
    Cache& operator=(const Cache& c)
    {
        if (&c != this)
        {
            c.key = key;
            c.value = value;
        }
    }
    K getkey()
    {
        return key;
    }
    V getvalue()
    {
        return value;
    }
};

template <typename K, typename V>
class CacheInterface 
{
public:
    virtual void New() = 0;                             // 创建Cache
    virtual bool Get(K key, V& v) = 0;                  // 获取cache
    virtual void Set(K key, V value) = 0;               // 设置cache
    virtual void Evict(int count) = 0;                  // 驱逐count个cahce
    virtual int Used() = 0;                             // 获取在用的cache个数
};

#define CACHE_COUNT_MAX 3
#define CACHE_COUNT_MIN (CACHE_COUNT_MAX/2)

template <typename K, typename V>
class LFUCache : public CacheInterface<K, V>
{
private:
    // Cache表 hash map 查询效率O(1)
    std::unordered_map<K, Cache<K, V>*> values_;                // key -> node             key到对应节点的映射
    // cache节点->频率表 hash map 查询效率O(1) 此表的存在是为了中间层转换实现key->counts->lists的查询
    std::unordered_map<K, int> counts_;                         // key -> counts_          key到频率的映射
    // 频率->cache节点列表的表 sorted map 频率相同时 优先驱逐先存入map中的cache
    std::map<int, std::list<Cache<K, V>*>*> frequencies_;       // counts_ -> value list 频率到节点列表的映射
    int max_capacity_;
    int used_;

    void removeIfListEmpty(int frequency)
    {
        if (frequencies_.at(frequency)->size() == 0)
        {
            frequencies_.erase(frequency);
        }
    }
public:
// 构造与析构
    LFUCache(int capacity = CACHE_COUNT_MAX)
        : max_capacity_(capacity), used_(0)
    {
        values_.clear();
        counts_.clear();
        frequencies_.clear();
    }
    ~LFUCache()
    {
        values_.clear();
        counts_.clear();
        frequencies_.clear();      
    }
// 接口实现
    virtual void New()
    {
        return;
    }
    // 驱逐缓存块
    virtual void Evict(int count)
    {
        // 缓存刚满 此时count为0 插入新值需要驱逐一块缓存
        while (count-- >= 0)
        {
            int lowestCount = frequencies_.begin()->first;
            Cache<K, V> *node2del = frequencies_.at(lowestCount)->front();
            K key2del = node2del->getkey();
            frequencies_.at(lowestCount)->remove(node2del);
            removeIfListEmpty(lowestCount);
            values_.erase(key2del);
            counts_.erase(key2del);
            --used_;
        } 
    }
    // O(1)
    virtual bool Get(K key, V& v)
    {
        // 查cache表
        typename std::unordered_map<K, Cache<K, V>*>::iterator it = values_.find(key);
        if (it == values_.end()) 
        {
            v = V();
            return false;
        }

        // (k,v)在表中
        // 获取cache节点
        Cache<K, V> *node = values_.at(key);

        // 获取cache节点的频率 并在对应频率下的列表中移除该cache节点
        int frequency = counts_.at(key);
        frequencies_.at(frequency)->remove(node);
        // 有必要的话移除cache节点所在的列表
        removeIfListEmpty(frequency);

        // 将cache节点移动到新的频率对应的列表下(尾插)
        typename std::map<int, std::list<Cache<K, V>*>*>::iterator fit = frequencies_.find(frequency + 1);
        if (fit == frequencies_.end())
        {
            frequencies_[frequency + 1] = new std::list<Cache<K, V>*>;
        }
        frequencies_[frequency + 1]->push_back(node);

        // 更新节点频率
        counts_[key] = frequency + 1;
        v = values_.at(key)->value;
        return true;
    }
    // O(1)
    virtual void Set(K key, V value)
    {
        typename std::unordered_map<K, Cache<K, V>*>::iterator it = values_.find(key);
        // 节点已存在 则更新值 且更新频率
        if (it != values_.end()) 
        {
            values_[key]->value = value;
            counts_[key] = counts_.at(key) + 1;
            return;
        }

        // 节点不存在 生成cache节点 且频率必为1 插入3个表中
        Cache<K, V> *node = new Cache<K,V>(key, value);
        // 检查是否驱逐缓存
        if (values_.size() >= max_capacity_)
        {
            Evict(values_.size() - max_capacity_);
        }

        values_[key] = node;
        counts_[key] = 1;
        typename std::map<int, std::list<Cache<K, V>*>*>::iterator fit = frequencies_.find(1);
        if (fit == frequencies_.end())
        {
            frequencies_[1] = new std::list<Cache<K, V>*>;
        }

        frequencies_[1]->push_back(node);
        ++used_;
    }
    virtual int Used()
    {
        return used_;
    }
//
    int Size() const
    {
        return max_capacity_;
    }
// 测试
    void PrintTable()
    {
        // key -> node
        typename std::unordered_map<K, Cache<K, V>*>:: iterator it = values_.begin();
        for ( ; it != values_.end(); ++it)
        {
            std::cout << "key: " << it->first <<  " -> k: "  << it->second->key <<  " v: " << it->second->value << std::endl;
        }
        std::cout << std::endl;
        // key -> counts_
        typename std::unordered_map<K, int>:: iterator itt = counts_.begin();
        for ( ; itt != counts_.end(); ++itt)
        {
            std::cout << "key: " << itt->first << " counts_: " << itt->second << std::endl;
        }
        std::cout << std::endl;
        // counts_ -> value list
        typename std::map<int, std::list<Cache<K, V>*>*>::iterator ittt = frequencies_.begin();
        for ( ; ittt != frequencies_.end(); ++ittt)
        {
            std::cout << "counts_: " << ittt->first << std::endl;
            for (typename std::list<Cache<K, V>*>::iterator lit = ittt->second->begin();
                 lit != ittt->second->end();
                 ++lit)
            {
                std::cout << "node: k: " << (*lit)->key << " v: " << (*lit)->value << " => ";
            }
            std::cout << std::endl;
        }
    }
};

#endif
