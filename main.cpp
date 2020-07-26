#include <iostream>
#include <string>
#include <cstring>
#include "parser.hpp"
#include "slicer.hpp"
#include "solver.hpp"
#include "visualizer.hpp"


/**
 * Apply all solvers to all slices.
 *
 * This method loops as long as some progress is made. A maximum of N*N*N = 729 loops
 * are needed if we assume that, in each loop, at least one bit changes.
 */
bool solveBoard(mask *board) {
    int maxLoops = N * N * N;
    int infoLoops = 1;
    int solverLength = 2;
    int slicerLength = 3;
    SolverInterface *solver[solverLength];
    SlicerInterface *slicer[slicerLength];
    solver[0] = new DetermineSolver();
    solver[1] = new EliminateSolver();
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    // some sudokus don't implement the diagonal rule, so let's not use DiagonalSlicer
    // slicer[3] = new DiagonalSlicer(board);
    int numLoops;
    for (numLoops = 0; numLoops < maxLoops; numLoops++) {
        for (int i = 0; i < solverLength; i++) {
            for (int j = 0; j < slicerLength; j++) {
                solver[i]->solveAllSlices(slicer[j]);
                slicer[j]->reset();
            }
        }
        ++numLoops;
        if (numLoops % infoLoops == 0) {
            std::cout << numLoops << " loops and counting ..." << std::endl;
            std::cout << Visualizer::printBoard(board) << std::endl;
        }
        if (SolverInterface::isBoardSolved(board)) {
            break;
        }
    }
    std::cout << "Done after " << numLoops << " loops." << std::endl;
    return true;
}


int main(int argc, char **argv) {
    std::cout << "simple sudoku solver, v 0.0.3" << std::endl;
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
    mask *board = p->unsolvedBoard;

    std::cout << "Input board: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    std::cout << "Solving board ..." << std::endl;
    solveBoard(board);
    int globalResult = SolverInterface::isBoardSolved(board);
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

