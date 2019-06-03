#ifndef STRING_H
#define STRING_H

#include "variable.h"



class String : public Variable
{
    std::string value = "";
public:
    String();
    String(std::string);
    std::string getValue() const;
    void setValue(const std::string &value);
};

#endif // STRING_H
