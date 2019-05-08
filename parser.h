#ifndef PARSER_H
#define PARSER_H

#include "treeelement.h"
#include "Lekser.h"
class Parser
{

private:
    TreeElement* root = new TreeElement(rootType);
    TreeElement* current = root;
    Lekser lekser;
    Token token;

    void errorOccured();
    void errorOccured(std::string message);
    void errorOccuredNoTakeToken(std::string message);

    bool getFunction();
    void consumeToken(types type, bool addingChild);
    void consumeToken(types type, int val, bool addingChild);
    void consumeToken(types type, double val, bool addingChild);
    void consumeToken(types type, std::string val, bool addingChild);
    void getParamsDefinitions();
    void getCode();
    bool getParam();
    bool getLine();
    void getDefinition();
    void getRValue();
    void getValue();
    void getFunctionCall();
    void insertBetween(TreeElement* parent, TreeElement* between);
    void getIdentifierFirst();
    void getKeyWord();
    void getExpression(bool firstValueAlreadyTaken);
    void getBracketExpression();

public:
    Parser();
    TreeElement* parse();
};

#endif // PARSER_H
