#ifndef CITYMAP_H
#define CITYMAP_H

#include "street.h"
#include "variable.h"

#include <vector>
#include <iostream>

class CityMap : public Variable
{
    std::vector<Street> streets;
    double width = 0.0;
    double height = 0.0;

public:
    CityMap();
    std::vector<Street> getStreets() const;
    void setStreets(const std::vector<Street> &value);
    double getWidth() const;
    void setWidth(double value);
    double getHeight() const;
    void setHeight(double value);
    void setValue(CityMap other);
    CityMap getValue();
    void addStreet(Street);
    friend std::ostream& operator<< (std::ostream &out, const CityMap &c){
        out << c.getWidth() << "x" << c.getHeight()<<"\t ulice:"<<std::endl;
        for (Street str : c.getStreets()){
            out<<str<<std::endl;
        }
        return out;
    }
};

#endif // CITYMAP_H
