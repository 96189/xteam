#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_

#include "entry.h"
#include "dictionary.h"
#include "quadlist.h"

namespace MYSTL 
{

template <typename K, typename V>
class SkipList : public Dictionary<K,V>, public List<Quadlist<Entry<K,V>>*>
{
protected:
    // skiplist词条查找算法(内部使用)
    // qlist 为顶层列表 p为qlist的首节点
    // 若成功 p为命中关键码所属塔的顶部节点 qlist为p所属列表
    // 若失败 p为所属塔的基座 该塔对应于不大于k的最大的且最靠右的关键码 qlist为空
    // 多个词条命中时 沿四联表取最靠后者
    bool skipSearch(ListNode<Quadlist<Entry<K,V>>*>* &qlist, QuadlistNode<Entry<K,V>>* &p, K& k)
    {
        // 向右向下查找目标关键码
        while (true)
        { // 在每一层
            while (p->succ && (p->entry.key <= k)) // 从前向后查找
                p = p->succ; // 直到出现更大的key甚至溢出至trailer
            p = p->pred; // 回退一步
            if (p->pred && (k == p->entry.key)) return true; // 命中
            // 未命中 转入下一层
            qlist = qlist->succ;
            if (!qlist->succ) return false; // 已经穿透底层 则表中不存在对应的k 搜索失败
            // 尚未穿透底层 可开始下一层查找
            p = (p->pred) ? p->below : qlist->data->first(); // 转至当前塔的下一节点
        }
        return false;
    }
public:
    // 底层Quadlist的规模
    int size() const 
    {
        return empty() ? 0 : last()->data->size();
    }
    // 跳转表词条插入算法(skiplist允许词条重复 必然成功)
    bool put(K k, V v)
    {
        Entry<K, V> e = Entry<K, V>(k, v);  // 构造词条
        if (empty()) insertAsFirst(new Quadlist<Entry<K, V>>);  // 插入首个entry
        // 从四联表的首节点出发
        ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
        QuadlistNode<Entry<K, V>>* p = qlist->data->first();
        // 查找适当的位置插入 (不大于关键码k的最后一个节点p)
        if (skipSearch(qlist, p, k))
            while (p->below ) p = p->below; // 若已有雷同词条 则需要强制转到塔底
        // 紧邻与p的右侧 一座新塔将自底而上逐层生长
        qlist = last();
        // 新节点b即为新塔基座 (e作为p的后继插入)
        QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);
        while (rand() & 1)  // 经抛硬币 若确定新塔需要再长高一层
        {
            // 找出不低于此高度的最近前驱
            while (qlist->data->valid(p) && !p->above) p = p->pred;
            // 若该前驱是header哨兵
            if (!qlist->data->valid(p))
            {
                // 且当前已是最顶层
                if (qlist == first())
                    // 则必须创建新的一层
                    insertAsFirst(new Quadlist<Entry<K, V>>);
                // p转至上一层skiplist的header
                p = qlist->pred->data->first()->pred;
            }
            // 否则
            else 
            {
                // 将p提升至该高度
                p = p->above;
            }
            // 上升一层
            qlist = qlist->pred;
            // 将新节点e插入p之后 b之上
            b = qlist->data->insertAfterAbove(e, p, b);
        }
        return true;
    }
    // 跳转表词条查找算法
    V* get(K k)
    {
        if (empty()) return NULL;
        // 从顶层的Quadlist的首节点开始
        ListNode<Quadlist<Entry<K,V>>*>* qlist = first(); 
        QuadlistNode<Entry<K,V>>* p = qlist->data->first();
        // 查找并报告
        return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
    }   // 有多个命中时靠后者优先
    // 跳转表词条删除算法
    bool remove(K k)
    {
        if (empty()) return false;  // 空表
        // 从顶层Quadlist的首节点开始
        ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
        QuadlistNode<Entry<K, V>>* p = qlist->data->first();
        // 目标词条不存在 直接返回
        if (!skipSearch(qlist, p, k)) return false;
        do // 目标词条存在 则逐层拆除与之对应的塔
        {
            QuadlistNode<Entry<K, V>>* lower = p->below;    // 记住下一层节点
            qlist->data->remove(p); // 删除当前层节点
            // 转入下一层
            p = lower;
            qlist = qlist->succ;
        } while (qlist->succ); // 直到塔基
        // 反复清除已可能不含词条的顶层Quadlist
        while (!empty() && first()->data->empty())
            List::remove(first());
        return true;
    }
    // 层高
    int level()
    {
        return List::size();
    }
};

};

#endif 