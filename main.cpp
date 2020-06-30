#include "sTest/test.h"
#include <iostream>
#include <string>
#include <cstring>

const int N = 9;
typedef unsigned long int uint16;
typedef uint16 slice[N];
typedef uint16 board[N*N];

class SlicerInterface {
    public:
        virtual ~SlicerInterface();
        virtual slice& getSlice(board&);
};

class ReducerInterface {
    public:
        virtual bool reduceSlice(slice&) = 0;
};

class Reducer
    : public ReducerInterface {
    public:
        Reducer() {}
        ~Reducer() {}
        bool reduceSlice(slice& inSlice) {
            return true;
        }
};

class Parser {
    std::string s;

    static uint16 inputCharToInt(std::string c) {
        if ("." == c)
            return 0;
        return std::stoul(c);
    }

    static uint16 inputCharToPotentialMask(char c) {
        const char dot = '.';
        if (dot == c) {
            std::cout << "Encountered DOT in " << c << std::endl;
            return 0b111111111;
        }
        return 0x1 << (int)c;
    }

    public:

        /* unit tests */
        static void test_inputCharToInt() {
            uint16 result;
            result = inputCharToInt(".");
            TEST(result == 0);
            result = inputCharToInt("9");
            TEST(result == 9);
        } 
        static void test_inputCharToPotentialMask() {
            uint16 result;
            result = inputCharToPotentialMask('.');
            TEST(result == 0b111111111);
            TEST(result == 0x111);
            result = inputCharToPotentialMask('1');
            TEST(result == 0b1);
            TEST(result == 0x1);
            result = inputCharToPotentialMask('2');
            TEST(result == 0b10);
            TEST(result == 0x2);
            result = inputCharToPotentialMask('9');
            TEST(result == 0b100000000);
            TEST(result == 0x9);
        }


        board unsolved;
        Parser(std::string s) {
            this->s = s;
        }
        int parse() {
            std::cout << "Parsing this:" << this->s << std::endl;
            if (this->s.length() != N * N) {
                std::cout << "Input string has wrong length" << std::endl;
                return -1;
            }
            for (int i = 0; i < N * N; i++) {
                //std::string c = this->s.substr(i, 1);
                //unsolved[i] = inputCharToInt(c);
                char c = this->s[i];
                unsolved[i] = inputCharToPotentialMask(c);
            }
            return 0;
        }
};

class Visualizer {
    public:
        Visualizer() {
            std::cout << "creating Visualizer" << std::endl;
        }
        std::string print(board& b) {
            std::string outString;
            for (int i = 0; i < N*N; i++) {
                outString += std::to_string(b[i]);
                if ((i + 1) % 3 == 0)
                    outString += " ";
                if ((i + 1) % 9 == 0)
                    outString += "\n";
            }
            return outString;
        }
};

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
    	TEST_SECTION("sudoku");

        TEST_GROUP("test_parser");
        Parser::test_inputCharToInt();
        Parser::test_inputCharToPotentialMask();

        TEST_GROUP("other");
        TEST(0x1 == 1);
        TEST((0x1 << 1) == 2);
        TEST(0x02 == 2);
        TEST(0b1 == 1);
        TEST(0b10 == 2);

        TEST_SUMMARY;
    }
    catch (...)
    {
        TEST_EXCEPTION;
    }
    return 0;
}

int main(int argc, char **argv) {
    std::cout << "sudoku solver, v 0.0.0" << std::endl;
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    char testArg[] = "test";
    if (argc > 1 && strcmp(argv[1], testArg) == 0) {
        std::cout << "running unit tests ... ";
        int areUnitTestsPassed = runUnitTests();
        exit(areUnitTestsPassed);
    }

    std::string inputString;
    std::string tmpString;
    while (std::getline(std::cin, tmpString))
    {
      inputString += tmpString;
    } 

    Parser * p;
    p = new Parser(inputString);
    int parseResult = p->parse();
    std::cout << "Parser code: " << parseResult << std::endl;
    uint16 potentials[N*N];

    Visualizer * v;
    v = new Visualizer();
    std::cout << "Parsed result: " << std::endl \
        << v->print(p->unsolved) << std::endl;

    ReducerInterface * r;
    r = new Reducer();
    std::cout << "Reducer info: " << r << std::endl;
    return 0;
}

