#include "citymap.h"
#include "double.h"
#include "function.h"
#include "int.h"
#include "interpreter.h"
#include "parser.h"
#include "string.h"
#include <iostream>
Interpreter::Interpreter()
{
    variable_types["void"] = 0;
    variable_types["int"] = 1;
    variable_types["double"] = 2;
    variable_types["cityMap"] = 3;
    variable_types["street"] = 4;
    variable_types["string"] = 5;
}

CityMap Interpreter::run(){
    Parser parser;
    root = parser.parse();

    //tworzy wszystkie funkcje
    setFunctions();
    size_t main_index = findFunctionByName("main");

    if (main_index >= root->variables.size()){
        error("No main fuction found");
        return CityMap();
    }
    TreeElement* mainFunc = root->children[main_index];
    if (((Function*)root->variables[main_index])->getReturnType() != "cityMap"){
        error("main function has to return cityMap type");
    }

    CityMap* cm = (CityMap*)handleFunctionCode(mainFunc, std::vector<Variable*>());
    CityMap toRet = *cm;
    delete cm;
    return toRet;
}

size_t Interpreter::findFunctionByName(std::string name){
    for (size_t index = 0; index < root->children.size(); ++index)
        if (root->variables[index]->getName() == name)
            return index;
    return root->variables.size();
}

long Interpreter::findVariableByName(std::vector<Variable*> vec, std::string name){
    for(long i = 0; i < vec.size(); ++i){
        if (vec[i]->getName() == name)
            return i;
    }
    return -1;
}

Variable* Interpreter::findVariableRecursivelyByName(TreeElement* currFun, std::string name){
    while(currFun != root){
        long id = findVariableByName(currFun->variables, name);
        if (id >=0){ // znaleziono
            return currFun->variables[id];
        }
        else {
            currFun = currFun->parent;
        }
    }
    return nullptr;
}

void Interpreter::setFunctions(){
    for (auto i : root->children){
        Function* fun = new Function();
        fun->setType("function");
        fun->setReturnType(i->children[0]->token.str);
        fun->setName(i->children[1]->token.str);
        for (size_t ind = 2;ind < i->children.size()-1;++ind) {
            fun->parametersTypes.push_back(i->children[ind]->children[0]->token.str);
            fun->parametersNames.push_back(i->children[ind]->children[1]->token.str);
        }
        root->variables.push_back(fun);
    }
}
//remember to deallocate return value after use
Variable* Interpreter::handleFunctionCode(TreeElement* currFunc, std::vector<Variable*> parameters){
    for(auto i : currFunc->variables){
        delete i;
    }
    currFunc->variables.clear();
    for(auto i : parameters){
        currFunc->variables.push_back(i);
    }

    bool returnAppear = false;
    Variable *toReturn;
    //for(auto i : currFunc->children[currFunc->children.size()-1]->children){
    for(size_t ind=0; ind < currFunc->children[currFunc->children.size()-1]->children.size(); ++ind){
        TreeElement* i = currFunc->children[currFunc->children.size()-1]->children[ind];
        if (returnAppear)
                return toReturn;
        switch(i->type) {
        case definitionType:
            handleDefinition(currFunc, i);
            break;
        case functionCallType:
            handleFunCal(currFunc, i, "void");
            break;
        case tokenType:
            switch(i->token.type){
            case identifier:
                handleIndentifier(currFunc, i);
                break;
            case keyWord:
                toReturn = handleKeyWord(currFunc, &i, &returnAppear, &ind);
                break;
            case assignment:
                handleAssigment(currFunc, i);
                break;
            default:
                error("blednie rozpoczeta instrukcja1");
            }
        break;
        default:
            error("blednie rozpoczeta instrukcja2");
        }
    }
    return toReturn;
}
Variable* Interpreter::handleKeyWord(TreeElement* currFunc, TreeElement** i_reference, bool* returnAppear, size_t* index){
    TreeElement* i = *i_reference;
    if (i->token.str == "while"){
        while(getBoolRValue(currFunc, i->children[0])){
            handleFunctionCode(i, std::vector<Variable*>());
        }
        return nullptr;
    }
    if (i->token.str == "if"){
        if (getBoolRValue(currFunc, i->children[0])){
            handleFunctionCode(i, std::vector<Variable*>());
            if (i->parent->children.size() > getMyIndexInParentsChilden(i) + 1 &&
                    i->parent->children[getMyIndexInParentsChilden(i) + 1]->token.str == "else"){
                //zeby po powrocie do funkcji glownej nie patrzylo na elsa drugi raz
                //i_reference = &(i->parent->children[getMyIndexInParentsChilden(i) + 1]);
                (*index) = (*index) + 1;
            }
        }
        else{
            if (i->parent->children.size() > getMyIndexInParentsChilden(i) + 1 &&
                    i->parent->children[getMyIndexInParentsChilden(i) + 1]->token.str == "else"){
                handleFunctionCode(i->parent->children[getMyIndexInParentsChilden(i) + 1], std::vector<Variable*>());
                //zeby po powrocie do funkcji glownej nie patrzylo na elsa drugi raz
                //i_reference = &(i->parent->children[getMyIndexInParentsChilden(i) + 1]);
                (*index) = (*index) + 1;
            }
        }
        return nullptr;
    }
    if (i->token.str == "else"){
        error("else without if");
        return nullptr;
    }
    if (i->token.str == "return"){
        *returnAppear = true;
        if (currFunc->children[0]->token.str == "void")
            return nullptr;
        Variable *var;
        switch(variable_types[currFunc->children[0]->token.str]){
        case 1:
            var = new Int();
            ((Int*)var)->setValue(getIntRValue(currFunc, i->children[0]));
            break;
        case 2:
            var = new Double();
            ((Double*)var)->setValue(getDoubleRValue(currFunc, i->children[0]));
            break;

        case 3:
            var = new CityMap();
            ((CityMap*)var)->setValue(getCityMapRValue(currFunc, i->children[0]));
            break;

        case 4:
            var = new Street();
            ((Street*)var)->setValue(getStreetRValue(currFunc, i->children[0]));
            break;

        case 5:
            var = new String();
            ((String*)var)->setValue(getStringRValue(currFunc, i->children[0]));
            break;
        default:
            error("invalid type name");
        }
        return var;
    }
    error("no keyword found");
}

