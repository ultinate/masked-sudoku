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
 * log2 expects mask to have only 1 bit set
 * otherwise, the position (log2) of the LSB of m is returned
 */
unsigned int Parser::log2(mask m) {
    for (int i=0; i < N; i++) {
        if (m & (0x1 << i)) {
            return i + 1;
        }
    }
}

Parser::Parser(std::string s) {
    this->s = s;
}

mask Parser::getMaskFromInt(unsigned int i) {
    return 0x1 << (i - 1);
}

int Parser::parse() {
    std::cout << "Parsing this:" << this->s << std::endl;
    if (this->s.length() != N * N) {
        std::cout << "Input string has wrong length" << std::endl;
        return -1;
    }
    for (int i = 0; i < N * N; i++) {
        //std::string c = this->s.substr(i, 1);
        //unsolved[i] = inputCharToInt(c);
        char c = this->s[i];
        unsolved[i] = inputCharToMask(c);
    }
    return 0;
}

