#include <bitset>
#include "parser.hpp"
#include "visualizer.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "slicer.hpp"


// Comments and docstrings: see header file.


/**
 * SolverInterface
 */

// Empty Destructor to avoid `missing vtable` linker errors
SolverInterface::~SolverInterface() {
}

// TODO: Creating mask array, then slice seems redundant. Simplify?
mask ** SolverInterface::transposeSlice(mask **slice) {
    mask *sliceT = new mask[N];
    for (int col = 0; col < N; col++) {
        mask vertical = 0x0;
        for (int row = 0; row < N; row++) {
            mask b = *slice[row] & (0x1 << (N - col - 1));
            if (row - col > 0) {
                b = b >> (row - col);
            }
            else if (row - col < 0) {
                b = b << (col - row);
            }
            vertical |= b;
        }
        sliceT[col] = vertical;
    }
    mask **sliceTPtr = new mask*[N];
    for (int i = 0; i < N; i++) {
        sliceTPtr[i] = &sliceT[i];
    }
    return sliceTPtr;
}

void SolverInterface::copySlice(mask **sliceTo, mask **sliceFrom) {
    for (int i = 0; i < N; i++) {
        sliceTo[i] = sliceFrom[i];
    }
}

bool SolverInterface::isSliceEqual(mask **sliceLhs, mask **sliceRhs) {
    for (int i = 0; i < N; i++) {
        if (*sliceLhs[i] != *sliceRhs[i]) {
            return false;
        }
    }
    return true;
}

void SolverInterface::copyBoard(mask *boardTo, mask *boardFrom) {
    for (int i = 0; i < N * N; i++) {
        boardTo[i] = boardFrom[i];
    }
}

bool SolverInterface::isBoardEqual(mask *lhs, mask *rhs) {
    HorizontalSlicer slicerLhs(lhs);
    HorizontalSlicer slicerRhs(rhs);
    mask **sliceLhs;
    mask **sliceRhs;
    for (sliceLhs = slicerLhs.nextSlice(), sliceRhs = slicerRhs.nextSlice();
            !slicerLhs.isDone() && !slicerRhs.isDone();
            sliceLhs = slicerLhs.nextSlice(), sliceRhs = slicerRhs.nextSlice()
            ) {
        if (!isSliceEqual(sliceLhs, sliceRhs)) {
            return false;
        }
    }
    return true;
}

bool SolverInterface::isSliceSolved(mask **slice) {
    bool isFilled = SolverInterface::isSolvedDetail(slice);
    bool isCorrect = SolverInterface::isSolvedDetail(transposeSlice(slice));
    if (isFilled && isCorrect) {
        return true;
    }
    else if (!isFilled && !isCorrect) {
        return false;
    }
    else {
        std::cout << "Error occured. Illegal slice state: " << \
            Visualizer::printSlice(slice) << std::endl;
        return false;
    }
}

bool SolverInterface::isBoardSolved(mask *board) {
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    EliminateSolver es;
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    for (int i = 0; i < slicerLength; i++) {
        for (mask **slice = slicer[i]->nextSlice();
                !slicer[i]->isDone();
                slice = slicer[i]->nextSlice()) {
            if (!es.isSliceSolved(slice)) {
                return false;
            }
        }
    }
    return true;
}

void SolverInterface::solveAllSlices(SlicerInterface *slicer) {
    for (mask **slice = slicer->nextSlice();
            !slicer->isDone();
            slice = slicer->nextSlice()) {
        solveSlice(slice);
    }
}

bool SolverInterface::isSolvedDetail(mask **slice) {
    for (int i = 0; i < N; i++) {
        if (!Parser::isOnlyOneBit(*slice[i])) {
            return false;
        }
    }
    return true;
}


/**
 * DetermineSolver
 */

void DetermineSolver::solveSlice(mask **slice) {
    //std::cout << "DetermineSolver before: " << Visualizer::printSlice(slice) << std::endl;
    EliminateSolver es;
    mask **sliceT = es.transposeSlice(slice);
    //std::cout << "DetermineSolver T: " << Visualizer::printSlice(sliceT) << std::endl;
    es.solveSlice(sliceT);
    mask **sliceAfter = es.transposeSlice(sliceT);
    //std::cout << "DetermineSolver after:  " << Visualizer::printSlice(sliceAfter) << std::endl;
    bool hasChanged = !isSliceEqual(slice, sliceAfter);
    if (hasChanged) {
        copySlice(slice, sliceAfter);
    }
}


/**
 * EliminateSolver
 */

void EliminateSolver::solveSlice(mask **slice) {
    for (int i = 0; i < N; i++) {
        if (Parser::isOnlyOneBit(*slice[i])) {
            unsigned int valueToEliminate = Parser::log2(*slice[i]);
            eliminate(slice, valueToEliminate, i);
        }
    }
}

void EliminateSolver::eliminate(mask **slice, unsigned int valueToEliminate,
                      int exceptIndex) {
    mask eliminateMask = Parser::getNotMask(valueToEliminate);
    for (int i = 0; i < N; i++) {
        if (i != exceptIndex) {
            *slice[i] = *slice[i] & eliminateMask;
        }
    }
}

