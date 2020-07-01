#include <iostream>
#include <string>
#include <cstring>

const int N = 9;
typedef unsigned short int mask;
typedef mask slice[N];
typedef mask board[N*N];


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

        board unsolved;

        Parser(std::string s);
        int parse();
        std::string getRaw() { return s; }
};

