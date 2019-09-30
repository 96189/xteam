#ifndef _B_SUB_TREE_H_
#define _B_SUB_TREE_H_

#include "vector.h"
#include "bNode.h"
namespace MYSTL 
{

// B-树模板类
template <typename T>
class BTree 
{
protected:
    int _size;          // 存放的关键码的总数
    int _order;         // B-树的阶次(分支数) 至少为3
    BTNodePosi(T) _root;    // 根节点
    BTNodePosi(T) _hot;     // search()最后访问的非空节点位置
    
    // 因插入而上溢之后的分裂处理
    void solveOverflow(BTNodePosi(T) v)
    {
        // 递归基:当前节点并未上溢
        if (_order >= v->child.size()) return;
        // 否则 当前节点已超过B-树阶次(分支数)上界 需要分裂
        // 树中每个节点关键码数组为有序的向量
        // 取轴点 数组中位数下标 该位置节点被提升
        Rank s = _order / 2;
        // 新节点存储v的右半边的值 左半边值依然存储在v中
        BTNodePosi(T) u = new BTNode<T>();
        for (Rank j = 0; j < _order - s - 1; ++j)
        {
            u->child.insert(j, v->child.remove(s + 1));
            u->key.insert(j, v->key.remove(s + 1));
        }
        // 删除将要被提升节点s位置相应的孩子指针
        // 移动v最靠右的孩子
        u->child[_order - s - 1] = v->child.remove(s + 1);
        // 新节点的父指针对应记录
        if (u->child[0])
        {
            for (Rank j = 0; j < _order - s; ++j)
            {
                u->child[j]->parent = u;
            }
        }
        // 记录将要上溢到的目标
        BTNodePosi(T) p = v->parent;
        // 若是树根节点发起的上溢 则产生新的树根
        if (!p)
        {
            _root = p = new BTNode<T>();
            p->child[0] = v;
            v->parent = p;
        }
        Rank r = 1 + p->key.search(v->key[0]);
        // 从v中删除s节点 将该节点插入p中
        p->key.insert(r, v->key.remove(s));
        // p的s节点的右指针指向u
        // p的s节点的左指针 不需要显示维护 
        // 由于s插入p中也是按照顺序找到对应位置的因此原来p指向s的左指针 依然可以作为s上升后的d的左指针
        p->child.insert(r + 1, u);
        u->parent = p;

        // 上升一层 若有必要则继续分裂 至多递归log(n)层
        solveOverflow(p);
    }
    // 因删除而下溢之后的合并处理
    void solveUnderflow(BTNodePosi(T) v)
    {
        if ((_order + 1) / 2 <= v->child.size())
            return; //递归基：当前节点并未下溢
        BTNodePosi(T) p = v->parent;
        if (!p)
        { //递归基：已到根节点，没有孩子的下限
            if (!v->key.size() && v->child[0])
            {
                //但倘若作为树根的v已不含关键码，却有（唯一的）非空孩子，则
                _root = v->child[0];
                _root->parent = NULL; //这个节点可被跳过
                v->child[0] = NULL;
                release(v); //并因不再有用而被销毁
            }               //整树高度降低一层
            return;
        }
        Rank r = 0;
        while (p->child[r] != v)
            r++;
        //确定v是p的第r个孩子——此时v可能不含关键码，故不能通过关键码查找
        //另外，在实现了孩子指针的判等器之后，也可直接调用Vector::find()定位
        // 情况1：向左兄弟借关键码
        if (0 < r)
        {                                       //若v不是p的第一个孩子，则
            BTNodePosi(T) ls = p->child[r - 1]; //左兄弟必存在
            if ((_order + 1) / 2 < ls->child.size())
            {                                                       //若该兄弟足够“胖”，则
                v->key.insert(0, p->key[r - 1]);                    //p借出一个关键码给v（作为最小关键码）
                p->key[r - 1] = ls->key.remove(ls->key.size() - 1); //ls的最大关键码转入p
                v->child.insert(0, ls->child.remove(ls->child.size() - 1));
                //同时ls的最右侧孩子过继给v
                if (v->child[0])
                    v->child[0]->parent = v; //作为v的最左侧孩子
                return;                      //至此，通过右旋已完成当前层（以及所有层）的下溢处理
            }
        } //至此，左兄弟要么为空，要么太“瘦”
          // 情况2：向右兄弟借关键码
        if (p->child.size() - 1 > r)
        {                                       //若v不是p的最后一个孩子，则
            BTNodePosi(T) rs = p->child[r + 1]; //右兄弟必存在
            if ((_order + 1) / 2 < rs->child.size())
            {                                            //若该兄弟足够“胖”，则
                v->key.insert(v->key.size(), p->key[r]); //p借出一个关键码给v（作为最大关键码）
                p->key[r] = rs->key.remove(0);           //ls的最小关键码转入p
                v->child.insert(v->child.size(), rs->child.remove(0));
                //同时rs的最左侧孩子过继给v
                if (v->child[v->child.size() - 1]) //作为v的最右侧孩子
                    v->child[v->child.size() - 1]->parent = v;
                return; //至此，通过左旋已完成当前层（以及所有层）的下溢处理
            }
        } //至此，右兄弟要么为空，要么太“瘦”
          // 情况3：左、右兄弟要么为空（但不可能同时），要么都太“瘦”——合并
        if (0 < r)
        {                                       //与左兄弟合并
            BTNodePosi(T) ls = p->child[r - 1]; //左兄弟必存在
            ls->key.insert(ls->key.size(), p->key.remove(r - 1));
            p->child.remove(r);
            //p的第r - 1个关键码转入ls，v不再是p的第r个孩子
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if (ls->child[ls->child.size() - 1]) //v的最左侧孩子过继给ls做最右侧孩子
                ls->child[ls->child.size() - 1]->parent = ls;
            while (!v->key.empty())
            { //v剩余的关键码和孩子，依次转入ls
                ls->key.insert(ls->key.size(), v->key.remove(0));
                ls->child.insert(ls->child.size(), v->child.remove(0));
                if (ls->child[ls->child.size() - 1])
                    ls->child[ls->child.size() - 1]->parent = ls;
            }
            release(v); //释放v
        }
        else
        {                                       //与右兄弟合并
            BTNodePosi(T) rs = p->child[r + 1]; //右兄度必存在
            rs->key.insert(0, p->key.remove(r));
            p->child.remove(r);
            //p的第r个关键码转入rs，v不再是p的第r个孩子
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if (rs->child[0])
                rs->child[0]->parent = rs; //v的最左侧孩子过继给ls做最右侧孩子
            while (!v->key.empty())
            { //v剩余的关键码和孩子，依次转入rs
                rs->key.insert(0, v->key.remove(v->key.size() - 1));
                rs->child.insert(0, v->child.remove(v->child.size() - 1));
                if (rs->child[0])
                    rs->child[0]->parent = rs;
            }
            release(v); //释放v
        }
        solveUnderflow(p); //上升一层，如有必要则继续分裂——至多递归O(logn)层
        return;
    }
    // 释放x指向的树
    void release(BTNodePosi(T) x)
    {
        
    }
public:
    // 构造函数 默认为最低的3阶
    BTree(int order = 3) 
        : _size(0), _order(order)
    {
        _root = new BTNode<T>();
    }
    // 析构函数 释放所有节点
    ~BTree()
    {
        if (_root) release(_root);
    }
    // 阶次
    int order() const 
    {
        return _order;
    }
    // 规模
    int size() const 
    {
        return _size;
    }
    // 树根
    BTNodePosi(T) & root()
    {
        return _root;
    }
    // 判空
    bool empty() const 
    {
        return !_root;
    }
    // 查找
    BTNodePosi(T) search(const T &e)
    {
        BTNodePosi(T) v = _root;
        _hot = NULL;
        // 遍历B-树中的节点
        while (v)
        {
            // 在单个节点(向量)中查找 返回值为最后一个小于等于e的元素下标
            Rank r = v->key.search(e);
            // 等于e的情况 确定命中
            if ((0 <= r) && (e == v->key[r])) return v;
            // 记录最后访问的节点
            _hot = v;
            // 小于e的情况 转入对应的子树 需要I/O
            v = v->child[r + 1];
        }
        // 失败 最终抵达外部节点
        return NULL;
    }
    // 插入
    bool insert(const T &e)
    {
        BTNodePosi(T) v = search(e);
        if (v) return false;    // 已存在 直接返回
        // 在B-树中访问的最后一个节点中 确认e应该插入的位置
        // 既然e不在树中 则此处的数组搜索必然返回最后一个小于e的元素的下标
        Rank r = _hot->key.search(e);
        // 根据位置插入关键码e
        _hot->key.insert(r + 1, e);
        // 插入关键码同时要插入相关的空子树指针
        _hot->child.insert(r + 2, NULL);
        ++_size;
        // 若有必要 需要做分裂
        solveOverflow(_hot);
        return true;
    }
    // 删除
    bool remove(const T &e)
    {
        BTNodePosi(T) v = search(e);
        if (!v) return false;   // 不存在 直接返回
        // 关键码向量中搜索
        Rank r = v->key.search(e);
        // v非叶子节点 e的中序遍历意义下的后继 必然为相邻右指针对应的子树中最左端的节点(同avl树删除的后继)
        if (v->child[0])
        {
            // u指向e的相邻右子树的指针
            BTNodePosi(T) u = v->child[r + 1];
            // 右子树最左端的节点
            while (u->child[0]) u = u->child[0];
            // 实际删除的节点为u 将u的值保存在原来准备删除的节点v中            
            v->key[r] = u->key[0];
            // 指向v的指针 指向u
            v = u;
            r = 0;
        } // 至此 v必然位于最底层 其中第r个关键码是待删除者
        // 删除
        v->key.remove(r);
        v->child.remove(r + 1);
        --_size;
        solveUnderflow(v);
        return true;
    }

}; //BTree

}; // MYSTL

#endif