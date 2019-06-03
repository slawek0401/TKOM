#ifndef LEKSER_H
#define LEKSER_H

#include "Source.h"
#include "Token.h"
#include <vector>

class Lekser
{
    public:
        Lekser();
        virtual ~Lekser();
        Token getToken();
        int getLine();
        int getColumn();
        const std::vector<std::string> buildInTypes {"void", "int", "double", "cityMap", "street", "string"};
        const std::vector<std::string> keyWords {"while", "if", "else", "return"};
    private:
    Source source;

    bool isSmallLetter(char a);
    bool isBigLetter(char a);
    bool isLetter(char a);
    bool isNumber(char a);
    bool isBuildInType(std::string a);
    bool isKeyWord(std::string a);

};

#endif // LEKSER_H
