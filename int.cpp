#include "int.h"

int Int::getValue() const
{
    return value;
}

void Int::setValue(int value)
{
    this->value = value;
}

Int::Int()
{

}

Int::Int(int other) : value(other)
{

}
