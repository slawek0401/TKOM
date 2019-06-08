#include "Source.h"
#include <iostream>
Source::Source()
{
    file.open("C:\\Qt\\projects\\Inter\\testy\\zrodlo8", std::fstream::in);
    file >> std::noskipws >> ch;
}

Source::~Source()
{
    file.close();
}

char Source::getChar(){
    if (file.eof())
        return EOF;
    return ch;

}
char Source::consumeChar(){
    if (file.eof())
        return EOF;
    char oldCh = ch;

    file >> std::noskipws >> ch;

    if (ch == '\n'){
        column = 0;
        ++line;
    }
    else
        ++column;
    ++positionInFile;
    return oldCh;
}

int Source::getColumn(){
    return column;
}

int Source::getLine(){
    return line;
}

int Source::getPositionInFile(){
    return positionInFile;
}
