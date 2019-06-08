#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "citymap.h"
#include "treeelement.h"
#include <map>

class Interpreter
{
    TreeElement *root;
    std::map<std::string, int> variable_types;

public:
    Interpreter();
    CityMap run();

private:
    void setFunctions();
    size_t findFunctionByName(std::string);
    Variable* handleFunctionCode(TreeElement* currFunc, std::vector<Variable*> parameters);
    void error(std::string message);
    void handleDefinition(TreeElement *currFunc, TreeElement *i);
    Variable* handleFunCal(TreeElement *currFunc, TreeElement *i, std::string);
    void handleAssigment(TreeElement *currFunc, TreeElement *i);
    long findVariableByName(std::vector<Variable *> vec, std::string name);
    Variable *findVariableRecursivelyByName(TreeElement *currFun, std::string name);
    void handleIndentifier(TreeElement *currFunc, TreeElement *i);
    void handleCityMapBuildInMethod(TreeElement *currFunc, TreeElement *i, Variable*);
    void handleStreetBuildInMethod(TreeElement *currFunc, TreeElement *i, Variable*);
    Variable* handleKeyWord(TreeElement *currFunc, TreeElement **i, bool* returnAppear, size_t*);
    bool getBoolRValue(TreeElement *currFunc, TreeElement *i);
    Street getStreetRValue(TreeElement *currFunc, TreeElement *i);
    CityMap getCityMapRValue(TreeElement *currFunc, TreeElement *i);
    int getIntRValue(TreeElement *currFunc, TreeElement *i);
    double getDoubleRValue(TreeElement *currFunc, TreeElement *i);
    std::string getStringRValue(TreeElement *currFunc, TreeElement *i);
    size_t getMyIndexInParentsChilden(TreeElement *i);
    Variable getVarFromFunctionCall(TreeElement *i);
    double getNumericRValue(TreeElement *currFunc, TreeElement *i);
};

#endif // INTERPRETER_H
