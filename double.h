#ifndef DOUBLE_H
#define DOUBLE_H

#include "variable.h"



class Double : public Variable
{
    double value = 0.0;
public:
    Double();
    Double(double);
    double getValue() const;
    void setValue(double value);
};

#endif // DOUBLE_H
