#include "Lekser.h"

Lekser::Lekser()
{
    //ctor
}

Lekser::~Lekser()
{
    //dtor
}

Token Lekser::getToken(){
    Token toRet;
    bool stop = false;

    while(std::isspace(source.getChar())){
        source.consumeChar();
    }
    while(!std::isspace(source.getChar()) && !stop){
        if (source.getChar() == EOF){
            toRet.type = null;
            return toRet;
        }
        switch(source.getChar()){
            case '(': case ')': case '{' : case '}':
                toRet.type = nawias;
                toRet.str = source.consumeChar();
                stop = true;
                break;
            case ';':
                toRet.type = endLine;
                toRet.str = source.consumeChar();
                stop = true;
                break;

            case '.':
                toRet.type = dot;
                toRet.str = source.consumeChar();
                stop = true;
                break;

            case ',':
                toRet.type = comma;
                toRet.str = source.consumeChar();
                stop = true;
                break;

            case '+': case '-': case '*': case '/':
                toRet.type = arithmeticFunction;
                toRet.str = source.consumeChar();
                if (source.getChar() == '=' || source.getChar() == '+' || source.getChar() == '-')
                    toRet.str += source.consumeChar();
                stop = true;
                break;

            case '<': case '>' :
                toRet.type = boolFunction;
                toRet.str = source.consumeChar();
                if (source.getChar() == '=')
                    toRet.str += source.consumeChar();
                stop = true;
                break;

            case '=':
                toRet.type = assignment;
                toRet.str = source.consumeChar();
                if (source.getChar() == '='){
                    toRet.type = boolFunction;
                    toRet.str += source.consumeChar();
                }
                stop = true;
                break;

            case '!':
                toRet.type = boolFunction;
                toRet.str = source.consumeChar();
                if (source.getChar() == '=')
                    toRet.str += source.consumeChar();
                else
                    toRet.type = undefined;
                stop = true;
                break;

            case '"':
                source.consumeChar();
                toRet.type = stringText;
                char prevChar;
                while(source.getChar() != '"' && source.getChar() != EOF){
                    prevChar = source.consumeChar();
                    if (prevChar == '\\' && source.getChar() == '"')
                        prevChar = source.consumeChar();
                    toRet.str += prevChar;
                }
                source.consumeChar();
                stop = true;
                break;

            default:
                if (isLetter(source.getChar())){
                    while (isLetter(source.getChar()) || isNumber(source.getChar())){
                        toRet.type = identifier;
                        toRet.str += source.consumeChar();
                        stop = true;
                    }
                    if (isBuildInType(toRet.str))
                        toRet.type = buildInType;
                    if (isKeyWord(toRet.str))
                        toRet.type = keyWord;
                }
                else{
                    if (isNumber(source.getChar())){
                        toRet.type = intNumber;
                        while (isNumber(source.getChar())){
                            toRet.number *= 10;
                            toRet.number += source.consumeChar() - '0';

                        }
                        if (source.getChar() == '.'){
                            toRet.type = doubleNumber;
                            toRet.doub = (double) toRet.number;
                            int i = 1;
                            source.consumeChar();
                            while (isNumber(source.getChar())){
                                toRet.doub += (double)(source.consumeChar() - '0')/(double)(i*10);
                                i *= 10;
                            }
                        }
                        stop = true;
                    }
                    else{
                        toRet.type = undefined;
                        toRet.str += source.consumeChar();
                    }
                }
        }
    }
    return toRet;
}

bool Lekser::isSmallLetter(char a) {
    return a >= 'a' && a <= 'z';
}

bool Lekser::isBigLetter(char a) {
    return a >= 'A' && a <= 'Z';
}

bool Lekser::isLetter(char a) {
    return isSmallLetter(a) || isBigLetter(a);
}


bool Lekser::isNumber(char a) {
    return a >= '0' && a <= '9';
}

int Lekser::getColumn(){
    return source.getColumn();
}

int Lekser::getLine(){
    return source.getLine();
}

bool Lekser::isKeyWord(std::string a){
    for (auto i : keyWords)
        if (a == i)
            return true;
    return false;
}

bool Lekser::isBuildInType(std::string a){
    for (auto i : buildInTypes)
        if (a == i)
            return true;
    return false;
}
