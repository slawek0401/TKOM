#ifndef FUNCTION_H
#define FUNCTION_H

#include "variable.h"
#include <vector>


class Function : public Variable
{
    std::string returnType;

public:
    std::vector<std::string> parametersTypes;
    std::vector<std::string> parametersNames;

    Function();
    std::string getReturnType() const;
    void setReturnType(const std::string &value);
    std::vector<std::string> getParametersTypes() const;
    void setParametersTypes(const std::vector<std::string> &value);
    std::vector<std::string> getParametersNames() const;
    void setParametersNames(const std::vector<std::string> &value);
};

#endif // FUNCTION_H
