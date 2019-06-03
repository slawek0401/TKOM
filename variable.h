#ifndef VARIABLE_H
#define VARIABLE_H

#include "Token.h"

#include <string>

class Variable
{
protected:
    std::string type;
    std::string name;

public:
    Variable();
    std::string getType() const;
    void setType(const std::string &value);
    std::string getName() const;
    void setName(const std::string &value);
};

#endif // VARIABLE_H
