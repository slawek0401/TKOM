#include "function.h"

std::string Function::getReturnType() const
{
    return returnType;
}

void Function::setReturnType(const std::string &value)
{
    returnType = value;
}

std::vector<std::string> Function::getParametersTypes() const
{
    return parametersTypes;
}

void Function::setParametersTypes(const std::vector<std::string> &value)
{
    parametersTypes = value;
}

std::vector<std::string> Function::getParametersNames() const
{
    return parametersNames;
}

void Function::setParametersNames(const std::vector<std::string> &value)
{
    parametersNames = value;
}

Function::Function()
{

}
