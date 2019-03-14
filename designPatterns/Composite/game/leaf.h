
#ifndef _LEAF_H_
#define _LEAF_H_

#include "component.h"

class NodeTree : public IMapNode
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

class NodePlayer : public IMapNode
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