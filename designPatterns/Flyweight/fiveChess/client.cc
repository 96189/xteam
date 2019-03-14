
#include "flyweightFactory.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    FiveChessmanFactory *pFactory = FiveChessmanFactory::getInstance();
    AbstractChessman *chess;
    int choice = 0;
    int n = 10;
    for (int i = 0; i < n; ++i)
    {
        chess = NULL;
        choice = rand() % 2;
        switch (choice)
        {
            case 0:
                chess = pFactory->getChessman('B');
                break;
            case 1:
                chess = pFactory->getChessman('W');
                break;
            default:
                break;
        }

        if (chess != NULL)
        {
            chess->point(i, rand() % n);
        }
    }
    // n个棋子实际上在内存中只产生两个对象 可共享的内部状态为黑白  不可共享的由外部客户端动态改变的外部状态为坐标
    return 0;
}