#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <cstring>


const int N = 9;
typedef unsigned short int mask;


class Parser {
    private:
        std::string inputString;
        mask * unsolvedBoard;

    public:
        /**
         * Returns integer if char is a digit, 0 otherwise
         */
        static unsigned int inputCharToInt(char c);

        static mask inputCharToMask(char c);

        /**
         * Returns 1-based integer from mask if only one bit is set, 0 otherwise
         */
        static unsigned int getIntFromMask(mask m);

        /**
         * Returns true if only one bit is set, false otherwise
         */
        static bool isOnlyOneBit(mask m);

        /**
         * Returns true if bit of 1-based integer _i_ is set, false otherwise
         */
        static bool isBitSet(mask m, unsigned int i);

        /**
         * Returns number of set bits in mask
         */
        static unsigned int countBits(mask m);

        /**
         * Returns log2 of a number if it is a multiple of 2
         *
         * log2 expects mask to have only 1 bit set.
         * Otherwise, the position (log2) of the LSB of m is returned,
         * or 0 if no bits are set.
         */
        static unsigned int log2(mask m);

        /**
         * Returns mask where a single bit for a 1-based integer is set
         */
        static mask getMaskFromInt(unsigned int i);

        /**
         * Returns mask where a single bit is not set
         *
         * Corresponds to bitwise_not(getMaskFromInt).
         */
        static mask getNotMask(unsigned int i);

        Parser(std::string inputString);
        ~Parser() {}
        int parse();
        std::string getRaw() { return inputString; }
        mask * getBoard() { return unsolvedBoard; }
};

#endif
