#include "variable.h"

std::string Variable::getType() const
{
    return type;
}

void Variable::setType(const std::string &value)
{
    type = value;
}

std::string Variable::getName() const
{
    return name;
}

void Variable::setName(const std::string &value)
{
    name = value;
}

Variable::Variable()
{

}
