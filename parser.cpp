#include "parser.h"
#include <iostream>
Parser::Parser()
{

}

TreeElement* Parser::parse(){
    token = lekser.getToken();
    while(getFunction());
    return root;
}

bool Parser::getFunction(){
    if (token.type == null)
        return false;
    if (token.type == buildInType){
        TreeElement* pom = new TreeElement(functionType);
        current->setAsChild(pom);
        current = pom;
        consumeToken(buildInType, true);
        consumeToken(identifier, true);
        consumeToken(nawias, "(", false);
        getParamsDefinitions();
        consumeToken(nawias, "{", false);
        getCode();
        current = current->parent;
    }
    else {
        errorOccured("niepoprawna definicja funkcji");
    }
    return true;
}

void Parser::getParamsDefinitions(){
    while(getParam());
}

bool Parser::getParam(){
    if (token.str == ")"){
        token = lekser.getToken();
        return false;
    }
    if (token.type == buildInType){
        TreeElement* pom = new TreeElement(paramType);
        current->setAsChild(pom);
        current = pom;
        pom = new TreeElement(token);
        current->setAsChild(pom);
    }
    else {
        errorOccured("spodziewany typ");
    }
    token = lekser.getToken();
    consumeToken(identifier,true);
    current = current->parent;
    if (token.str == ")"){
        token = lekser.getToken();
        return false;
    }
    else
        if (token.type == comma){
            token = lekser.getToken();
            return true;
        }
    token = lekser.getToken();
    errorOccured("niezidentyfikowany obiekt");
    return true;
}

void Parser::consumeToken(types type, bool addingChild){
    if (token.type == type){
        if (addingChild){
            TreeElement* pom = new TreeElement(token);
            current->setAsChild(pom);
        }
    }
    else {
        errorOccured("spodziewany token innego typu");
    }
    token = lekser.getToken();
}

void Parser::consumeToken(types type, int val, bool addingChild){
    if (token.type == type && token.number == val){
        if (addingChild){
            TreeElement* pom = new TreeElement(token);
            current->setAsChild(pom);
        }
    }
    else {
        errorOccured("spodziewany token innego typu");
    }
    token = lekser.getToken();
}

void Parser::consumeToken(types type, double val, bool addingChild){
    if (token.type == type && token.doub - val < 0.000001 && token.doub - val > -0.000001){
        if (addingChild){
            TreeElement* pom = new TreeElement(token);
            current->setAsChild(pom);
        }
    }
    else {
        errorOccured("spodziewany token innego typu");
    }
    token = lekser.getToken();
}

void Parser::consumeToken(types type, std::string val, bool addingChild){
    if (token.type == type && token.str == val){
        if (addingChild){
            TreeElement* pom = new TreeElement(token);
            current->setAsChild(pom);
        }
    }
    else {
        errorOccured("spodziewany token innego typu");
    }
    token = lekser.getToken();
}

void Parser::getCode(){
    if (token.str == "}"){
        token = lekser.getToken();
        return;
    }
    TreeElement* pom = new TreeElement(codeType);
    current->setAsChild(pom);
    current = pom;
    while(getLine());
    current = current->parent;
}

bool Parser::getLine(){ // zwraca czy pobierac nastepna linie (falsz gdy poojawi sie nawias domykający '}' )
    if (token.str=="}"){
        token = lekser.getToken();
        return false;
    }
    switch (token.type) {
        case null: errorOccured("Niespodziewane zakonczenie kodu"); return false;
        case undefined: errorOccured("niezidentyfikowany token"); return true;
        case endLine: token = lekser.getToken(); return true;
        case buildInType: getDefinition(); break;
        case identifier: getIdentifierFirst(); break;
        case keyWord: getKeyWord(); return true;
        default: errorOccured("niepoprawny poczatek linii");
    }
    if (token.type != endLine){
        errorOccuredNoTakeToken("spodziewany znak ';'");
    }
    return true;
}

void Parser::getKeyWord(){
    TreeElement* pom = new TreeElement(token);
    current->setAsChild(pom);
    current = pom;
    bool isElse = token.str == "else";
    token = lekser.getToken();
    if (!isElse){
        if (token.str == "(")
            token = lekser.getToken();
        else
            errorOccured("brak nawiasu za key word "+token.str);
        getExpression(false);

        if (token.str == ")")
            token = lekser.getToken();
        else
            errorOccured("brak nawiasu zamykajacego za key word "+token.str);
    }
    if (token.str == "{")
        token = lekser.getToken();
    else
        if (token.type == endLine){
            token = lekser.getToken();
            current = current->parent;
            return;
        }
        else {
            errorOccured("niepoprawny znak za wyrazeniem \"" + token.str + "\" spodziewany '{' lub ';'");
            current = current->parent;
            return;
        }
    getCode();

    current = current->parent;
}


