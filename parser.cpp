#include "parser.hpp"

/**
 * Parser methods
 */
unsigned int Parser::inputCharToInt(char c) {
    if (c == '.')
        return 0;
    else
        return c - '0';
}

mask Parser::inputCharToMask(char c) {
    if (c == '.') {
        return 0b111111111;
    }
    else {
        unsigned int i = Parser::inputCharToInt(c);
        return Parser::getMaskFromInt(i);
    }
}

unsigned int Parser::getIntFromMask(mask m) {
    if (Parser::isOnlyOneBit(m))
        return Parser::log2(m);
    else
        return 0;
}

bool Parser::isOnlyOneBit(mask m) {
    return Parser::countBits(m) == 1;
}

int Parser::countBits(mask m) {
    int counter = 0;
    for (int i = 0; i < N; i++) {
        if ((m & (0x1 << i)) > 0) {
            counter++;
        }
    }
    return counter;
}

/**
 * Returns log2 of a number if it is a multiple of 2
 *
 * log2 expects mask to have only 1 bit set.
 * Otherwise, the position (log2) of the LSB of m is returned,
 * or 0 if no bits are set.
 */
unsigned int Parser::log2(mask m) {
    for (int i=0; i < N; i++) {
        if (m & (0x1 << i)) {
            return i + 1;
        }
    }
    return 0;
}

Parser::Parser(std::string s) {
    this->s = s;
    unsolvedBoard = new mask[N*N];
}

mask Parser::getMaskFromInt(unsigned int i) {
    return 0x1 << (i - 1);
}

int Parser::parse() {
    if (this->s.length() != N * N) {
        std::cout << "Input string has wrong length" << std::endl;
        return -1;
    }
    for (int i = 0; i < N * N; i++) {
        char c = this->s[i];
        unsolvedBoard[i] = inputCharToMask(c);
    }
    return 0;
}

