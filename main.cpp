#include <iostream>
#include "parser.h"

using namespace std;

void printTreeElem(TreeElement* iter){

    switch (iter->type) {
        case rootType: cout<<"root"; break;
        case functionType: cout<<"function"; break;
        case codeType:cout<<"code"; break;
        case paramType:cout<<"parameter"; break;
        case definitionType:cout<<"definicja"; break;
        case ValueType:cout<<"Value"; break;
        case functionCallType:cout<<"Fun Call"; break;
        case expressionType: cout<<"Expression"; break;
        case bracketExpressionType: cout<<"Bracket Expression"; break;
    default: break;
    }
    if (iter->type == tokenType)
        switch ((iter->token).type) {
            case identifier: cout<<"ident: "<<(iter->token).str; break;
            case buildInType: cout<<"build in type: "<<(iter->token).str; break;
            case intNumber: cout<<"int const: "<<(iter->token).number; break;
            case doubleNumber: cout<<"double const: "<<(iter->token).doub; break;
            case stringText: cout<<"string const: "<<(iter->token).str; break;
            case keyWord: cout<<"keyword " + iter->token.str; break;
            case arithmeticFunction: cout<<"znak arytmetyczny " + iter->token.str; break;
            case boolFunction: cout<<"znak logiczny " + iter->token.str; break;
            case assignment: cout<<"przypisanie" + iter->token.str; break;
            default: cout<< "cos innego"; break;
        }
}

void printNode(int level, TreeElement* iter) {
    for (int i = 0; i < level; ++i)
        cout << "\t";
    printTreeElem(iter);
    cout << endl;
    for (auto i : iter->children)
        printNode(level + 1, i);
}

void printTree(TreeElement* root)
{
    cout<<endl;
    printNode(0, root);
}

int main(){
    //Wypisywanie po kolei tokenów
    /*Token token;
    Lekser lek;
    while (true){
        token = lek.getToken();
        if (token.type == null)
            break;
        cout<<"token: "<<token.type<<" ";
        if (token.type == intNumber)
            cout<<token.number;
        else
            if (token.type == undefined)
                cout<<"UNDEFINED at "<<lek.getLine()<<" line; "<<lek.getColumn()<<" column";
            else
                if (token.type == doubleNumber)
                    cout<<token.doub;
                else
                    cout<<token.str;
        cout<<endl;
    }*/
    //Wypisywanie drzewa stworzonego przez parser
    Parser parser;
    printTree(parser.parse());

    return 0;
}
