
//

#ifndef _PRODUCT_HPP_
#define _PRODUCT_HPP_

#include <string>

typedef std::string Sex;
typedef std::string Role;
typedef std::string Face;
typedef std::string Costume;
typedef std::string HairStyle;

class Actor 
{
private:
    Sex sex_;
    Role role_;
    Face face_;
    Costume constume_;
    HairStyle hairstyle_;

public:
    // 构造 析构
    Actor()
        : sex_(""), role_(""), face_(""), constume_(""), hairstyle_("")
    {

    }

    ~Actor()
    {

    }

    // sex
    Sex GetSex()
    {
        return sex_;
    }
    void SetSex(Sex sex)
    {
        sex_ = sex;
    }
    // role
    Role GetRole()
    {
        return role_;
    }
    void SetRole(Role role)
    {
        role_ = role;
    }
    // face
    Face GetFace()
    {
        return face_;
    }
    void SetFace(Face face)
    {
        face_ = face;
    }
    // costume
    Costume GetCostume()
    {
        return constume_;
    }
    void SetCostume(Costume costume)
    {
        constume_ = costume;
    }
    // hairstyle
    HairStyle GetHairStyle()
    {
        return hairstyle_;
    }
    void SetHairStyle(HairStyle hairstyle)
    {
        hairstyle_ = hairstyle;
    }

};

#endif