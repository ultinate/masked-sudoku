#include <bitset>
#include "../sTest/test.h"
#include "../parser.hpp"


/* Parser unit tests */
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

/** 
 * Run all unit tests 
 * 
 * based on light-weight sTest framework, 
 * https://github.com/greg-white/sTest 
 */ 
int runUnitTests() 
{ 
    try 
    { 
        TEST_SECTION("types");
        TEST_GROUP("mask");
        mask m;
        m = 0x0;
        TEST(m == 0);

        TEST_GROUP("slice");
        mask *slice = new mask[N];
        slice[0] = m;
        slice[1] = 0x2;
        slice[9] = 0x1ff;
        TEST(slice[1] == 0x2);

        TEST_GROUP("board");
        mask *board = new mask[N*N];
        board[0] = m;
        board[1] = 0x3;
        TEST(board[0] == 0x0);
        TEST(board[1] == 0x3);


        TEST_SECTION("parser");
 
        TEST_GROUP("inputCharToInt"); 
        test_inputCharToInt(); 
        TEST_GROUP("inputCharToMask"); 
        test_inputCharToMask(); 
        TEST_GROUP("getIntFromMask"); 
        test_getIntFromMask();
        TEST_GROUP("isOnlyOneBit"); 
        test_isOnlyOneBit();
        TEST_GROUP("countBits"); 
        test_countBits();
        TEST_GROUP("log2"); 
        test_log2();
        TEST_GROUP("getMaskFromInt"); 
        test_getMaskFromInt();


        TEST_GROUP("hex and binary representation"); 
        TEST(0x1 == 1); 
        TEST(0x1 == 0b1); 
        TEST((0x1 << 1) == 2); 
        TEST(0x02 == 2); 
        TEST(0b1 == 1); 
        TEST((0b1 << 1) == 2); 
        TEST(0b10 == 2);
        TEST(0x1ff == 0b111111111);
        
        // overflow of uint16 (= unsigned short int on this system)
        m = 0xffffffff;
        TEST(m == 0xffff);

        m = 0xffff;
        TEST(m == 0xffff);
        m = 0x0;
        TEST (m == 0x0);
        TEST (m == 0b0000000000000000);

        TEST_SUMMARY; 
    } 
    catch (...) 
    { 
        TEST_EXCEPTION; 
    } 
    return 0; 
} 

int main(int argc, char **argv) {
    std::cout << "running unit tests ... ";
    int areUnitTestsPassed = runUnitTests();
    return areUnitTestsPassed;
}
