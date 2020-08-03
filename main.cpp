#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include "parser.hpp"
#include "slicer.hpp"
#include "solver.hpp"
#include "visualizer.hpp"


/**
 * Apply all solvers to all slices.
 *
 * This method loops as long as some progress is made. A maximum of
 * N*N*N = 729 loops are needed if we assume that - in each loop - at
 * least one bit changes.
 */
bool solveBoard(mask *board, bool isVerbose) {
    int maxLoops = N * N * N;
    int infoLoops = 1;
    int solverLength = 3;
    SolverInterface *solver[solverLength];
    solver[0] = new DetermineSolver();
    solver[1] = new EliminateSolver();
    solver[2] = new OverlapSolver();
    int numLoops;
    for (numLoops = 0; numLoops < maxLoops; numLoops++) {
        for (int i = 0; i < solverLength; i++) {
            solver[i]->solveBoard(board);
        }
        if (BoardManager::isBoardSolved(board)) {
            break;
        }
        if (isVerbose and (numLoops % infoLoops == 0)) {
            std::cout << "\n\n" << std::endl;
            std::cout << "Loop " << (numLoops + 1) << " ..." << std::endl;
            std::cout << Visualizer::printBoard(board) << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout.flush();
        }
    }
    std::cout << "Done after " << (numLoops + 1) << " loops." << std::endl;
    return true;
}


int main(int argc, char **argv) {
    std::cout << "simple sudoku solver, v 0.0.4" << std::endl;
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

    Parser *p = new Parser(inputString);
    int parseResult = p->parse();
    if (parseResult != 0) {
        std::cout << "Parsing not successful. Terminating." << std::endl;
        return parseResult;
    }
    mask *board = p->getBoard();

    std::cout << "Input board: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    std::cout << "Solving board ..." << std::endl;
    solveBoard(board, isVerbose);
    int globalResult = BoardManager::isBoardSolved(board);
    int returnValue;
    if (globalResult) {
        std::cout << "Solved!" << std::endl;
        returnValue = 0;
    }
    else {
        std::cout << "Not successful!" << std::endl;
        returnValue = -1;
    }

    std::cout << "Output board: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    // DEBUG
    // int i = 6;
    // std::cout << Visualizer::printBoardHighlight(board, i) << std::endl;

    return returnValue;
}

