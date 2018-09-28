
//

#ifndef _MSG_HPP_
#define _MSG_HPP_

#include <string>

enum MSG_TYPE 
{
    NONE = 0x0,
    LOGIN,
    LOGOUT,
    RECONNECT,
    COMMENT,
    BUY,
    CANCELBUY,
    SAVE,
    SEARCH,
    ADD,
    DELETE
};

class Msg 
{
public:
    int type_;
    std::string msg_;

public:
    Msg(int type=NONE, std::string msg="")
        : type_(type), msg_(msg)
    {

    }
    ~Msg()
    {

    }
};

#endif