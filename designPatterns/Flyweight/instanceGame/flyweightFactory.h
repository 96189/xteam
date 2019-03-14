
#ifndef _FLYWEIGHT_FACTORY_H_
#define _FLYWEIGHT_FACTORY_H_

#include "flyweightConcrete.h"
#include <unordered_map>
#include <string>

class TreeFactory
{
public:
    static TreeFactory* getInstance()
    {
        if (!instance_)
        {
            instance_ = new TreeFactory();
        }
        return instance_;
    }
    Tree* getTree(std::string mod, Pos pos, double h, double t, Color b, Color l)
    {
        Tree *tree = NULL;
        auto it = treeModels_.find(mod);
        if (it != treeModels_.end())
        {
            tree = new Tree(it->second, pos, h, t, b, l);
        }
        else 
        {
            treeModels_[mod] = new TreeModel();
            tree = new Tree(model, pos, h, t, b, l);
        }
    }
private:
    TreeFactory()
    {

    }
    TreeFactory(const TreeFactory& rhs);
private:
    static TreeFactory *instance_;
    std::unordered_map<std::string, TreeModel*> treeModels_;     // 享元池
};


TreeFactory *TreeFactory::instance_;
#endif