void Interpreter::handleIndentifier(TreeElement* currFunc, TreeElement* i){
    Variable* var = findVariableRecursivelyByName(currFunc, i->token.str);
    if (var == nullptr){
        error("No variable " + i->token.str + "found");
        return;
    }
    if(i->children.size() > 0){
        if (i->children[0]->type == functionCallType){
            switch(variable_types[var->getType()]){
            case 3://citymap
                handleCityMapBuildInMethod(currFunc, i->children[0], var);
                break;
            case 4://street
                handleStreetBuildInMethod(currFunc, i->children[0], var);
                break;
            default:
                error("method call on incorrect type variable");
            }

        }
        else {
            error("incorrect variable use");
        }
    }
}

void Interpreter::handleAssigment(TreeElement* currFunc, TreeElement* i){
    Variable* var = findVariableRecursivelyByName(currFunc, i->children[0]->token.str);
    if (var == nullptr){
        error("no variable "+i->children[0]->token.str+" found");
        return;
    }
    switch(variable_types[var->getType()]){
    case 1:
        ((Int*)var)->setValue(getIntRValue(currFunc, i->children[1]));
        break;
    case 2:
        ((Double*)var)->setValue(getDoubleRValue(currFunc, i->children[1]));
        break;

    case 3:
        ((CityMap*)var)->setValue(getCityMapRValue(currFunc, i->children[1]));
        break;

    case 4:
        ((Street*)var)->setValue(getStreetRValue(currFunc, i->children[1]));
        break;

    case 5:
        ((String*)var)->setValue(getStringRValue(currFunc, i->children[1]));
        break;
    default:
        error("invalid type name");
    }
}

