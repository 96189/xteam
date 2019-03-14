
#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include "component.h"
#include <vector>

class IMapNode;
class NodeLayer : public IMapNode
{
public:
    void addChild(IMapNode* child)
    {
        childrens_.push_back(child);
    }
    void removeChild(IMapNode* child)
    {
        auto it = std::find(childrens_.begin(), childrens_.end(), child);
        if (it != childrens_.end())
        {
            childrens_.erase(it);
        }
    }
public:
    virtual void cycle() 
    {
        for (auto it = childrens_.begin(); it != childrens_.end(); ++it)
        {
            (*it)->cycle();
        }
    }
    virtual void draw(GraphicsContext& g)
    {   
        for (auto it = childrens_.begin(); it ！= childrens_.end(); ++it)
        {
            (*it)->draw(g);
        }
    }

protected:
    std::vector<IMapNode*> childrens_;
};

class NodeRoot : public NodeLayer
{
public:
    virtual void cycle() 
    {
        // update the state of the tree
    }
    virtual void draw(GraphicsContext& g) 
    {
        // draw the tree via g
    }
};

#endif