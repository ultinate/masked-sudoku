#include "parser.hpp"


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

unsigned int Parser::countBits(mask m) {
    int counter = 0;
    for (int i = 0; i < N; i++) {
        if ((m & (0x1 << i)) > 0) {
            counter++;
        }
    }
    return counter;
}

unsigned int Parser::log2(mask m) {
    for (int i=0; i < N; i++) {
        if (m & (0x1 << i)) {
            return i + 1;
        }
    }
    return 0;
}

Parser::Parser(std::string inputString) {
    this->inputString = inputString;
    this->unsolvedBoard = new mask[N*N];
}

bool Parser::isBitSet(mask m, unsigned int i) {
    return m & (0x1 << (i - 1));
}

mask Parser::getMaskFromInt(unsigned int i) {
    return 0x1 << (i - 1);
}

mask Parser::getNotMask(unsigned int i) {
    return ~(getMaskFromInt(i));
}

int Parser::parse() {
    if (this->inputString.length() != N * N) {
        std::cout << 
            "Input string has wrong length (" <<
            this->inputString.length() << " instead of " << 
            N*N << ")" << std::endl;
        return -1;
    }
    for (int i = 0; i < N * N; i++) {
        char c = this->inputString[i];
        unsolvedBoard[i] = inputCharToMask(c);
    }
    return 0;
}

