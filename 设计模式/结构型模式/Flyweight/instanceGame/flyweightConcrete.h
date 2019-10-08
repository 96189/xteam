
#ifndef _FLYWEIGHT_CONCRETE_H_
#define _FLYWEIGHT_CONCRETE_H_

#include "flyweight.h"

struct Pos
{
    int x;
    int y;
};
struct Color
{
    int c;
};
struct Mesh
{
    int m;
};
struct Texture
{
    int t;
};


struct TreeModel
{
    Mesh mesh_;     // 网格模型
    Texture bole_;  // 树干纹理
    Texture leaves_;// 树叶纹理
};

// 具体享元类
class Tree : public ITree
{
public:
    Tree()
    {

    }
    Tree(TreeModel* model, Pos pos, double height, double thickness, Color bole, Color leaf)
    : model_(model)
    {
        setParam(pos, height, thickness, bole, leaf);
    }

public:
    virtual void setParam(Pos pos, double height, double thickness, Color bole, Color leaf)
    {
        pos_ = pos;
        height_ = height;
        thickness_ = thickness;
        boleColor_ = bole;
        leafColor_ = leaf;
    }
private:
    TreeModel *model_;      // 可以共享的内部状态
    Pos pos_;               // 以下,通过接口由客户端改变的外部状态
    double height_;
    double thickness_;
    Color boleColor_;
    Color leafColor_;
};

#endif