#include "double.h"

double Double::getValue() const
{
    return value;
}

void Double::setValue(double value)
{
    this->value = value;
}

Double::Double()
{

}

Double::Double(double other): value(other){}
