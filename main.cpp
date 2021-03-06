#include <iostream>
#include <string>
#include <cstring>
#include "parser.hpp"
#include "slicer.hpp"
#include "solver.hpp"
#include "visualizer.hpp"


int main(int argc, char **argv) {
    std::cout << "simple sudoku solver, v 1.0.0" << std::endl;
    bool isVerbose = false;
    if (argc > 1) {
        if (strcmp(argv[1], "-v") == 0 or strcmp(argv[1], "--verbose") == 0) {
            isVerbose = true;
        }
        else {
            std::cout << "Usage: ./sudoku [-v]" << std::endl;
            exit(-1);
        }
    }
    std::string inputString;
    std::string tmpString;
    for (int i = 0; i < N; i++) {
        std::getline(std::cin, tmpString);
        inputString += tmpString;
    } 

    Parser parser(inputString);
    int parseResult = parser.parse();
    if (parseResult != 0) {
        std::cout << "Parsing not successful. Terminating." << std::endl;
        return parseResult;
    }
    mask *board = parser.getBoard();

    std::cout << "Input board: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    std::cout << "Solving board ..." << std::endl;
    GuessSolver solver;
    int globalResult = solver.solveBoard(board, isVerbose);
 
    std::cout << "Output board: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    if (globalResult) {
        std::cout << "Solved!" << std::endl;
        return 0;
    }
    else {
        std::cout << "Not successful!" << std::endl;
        return -1;
    }
}
