#include "string.h"

std::string String::getValue() const
{
    return value;
}

void String::setValue(const std::string &value)
{
    this->value = value;
}

String::String()
{

}

String::String(std::string other) : value(other){}
