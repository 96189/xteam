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
    // cache节点的物理存储视角
    // Cache表 hash map 查询效率O(1)
    std::unordered_map<K, Cache<K, V>*> values_;                // key -> node             key到对应节点的映射
    // cache节点->频率表 hash map 查询效率O(1) 此表的存在是为了中间层转换实现key->counts->lists的查询
    std::unordered_map<K, int> counts_;                         // key -> counts_          key到频率的映射
    // cache节点的逻辑存储视角
    // 频率->cache节点列表的表 sorted map 按照频率排序 频率相同时 优先驱逐先存入map中的cache
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
        // 手动释放new过的内存
        for (typename std::unordered_map<K, Cache<K, V>*>::iterator it = values_.begin();
             it != values_.end(); ++it)
        {
            delete it->second;
        }
        for (typename std::map<int, std::list<Cache<K, V>*>*>::iterator lit = frequencies_.begin();
             lit != frequencies_.end(); ++lit)
        {
            lit->second->clear();
        }
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

template <typename K, typename V>
class LRUCache : public CacheInterface<K, V>
{
private:
    // hash map cache节点逻辑存储视角 O(1)查询
    std::unordered_map<K, Cache<K, V>*> values_;
    // cache节点物理存储视角 列表节点的排列按照由新到旧的顺序
    std::list<Cache<K, V>*> logiclists_;
    int max_capacity_;
    int used_;

public:
// 构造与析构
    LRUCache(int cap = CACHE_COUNT_MAX)
        : max_capacity_(cap), used_(0)
    {
        values_.clear();
        logiclists_.clear();
    } 
    ~LRUCache()
    {
        // 手动释放new过的内存
        for (typename std::list<Cache<K, V>*>::iterator it = logiclists_.begin();
             it != logiclists_.end(); ++it)
        {
            delete (*it);
        }
        values_.clear();
        logiclists_.clear();        
    }
// 接口实现
    virtual void New()
    {
        return;
    }
    virtual bool Get(K key, V& v) 
    {
        typename std::unordered_map<K, Cache<K, V>*>::iterator it = values_.find(key);
        if (it == values_.end())     
        {
            v = V();
            return false;
        }    
        // 查询到key对应的cache节点 将对应节点移动到链表最前端 保持由新到旧的顺序
        v = it->second->value;
        Cache<K, V> *node = new Cache<K,V>(key, v);

        logiclists_.remove(it->second);
        logiclists_.push_front(node);

        values_[key] = node;
        return true;
    }
    virtual void Set(K key, V value) 
    {
        Cache<K, V> *node = NULL;
        typename std::unordered_map<K, Cache<K, V>*>::iterator it = values_.find(key);
        // 已存在更新值
        if (it != values_.end())     
        {
            it->second->value = value;
            // 已在链表最前端(已最新)
            if (logiclists_.front() == it->second) return;
            // 不在链表前端 需要调整cache节点位置 
            logiclists_.remove(it->second);
        }
        // 否则生成新cache节点
        node = new Cache<K,V>(key, value);
        // 检查是否驱逐缓存
        if (values_.size() >= max_capacity_)
        {
            Evict(values_.size() - max_capacity_);
        }

        logiclists_.push_front(node);

        values_[key] = node;
        ++used_;
    }
    virtual void Evict(int count)
    {
        // 链表总是保持由新到旧 驱逐缓存 操作链表末尾即可
        // 缓存刚满 此时count为0 插入新值需要驱逐一块缓存
        while (count-- >= 0)
        {
            values_.erase(logiclists_.back()->key);
            logiclists_.pop_back();
            --used_;
        }
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
// 测试
    void PrintTable()
    {
        typename std::unordered_map<K, Cache<K, V>*>:: iterator it = values_.begin();
        for ( ; it != values_.end(); ++it)
        {
            std::cout << "key: " << it->first <<  " -> k: "  << it->second->key <<  " v: " << it->second->value << std::endl;
        }
        for (typename std::list<Cache<K, V> *>::iterator lit = logiclists_.begin();
             lit != logiclists_.end();
             ++lit)
        {
            std::cout << "node: k: " << (*lit)->key << " v: " << (*lit)->value << " => ";
        }
        std::cout << std::endl;
    }
};

#endif
