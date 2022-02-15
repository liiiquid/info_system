#include "user.h"

user::user()
{

}

user::user(int id, QString name, int type)
{
    this->id = id;
    this->name = name;
    this->type = type;
}
