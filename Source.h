#ifndef SOURCE_H
#define SOURCE_H

#include <fstream>
class Source
{
    public:
        Source();
        virtual ~Source();
        char getChar();
        char consumeChar();
        int getLine();
        int getColumn();
        int getPositionInFile();

    private:
        char ch;
        int line = 1;
        int column = 0;
        int positionInFile = 0;
        std::ifstream file;
};

#endif // SOURCE_H
