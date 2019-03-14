
#ifndef _FLYWEIGHT_FACTORY_H_
#define _FLYWEIGHT_FACTORY_H_

#include <unordered_map>
#include "flyweightConcrete.h"

class FiveChessmanFactory 
{
public:
    static FiveChessmanFactory* getInstance()
    {
        if (!instance_)
        {
            instance_ = new FiveChessmanFactory();
        }
        return instance_;
    }

    AbstractChessman* getChessman(char c)
    {
        AbstractChessman* chess = NULL;
        auto it = chessPool_.find(c);
        if (it != chessPool_.end())
        {
            chess = it->second;
        }
        else 
        {
            if (c == 'B')
            {
                chess = new BlackChessman();
            }
            if (c == 'W')
            {
                chess = new WhiteChessman();
            }
            if (chess != NULL)
            {
                chessPool_[c] = chess;
            }
        }
        return chess;
    }

private:
    FiveChessmanFactory()
    {

    }
    FiveChessmanFactory(const FiveChessmanFactory&);

    std::unordered_map<char, AbstractChessman*> chessPool_;     // 享元池
    static FiveChessmanFactory *instance_;
};

FiveChessmanFactory *FiveChessmanFactory::instance_ = NULL;

#endif