#ifndef INT_H
#define INT_H

#include "variable.h"



class Int : public Variable
{
    int value = 0;
public:
    Int();
    Int(int);
    int getValue() const;
    void setValue(int value);
};

#endif // INT_H
