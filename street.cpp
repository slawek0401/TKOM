#include "street.h"

double Street::getX() const
{
    return x;
}

void Street::setX(double value)
{
    x = value;
}

double Street::getY() const
{
    return y;
}

void Street::setY(double value)
{
    y = value;
}

double Street::getWidth() const
{
    return width;
}

void Street::setWidth(double value)
{
    width = value;
}

double Street::getLength() const
{
    return length;
}

double Street::getRotation() const
{
    return rotation;
}

void Street::setRotation(double value)
{
    rotation = value;
}

void Street::setLength(double value)
{
    length = value;
}

Street::Street()
{

}

Street Street::getValue() const{
    return *this;
}

void Street::setValue(Street other){
    this->x = other.getX();
    this->y = other.getY();
    this->width = other.getWidth();
    this->length = other.getLength();
    this->rotation = other.getRotation();
}