Variable* Interpreter::handleFunCal(TreeElement* currFunc, TreeElement* i, std::string expectedType){
    size_t fun_id = findFunctionByName(i->children[0]->token.str);
    if (fun_id >= root->variables.size()){
        error("no function " + i->children[0]->token.str + "found");
        return nullptr;
    }
    if(((Function*)root->variables[fun_id])->getReturnType() != expectedType){
        error("functions " + root->variables[fun_id]->getName() + " return type is difrent than " + expectedType);
        return nullptr;
    }
    if (((Function*)root->variables[fun_id])->getParametersTypes().size() !=  i->children.size()-1){
        error("wrong parameters number in function call " + root->variables[fun_id]->getName());
        return nullptr;
    }
    std::vector<Variable*> parameters;
    for (size_t ind = 1; ind < i->children.size(); ++ind){
        bool defined = true;
        Variable* var;

        switch(variable_types[((Function*)root->variables[fun_id])->getParametersTypes()[ind-1]]){
        case 1:
            var = new Int();
            getIntRValue(currFunc, i->children[ind]);
            ((Int*)var)->setValue(getIntRValue(currFunc, i->children[ind]));
            break;
        case 2:
            var = new Double();
            ((Double*)var)->setValue(getDoubleRValue(currFunc, i->children[ind]));
            break;

        case 3:
            var = new CityMap();
            ((CityMap*)var)->setValue(getCityMapRValue(currFunc, i->children[ind]));
            break;

        case 4:
            var = new Street();
            ((Street*)var)->setValue(getStreetRValue(currFunc, i->children[ind]));
            break;

        case 5:
            var = new String();
            ((String*)var)->setValue(getStringRValue(currFunc, i->children[ind]));
            break;
        default:
            error("invalid type name");
            defined = false;
        }
        if (defined){
            var->setType(((Function*)root->variables[fun_id])->getParametersTypes()[ind-1]);
            var->setName(((Function*)root->variables[fun_id])->getParametersNames()[ind-1]);
            parameters.push_back(var);
        }
        else{
            delete var;
        }
    }
    return handleFunctionCode(root->children[fun_id], parameters);
}

void Interpreter::handleDefinition(TreeElement* currFunc, TreeElement* i){
    if (findVariableByName(currFunc->variables, i->children[1]->token.str) > 0){
        error("variable "+i->children[1]->token.str+" already exist");
        return;
    }
    Variable* var;
    bool defined = true;
    switch(variable_types[i->children[0]->token.str]){
    case 1:
        var = new Int();
        if (i->children.size() > 2){
            ((Int*)var)->setValue(getIntRValue(currFunc, i->children[2]));
        }
        break;
    case 2:
        var = new Double();
        if (i->children.size() > 2){
            ((Double*)var)->setValue(getDoubleRValue(currFunc, i->children[2]));
        }
        break;

    case 3:
        var = new CityMap();
        if (i->children.size() > 2){
            ((CityMap*)var)->setValue(getCityMapRValue(currFunc, i->children[2]));
        }
        break;

    case 4:
        var = new Street();
        if (i->children.size() > 2){
            ((Street*)var)->setValue(getStreetRValue(currFunc, i->children[2]));
        }
        break;

    case 5:
        var = new String();
        if (i->children.size() > 2){
            ((String*)var)->setValue(getStringRValue(currFunc, i->children[2]));
        }
        break;
    default:
        defined = false;
        error("invalid type name");
    }
    if (defined){
        var->setType(i->children[0]->token.str);
        var->setName(i->children[1]->token.str);
        currFunc->variables.push_back(var);
    }
}

void Interpreter::handleCityMapBuildInMethod(TreeElement* currFunc, TreeElement* i, Variable* var){
    if (i->children[0]->token.type == identifier){
        if (i->children[0]->token.str == "setSize"){
            if (i->children.size() != 3){
                error("incorrect number of setSize arguments, needed 2");
                return;
            }
            ((CityMap*)var) ->setWidth(getDoubleRValue(currFunc, i->children[1]));
            ((CityMap*)var) ->setHeight(getDoubleRValue(currFunc, i->children[2]));
            return;
        }
        if (i->children[0]->token.str == "add"){
            if (i->children.size() != 2){
                error("incorrect number of add (on cityMap) arguments, needed 2");
                return;
            }
            ((CityMap*)var) ->addStreet(getStreetRValue(currFunc, i->children[1]));
            return;
        }
        error("unnokwn metod call");
    }
}

