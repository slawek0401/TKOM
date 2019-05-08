#ifndef TOKEN_H
#define TOKEN_H
#include <string>
//          0     1          2       3        4    5      6           7            8          9             10            11                  12          13       14
enum types {null, undefined, nawias, endLine, dot, comma, identifier, buildInType, intNumber, doubleNumber, boolFunction, arithmeticFunction, assignment, keyWord, stringText};

class Token
{
    public:
        Token();
        virtual ~Token();

    types type;
    int number;
    double doub;
    std::string str;
};

#endif // TOKEN_H
