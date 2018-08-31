#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

namespace MYSTL 
{

// 词典模板类 接口定义
template <typename K, typename V>
class Dictionary 
{
public:
    virtual int size() const = 0;   // 当前词条总数
    virtual bool put(K k, V v) = 0; // 插入词条(禁止雷同词条插入时可能失败,依赖于实现)
    virtual V* get(K k) = 0;        // 读取词条
    virtual bool remove(K k) = 0;   // 删除词条
};

};

#endif