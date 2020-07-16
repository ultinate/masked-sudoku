#include <iostream>
#include <string>
#include <cstring>
#include "parser.hpp"
#include "slicer.hpp"
#include "solver.hpp"
#include "visualizer.hpp"


void printSlices(SlicerInterface *slicer) {
    // std::cout << slicer->getName() << ": " << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next()) {
        std::cout << Visualizer::printSlice(*slice) << std::endl;
    }
}

bool solveAllSlices(SlicerInterface *slicer, SolverInterface *solver) {
    bool hasChanged = false;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next()) {
        // std::cout << "Slice before: " << Visualizer::printSlice(*slice) << std::endl;
        bool changed = solver->solveSlice(*slice);
        if (changed) {
            // std::cout << "Slice after:  " << Visualizer::printSlice(*slice) << std::endl;
        }
        hasChanged |= changed;
    }
    return hasChanged;
}

/**
 * Apply all solvers to all slices and loop as long as some progress
 * is made.
 *
 * TODO: Check validity of result in the end
 */
bool solveBoard(mask *board) {
    int maxLoops = 1000;
    int infoLoops = 100;
    bool hasChanged = true;
    int solverLength = 2;
    int slicerLength = 1;  // DEBUG
    SolverInterface *solver[solverLength];
    SlicerInterface *slicer[slicerLength];
    DetermineSolver ds;
    EliminateSolver es;
    solver[0] = &ds;
    solver[1] = &es;
    int numLoops = 0;
    while (hasChanged) {
        mask *boardCopy = new mask[N*N];
        es.copyBoard(boardCopy, board);
        for (int i = 0; i < solverLength; i++) {
            slicer[0] = new HorizontalSlicer(board);
            slicer[1] = new VerticalSlicer(board);
            slicer[2] = new BoxSlicer(board);
            // some sudokus don't implement the diagonal rule
            // slicer[3] = new DiagonalSlicer(board);
            for (int j = 0; j < slicerLength; j++) {
                solveAllSlices(slicer[j], solver[i]);
            }
        }
        ++numLoops;
        hasChanged = !es.isBoardEqual(boardCopy, board);
        if (numLoops % infoLoops == 0) {
            std::cout << numLoops << " loops and counting ..." << std::endl;
        }
        if (numLoops >= maxLoops) {
            std::cout << numLoops << " reached. Exiting." << std::endl;
            break;
        }
    }
    std::cout << "Done after " << numLoops << " loops." << std::endl;
    return true;
}


int main(int argc, char **argv) {
    std::cout << "sudoku solver, v 0.0.2" << std::endl;
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
    EliminateSolver es;
    int globalResult = es.isSolved(board);
    if (globalResult) {
        std::cout << "Solved!" << std::endl;
    }
    else {
        std::cout << "Not successful!" << std::endl;
    }

    std::cout << "Output board: " << std::endl;
    for (int i = 0; i < N; i++) {
        std::cout << Visualizer::printBoard(board) << std::endl;
        std::cout << Visualizer::printBoardHighlight(board, i+1) << std::endl;
    }

    return globalResult;
}

