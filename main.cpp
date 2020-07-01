#include <iostream>
#include <string>
#include <cstring>
#include "parser.hpp"


class SlicerInterface {
    public:
        virtual ~SlicerInterface();
        virtual slice& getSlice(board&);
};

class HorizontalSlicer : public SlicerInterface {


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

/**
 * Visualizer
 */
class Visualizer {
    public:
        Visualizer() {
            std::cout << "creating Visualizer" << std::endl;
        }
        static std::string maskToStr(mask m) {
            unsigned int number = Parser::getIntFromMask(m);
            if (number > 0)
                return std::to_string(number);
            else
                return " ";
        }
        std::string print(board& b) {
            std::string outString = "";
            outString += "-------------------------\n";
            for (int i = 0; i < N*N; i++) {
                if (i % N == 0)
                    outString += "| ";
                outString += maskToStr(b[i]);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
                if ((i + 1) % N == 0)
                    outString += "\n";
                if ((i + 1) % (N*3) == 0)
                    outString += "-------------------------\n";
            }
            return outString;
        }
};

int main(int argc, char **argv) {
    std::cout << "sudoku solver, v 0.0.1" << std::endl;
    std::string inputString;
    std::string tmpString;
    while (std::getline(std::cin, tmpString))
    {
      inputString += tmpString;
    } 

    Parser * p;
    p = new Parser(inputString);
    int parseResult = p->parse();
    if (parseResult != 0) {
        std::cout << "Parsing not successful. Terminating." << std::endl;
        return parseResult;
    }

    Visualizer * v;
    v = new Visualizer();
    std::cout << "Parsed result: " << std::endl;
    std::cout << v->print(p->unsolved) << std::endl;

    ReducerInterface * r;
    r = new Reducer();
    std::cout << "Reducer info: " << r << std::endl;
    return 0;
}

