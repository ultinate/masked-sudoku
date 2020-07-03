#include <iostream>
#include <string>
#include <cstring>
#include "parser.hpp"
#include "slicer.hpp"


class ReducerInterface {
    public:
        virtual bool reduceSlice(mask *slice) = 0;
};

class Reducer
    : public ReducerInterface {
    public:
        Reducer() {}
        ~Reducer() {}
        bool reduceSlice(mask *slice) {
            return true;
        }
};

/**
 * Visualizer
 */
class Visualizer {
    public:
        Visualizer() {
        }
        static std::string maskToStr(mask m) {
            unsigned int number = Parser::getIntFromMask(m);
            if (number > 0)
                return std::to_string(number);
            else
                return " ";
        }
        static std::string printSlice(mask *slice) {
            std::string outString = "";
            outString += "| ";
            for (int i = 0; i < N; i++) {
                outString += maskToStr(slice[i]);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
            }
            return outString;
        }
        static std::string printBoard(mask *board) {
            std::string outString = "";
            outString += "-------------------------\n";
            for (int i = 0; i < N*N; i++) {
                if (i % N == 0)
                    outString += "| ";
                outString += maskToStr(board[i]);
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

void printSlices(SlicerInterface *slicer) {
    std::cout << slicer->getName() << ": " << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    for (mask *slice = sliceIt.Next();
            !sliceIt.isDone();
            slice = sliceIt.Next()) {
        std::cout << Visualizer::printSlice(slice) << std::endl;
    }
}

int main(int argc, char **argv) {
    std::cout << "sudoku solver, v 0.0.2" << std::endl;
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
    mask *board = p->unsolvedBoard;

    std::cout << "Parsed result: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    SlicerInterface **slicer;
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    slicer[3] = new DiagonalSlicer(board);
    for (int i = 0; i < 4; i++) {
        printSlices(slicer[i]);
    }
 
    ReducerInterface * r;
    r = new Reducer();
    std::cout << "Reducer info: " << r << std::endl;

    return 0;
}