void Parser::getExpression(bool firstValueAlreadyTaken){
    TreeElement* pom = new TreeElement(expressionType);
    if (firstValueAlreadyTaken){
        insertBetween(current, pom);
        current = pom;
        TreeElement* pom = new TreeElement(token);
        current->setAsChild(pom);
        token = lekser.getToken();
    }
    else {
        current->setAsChild(pom);
    }
    current = pom;
    bool stop;
    do{
        if (token.str == "("){
            getBracketExpression();
        }
        else {
            getValue();
        }
        if (token.type == boolFunction || token.type == arithmeticFunction){
            TreeElement* pom = new TreeElement(token);
            current->setAsChild(pom);
            token = lekser.getToken();
            stop = false;
        }
        else{
            stop = true;
        }

    }while(!stop);

    current = current->parent;
}

void Parser::getBracketExpression(){
    TreeElement* pom = new TreeElement(bracketExpressionType);
    current->setAsChild(pom);
    current = pom;
    token = lekser.getToken();
                bool stop;
                do{
                    if (token.str == "("){
                        getBracketExpression();
                    }
                    else {
                        getValue();
                    }
                    if (token.type == boolFunction || token.type == arithmeticFunction){
                        TreeElement* pom = new TreeElement(token);
                        current->setAsChild(pom);
                        token = lekser.getToken();
                        stop = false;
                    }
                    else{
                        if (token.str == ")"){
                            token = lekser.getToken();
                            stop = true;
                        }
                        else {
                            errorOccuredNoTakeToken("niezidentyfikowana wartosc w wyrazeniu");
                            stop = false;
                        }
                    }

                }while(!stop);
    current = current->parent;
}

void Parser::getIdentifierFirst(){
    consumeToken(identifier, true);
    if (token.str == "("){
        getFunctionCall();
        return;
    }
    if (token.type == dot){
        token = lekser.getToken();
        if (token.type == identifier){
            current = current->children[current->children.size() - 1];
            consumeToken(identifier, true);
            getFunctionCall();
            current = current->parent;
        }
        else {
            errorOccured("spodziewane wywolanie metody");
        }
        return;
    }
    if (token.type == assignment){
        TreeElement* pom = new TreeElement(token);
        insertBetween(current, pom);
        token = lekser.getToken();
        current = pom;
        getRValue();
        current = current->parent;
        return;
    }
}

void Parser::getDefinition(){
    TreeElement* pom = new TreeElement(definitionType);
    current->setAsChild(pom);
    current = pom;
    consumeToken(buildInType, true);
    consumeToken(identifier, true);
    if (token.type == assignment){
        token = lekser.getToken();
        getRValue();
    }
    current = current->parent;
}

void Parser::getRValue(){
    if (token.str == "(")
        getBracketExpression();
    else {
        getValue();
    }
    if (token.type == boolFunction || token.type == arithmeticFunction){
        getExpression(true);
    }
}

void Parser::getValue(){ // pojedyncza wartosc ktora moze byc przypisana (np a; 120.34; f(f1(a)), ale juz nie a+b ani a>b (to expression))
    TreeElement* pom = new TreeElement(ValueType);
    current->setAsChild(pom);
    current = pom;
    switch (token.type) {
        case intNumber: consumeToken(intNumber, true); break;
        case doubleNumber: consumeToken(doubleNumber, true); break;
        case stringText: consumeToken(stringText, true); break;
        case identifier: getIdentifierFirst(); break;
        default: errorOccured("niepoprawna wartosc");
    }
    current = current->parent;
}


void Parser::getFunctionCall(){
    TreeElement* pom = new TreeElement(functionCallType);
    insertBetween(current, pom);
    current = pom;
    token = lekser.getToken();
    if (token.str == ")"){
        token = lekser.getToken();
        current = current->parent;
        return;
    }
    bool stop = true;
    do {
        getRValue();
        if (token.type == comma){
          token = lekser.getToken();
          stop = false;
        }
        else {
            stop = true;
        }
    } while(!stop);

    if (token.str == ")"){
        token = lekser.getToken();
    }
    else{
        errorOccured("spodziewany znak ')'");
    }
    current = current->parent;
}

void Parser::insertBetween(TreeElement* parent, TreeElement* between){ // wstawia element pomiędzy parenta a jego ostatnie dziecko
    (parent->children[parent->children.size()-1])->parent = between;
    between->setAsChild((current->children[current->children.size()-1]));
    current->children[current->children.size()-1] = between;
    between->parent = current;
}

void Parser::errorOccured(){
    std::cout<<"ERROR at line: "<<lekser.getLine()<<", column: "<<lekser.getColumn()<<std::endl;
    token = lekser.getToken();
}

void Parser::errorOccured(std::string message){
    std::cout<<"ERROR at line: "<<lekser.getLine()<<", column: "<<lekser.getColumn()<<": "<<message<<std::endl;
    token = lekser.getToken();
}

void Parser::errorOccuredNoTakeToken(std::string message){
    std::cout<<"ERROR at line: "<<lekser.getLine()<<", column: "<<lekser.getColumn()<<": "<<message<<std::endl;
}
