
#ifndef  __QTREE_H_
#define  __QTREE_H_

#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <string>

#define M_TO_CM(n)  ((n)*100)   //米转换成厘米
#define CM_TO_M(n)  ((n)/100)   //厘米转换成米

namespace qtree {
    //坐标值类型，根据需求来定义成相应类型
    typedef int pos_type;
    //typedef signed long pos_type;
    //节点数据类型，根据需求来定义成相应类型
    typedef unsigned long node_data_type;
    //子节点计数类型，根据实际规模来选用相应类型
    typedef unsigned long node_count_type;

    //点定义
    struct Point {
        pos_type x;
        pos_type y;
      
        bool operator < (const Point& pt) const;
        bool operator==(const Point& pt) const;
    };

    //外包矩形定义
    class Bound {
        public:
            pos_type left;
            pos_type right;
            pos_type top;
            pos_type bottom;

            //设置矩形的值
            void SetData(pos_type left, pos_type top, pos_type right, pos_type bottom);
            //检查矩形是否为空
            bool IsEmpty() const;
            //检验是否还可分
            bool CanDivide() const;
            //获得矩形的中心点
            Point Center() const;
            //检验两个矩形是否相交
            bool Intersect(const Bound &bound) const;
            //检验是否包含指定点
            bool Contain(const Point &point) const;
            //检验是否包含指矩形
            bool Contain(const Bound &bound) const;
    };

    //内部节点，外部不要使用
    struct InterNodeRecord {
        Bound bound;
        node_count_type child_count;
    };

    class Node;

    //数据节点
    struct DataNodeRecord {
        Point position;
        node_data_type data;
        Node* next; //链表模式下的下一个Node

        void Print() {
            char buff[1024] = {'\0'};
            snprintf(buff, 1024, "pos:(%d,%d) data:%d next:%p\n", position.x, position.y, data, next);
            printf("%s", buff);
        }
    };

    //节点类型定义，DATA代表数据节点，INTER代表内部节点
    enum NodeType {
        DATA, INTER,
    };

    //QTree节点定义
    class Node {
        public:
            NodeType type;
            union {
                InterNodeRecord inter;
                DataNodeRecord data;
            } record;

            //子节点
            Node *child[4];

            //默认构造函数，构造的是一个空内部节点
            Node();
            //根据指定矩形构造一个内部节点
            Node(const Bound &bound);
            //根据位置坐标和数值构造一个数据节点
            Node(const Point &position, node_data_type data);
            virtual ~Node();
    };

    //四叉树
    class QTree {
        public:
            //构造函数，根节点为外包范围是bound的矩形
            explicit QTree(const Bound &bound);
            //构造函数，从指定dump文件构建四叉树
            explicit QTree(const char *file_name) throw (int);
            ~QTree();
            //添加一个数据节点，位置为position，值为data
            bool AddNode(const Point &position, node_data_type data);
            //查找指定矩形范围内的所有点，返回结果在result数组内
            int Search(const Bound &query, std::vector<qtree::DataNodeRecord> &result, int &search_times) const;
            int SearchForNearestPoint( qtree::pos_type x, qtree::pos_type y,
                    size_t max_radius);
            //查找指定矩形包围的点的个数
            int GetPointCount(const Bound &query);
            //查找指定矩形包围内是否有点
            bool GetPointExistance(const Bound &query);
            //将树dump到指定文件
            bool DumpToFile(const char *file_name);
            //从指定文件加载四叉树
            bool LoadFromDumpFile(const char *file_name);
            //设置链表模式.ture: list, false: non-list
            void set_list_mode(bool be_list);
            //查询是否支持链表模式
            bool get_list_mode();
        private:
            //根节点
            Node *root;
            //是否是从dump文件创建
            bool dump_mode;
            //数据节点是否支持链表模式
            bool be_list;
            //添加一个节点
            bool AddNode(Node *root, Node *node);
            //删除一个节点
            void DeleteNode(Node *node);
            //以root为根节点查找query矩形内的的所有点，结果在result内
            int Search(Node *root, const Bound &query,
                    std::vector<qtree::DataNodeRecord> &result, int &search_times) const;
            //查找指定矩形包围的点的个数
            int GetPointCount(Node *root, const Bound &query);
            //查找指定矩形包围内是否有点
            bool GetPointExistance(Node* root, const Bound &query);
            //将树dump到指定文件
            bool DumpToFile(FILE *fp);
            //dump指定节点
            bool DumpNode(FILE *fp, Node *node);
            //从一段连续内存加载四叉树
            Node * LoadNodeFromBuffer(const char *buffer, size_t &offset);
            //从dump文件加载四叉树
            bool LoadFromDumpFile(FILE *fp);
            int RectSearchForNearestPoint(qtree::pos_type x, qtree::pos_type y,
                    size_t max_radius, size_t radius = 1,
                    size_t last_radius = 0, int max_empty_radius = -1,
                    int min_overwhelm_radius = -1, bool bForceEnd = false);
    };

}
;

#endif  //__QTREE_H_
