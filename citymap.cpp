#include "citymap.h"

std::vector<Street> CityMap::getStreets() const
{
    return streets;
}

void CityMap::setStreets(const std::vector<Street> &value)
{
    streets = value;
}

double CityMap::getWidth() const
{
    return width;
}

void CityMap::setWidth(double value)
{
    width = value;
}

double CityMap::getHeight() const
{
    return height;
}

void CityMap::setHeight(double value)
{
    height = value;
}

CityMap::CityMap()
{

}

void CityMap::setValue(CityMap other){
    this->width = other.getWidth();
    this->height = other.getHeight();
    this->streets = other.getStreets();
    this->name = other.getName();
    this->type = other.getType();
}

CityMap CityMap::getValue(){
    return *this;
}

void CityMap::addStreet(Street str){
    streets.push_back(str);
}