void Interpreter::handleStreetBuildInMethod(TreeElement* currFunc, TreeElement* i, Variable* var){
    if (i->children[0]->token.type == identifier){
        if (i->children[0]->token.str == "setX"){
            if (i->children.size() != 2){
                error("incorrect number of " + i->children[0]->token.str + " arguments, needed 1");
                return;
            }
            ((Street*)var)->setX( getDoubleRValue(currFunc, i->children[1]));
            return;
        }
        if (i->children[0]->token.str == "setY"){
            if (i->children.size() != 2){
                error("incorrect number of " + i->children[0]->token.str + " arguments, needed 1");
                return;
            }
            ((Street*)var)->setY( getDoubleRValue(currFunc, i->children[1]));
            return;
        }
        if (i->children[0]->token.str == "setLength"){
            if (i->children.size() != 2){
                error("incorrect number of " + i->children[0]->token.str + " arguments, needed 1");
                return;
            }
            ((Street*)var)->setLength( getDoubleRValue(currFunc, i->children[1]));
            return;
        }
        if (i->children[0]->token.str == "setWidth"){
            if (i->children.size() != 2){
                error("incorrect number of " + i->children[0]->token.str + " arguments, needed 1");
                return;
            }
            ((Street*)var)->setWidth( getDoubleRValue(currFunc, i->children[1]));
            return;
        }
        if (i->children[0]->token.str == "setRotation"){
            if (i->children.size() != 2){
                error("incorrect number of " + i->children[0]->token.str + " arguments, needed 1");
                return;
            }
            ((Street*)var)->setRotation( getDoubleRValue(currFunc, i->children[1]));
            return;
        }
        error("unnokwn metod call");
    }
}

Street Interpreter::getStreetRValue(TreeElement *currFunc, TreeElement* i){
    Street undefinde;
    if (i->type == treeElementTypes::ValueType){
        if(i->children[0]->type == treeElementTypes::functionCallType){
            Street* returned = ((Street*)handleFunCal(currFunc, i->children[0], "street"));
            Street toRet = *returned;
            delete returned;
            return toRet;
        }
        if(i->children[0]->token.type == types::identifier){
            Variable* var = findVariableRecursivelyByName(currFunc, i->children[0]->token.str);
            if (var->getType() != "street"){
                error("variable " + var->getName() + "is not of type street");
                return undefinde;
            }
            return *((Street*)var);
        }

    return undefinde;
    }
}

CityMap Interpreter::getCityMapRValue(TreeElement *currFunc, TreeElement* i){
    CityMap undefinde;
    if (i->type == treeElementTypes::ValueType){
        if(i->children[0]->type == treeElementTypes::functionCallType){
            CityMap* returned = ((CityMap*)handleFunCal(currFunc, i, "cityMap"));
            CityMap toRet = *returned;
            delete returned;
            return toRet;
        }
        if(i->children[0]->token.type == types::identifier){
            Variable* var = findVariableRecursivelyByName(currFunc, i->children[0]->token.str);
            if (var->getType() != "cityMap"){
                error("variable " + var->getName() + "is not of type cityMap");
                return undefinde;
            }
            CityMap toRet = *((CityMap*)var);
            return toRet;
        }
    }
    return undefinde;
}

int Interpreter::getIntRValue(TreeElement *currFunc,TreeElement* i){
    double ret = getNumericRValue(currFunc, i);
    return (int)ret;
}

double Interpreter::getDoubleRValue(TreeElement *currFunc, TreeElement* i){
    return getNumericRValue(currFunc, i);
}

