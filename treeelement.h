#ifndef TREEELEMENT_H
#define TREEELEMENT_H

#include "Token.h"
#include "variable.h"
#include <vector>

enum treeElementTypes {tokenType, rootType, functionType, codeType, paramType, definitionType, ValueType, functionCallType, expressionType, bracketExpressionType};

class TreeElement
{
public:
    Token token;
    TreeElement* parent;
    std::vector<TreeElement*> children;
    treeElementTypes type;
    std::vector<Variable*> variables;

public:
    TreeElement(Token);
    TreeElement(treeElementTypes);
    void setAsChild(TreeElement*);
};

#endif // TREEELEMENT_H
