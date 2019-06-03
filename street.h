#ifndef STREET_H
#define STREET_H

#include "variable.h"
#include <iostream>


class Street : public Variable
{
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double length = 0.0;
    double rotation = 0.0;
public:
    Street();
    double getX() const;
    void setX(double value);
    double getY() const;
    void setY(double value);
    double getWidth() const;
    void setWidth(double value);
    double getLength() const;
    void setLength(double value);
    double getRotation() const;
    void setRotation(double value);
    Street getValue() const;
    void setValue(Street other);
    friend std::ostream& operator<< (std::ostream &out, const Street &c){
        out << "szer:"<<c.getWidth() << " dl:"<< c.getLength();
        out << " x:" << c.x << " y:" << c.y << " rot:" << c.rotation;
        return out;
    }
};

#endif // STREET_H