double Interpreter::getNumericRValue(TreeElement *currFunc, TreeElement* i){
    double undefinde;
    if (i->type == treeElementTypes::ValueType){
        if(i->children[0]->type == treeElementTypes::functionCallType){
            std::string fName = i->children[0]->children[0]->token.str;
            size_t fun_id = findFunctionByName(fName);
            if (fun_id >= root->variables.size()){
                error("no function " + i->children[0]->token.str + "found");
                return undefinde;
            }
            if(((Function*)root->variables[fun_id])->getReturnType() == "double"){
                Double* returned = ((Double*)handleFunCal(currFunc, i->children[0], "double"));
                //Double* returned = ((Double*)handleFunCal(currFunc, i, "double"));
                double toRet = returned->getValue();
                delete returned;
                return toRet;
            }
            if(((Function*)root->variables[fun_id])->getReturnType() == "int"){
                Int* returned = ((Int*)handleFunCal(currFunc, i->children[0], "int"));
                //Int* returned = ((Int*)handleFunCal(currFunc, i, "int"));
                double toRet = returned->getValue();
                delete returned;
                return toRet;
            }
            return undefinde;
        }
        if(i->children[0]->token.type == types::identifier){
            Variable* var = findVariableRecursivelyByName(currFunc, i->children[0]->token.str);
            if (var->getType() != "double" && var->getType() != "int"){
                error("variable " + var->getName() + "is not of type int or double");
                return undefinde;
            }
            if (var->getType() == "double")
                return ((Double*)var)->getValue();
            if (var->getType() == "int"){
                return (double)((Int*)var)->getValue();
            }
        }
        if(i->children[0]->token.type == types::doubleNumber){
            return i->children[0]->token.doub;
        }
        if(i->children[0]->token.type == types::intNumber){
            return i->children[0]->token.number;
        }
    }
    if (i->type == treeElementTypes::expressionType || i->type == treeElementTypes::bracketExpressionType){
        if (i->children[1]->token.type != types::arithmeticFunction){
            error("arithmetic token not found");
            return undefinde;
        }
        double resTillNow;
        double a = getNumericRValue(currFunc, i->children[0]);
        double b = getNumericRValue(currFunc, i->children[2]);
        if (i->children[1]->token.str == "+"){
            resTillNow = a+b;
        }
        if (i->children[1]->token.str == "-"){
            resTillNow = a-b;
        }
        if (i->children[1]->token.str == "*"){
            resTillNow = a*b;
        }
        if (i->children[1]->token.str == "/"){
            resTillNow = a/b;
        }
        for (size_t ind = 3; ind < i->children.size(); ind+=2){
            if (i->children[ind]->token.type != types::arithmeticFunction){
                error("arithmetic token not found");
                return undefinde;
            }
            double c = getNumericRValue(currFunc, i->children[ind+1]);
            if (i->children[ind]->token.str == "+"){
                resTillNow = resTillNow+c;
            }
            if (i->children[ind]->token.str == "-"){
                resTillNow = resTillNow-c;
            }
            if (i->children[ind]->token.str == "*"){
                resTillNow = resTillNow*c;
            }
            if (i->children[ind]->token.str == "/"){
                resTillNow = resTillNow/c;
            }
        }
        return resTillNow;
    }
    return undefinde;
}

std::string Interpreter::getStringRValue(TreeElement *currFunc, TreeElement* i){
    return i->children[0]->token.str;
}

bool Interpreter::getBoolRValue(TreeElement *currFunc, TreeElement* i){
    if (i->type == treeElementTypes::expressionType){
        if (i->children[1]->token.type != types::boolFunction){
            error("bool token not found");
            return false;
        }

        double a = getNumericRValue(currFunc, i->children[0]);
        double b = getNumericRValue(currFunc, i->children[2]);
        if (i->children[1]->token.str == ">"){
            return a > b;
        }
        if (i->children[1]->token.str == "<"){
            return a < b;
        }
        if (i->children[1]->token.str == ">="){
            return a >= b;
        }
        if (i->children[1]->token.str == "<="){
            return a <= b;
        }
        if (i->children[1]->token.str == "=="){
            return a == b;
        }
        if (i->children[1]->token.str == "!="){
            return a != b;
        }
    }
}

void Interpreter::error(std::string message){
    std::cout<<message<<std::endl;
}

size_t Interpreter::getMyIndexInParentsChilden(TreeElement* i){
    for (size_t index = 0; i->parent->children.size(); ++index){
        if (i->parent->children[index] == i)
            return index;
    }
    return i->parent->children.size();
}
