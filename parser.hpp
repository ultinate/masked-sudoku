#include <iostream>
#include <string>
#include <cstring>

const int N = 9;
typedef unsigned short int mask;


class Parser {
    private:
        std::string s;

    public:
        static unsigned int inputCharToInt(char c);
        static mask inputCharToMask(char c);
        static unsigned int getIntFromMask(mask m);
        static bool isOnlyOneBit(mask m);
        static int countBits(mask m);
        static unsigned int log2(mask m);
        static mask getMaskFromInt(unsigned int i);

        mask * unsolvedBoard;

        Parser(std::string s);
        int parse();
        std::string getRaw() { return s; }
};

