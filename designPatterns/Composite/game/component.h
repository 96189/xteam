
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GraphicsContext;
class IMapNode
{
public:
    virtual ~IMapNode() {};

    virtual void cycle() = 0;
    virtual void draw(GraphicsContext& g) = 0;
};

#endif