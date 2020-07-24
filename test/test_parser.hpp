#include <bitset>
#include "../sTest/test.h"
#include "../parser.hpp"
#include "../visualizer.hpp"


/* Parser unit tests */
void test_Parser() {
    std::string inputString = 
        "..37..4.6"
        "...3.5..."
        "92..6.8.."
        ".5...2..4"
        "..1...6.."
        "4..98..7."
        ".3..71..8"
        "..4...7.2"
        "..6...1.3";
    Parser *p = new Parser(inputString);
    int parseResult = p->parse();
    TEST(parseResult == 0);
    mask *board = p->unsolvedBoard;
    std::string expected = inputString;
    TEST(expected == Visualizer::printBoardMini(board));
}

void test_inputCharToInt() {
    int result;
    result = Parser::inputCharToInt('.');
    TEST(result == 0);
    result = Parser::inputCharToInt('1');
    TEST(result == 1);
    result = Parser::inputCharToInt('9');
    TEST(result == 9);
}

void test_inputCharToMask() {
    mask result;
    result = Parser::inputCharToMask('.');
    TEST(result == 0b111111111);
    TEST(result == 0x1ff);
    result = Parser::inputCharToMask('1');
    TEST(result == 0b1);
    TEST(result == 0x1);
    result = Parser::inputCharToMask('2');
    TEST(result == 0b10);
    TEST(result == 0x2);
    result = Parser::inputCharToMask('9');
    TEST(result == 0b100000000);
    TEST(result == 0x100);
}

void test_getIntFromMask() {
    int result;
    // known numbers (single bit set)
    result = Parser::getIntFromMask(0x1);
    TEST(result == 1);
    result = Parser::getIntFromMask(0b100000000);
    TEST(result == 9);
    // unknown numbers (multiple bits set)
    result = Parser::getIntFromMask(0b111111111);
    TEST(result == 0);
    result = Parser::getIntFromMask(0b111111111);
    TEST(result == 0);
    result = Parser::getIntFromMask(0b100110000);
    TEST(result == 0);
    result = Parser::getIntFromMask(0x3);
    TEST(result == 0);
    // edge case (should not occur, no bit set)
    result = Parser::getIntFromMask(0x0);
    TEST(result == 0);
}

void test_isOnlyOneBit() {
    TEST(true == Parser::isOnlyOneBit(0b000000001));
    TEST(true == Parser::isOnlyOneBit(0b100000000));
    TEST(false == Parser::isOnlyOneBit(0b000000000));
    TEST(false == Parser::isOnlyOneBit(0b101000000));
    TEST(false == Parser::isOnlyOneBit(0b111111111));
}

void test_countBits() {
    TEST(1 == Parser::countBits(0b000000001));
    TEST(1 == Parser::countBits(0b100000000));
    TEST(0 == Parser::countBits(0b000000000));
    TEST(2 == Parser::countBits(0b101000000));
    TEST(9 == Parser::countBits(0b111111111));
}

void test_log2() {
    // only test expected cases (single bit set)
    TEST(1 == Parser::log2(0b000000001));
    TEST(2 == Parser::log2(0b000000010));
    TEST(9 == Parser::log2(0b100000000));
}

void test_getMaskFromInt() {
    mask m = 0x1;
    TEST(m == Parser::getMaskFromInt(1));
    m = 0x2;
    TEST(m == Parser::getMaskFromInt(2));
    m = 0b10;
    TEST(m == Parser::getMaskFromInt(2));
    m = 0b100000000;
    TEST(m == Parser::getMaskFromInt(9));
}

void test_getNotMask() {
    mask result;
    result = Parser::getNotMask(1);
    // how to visualize binary values
    /*
    std::bitset<32> b(result);
    std::cout << b << std::endl;
    */
    TEST(result == 0b1111111111111110);
    result = Parser::getNotMask(2);
    TEST(result == 0b1111111111111101);
    result = Parser::getNotMask(9);
    TEST(result == 0b1111111011111111);
}

