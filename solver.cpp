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

mask * SolverInterface::transposeSlice(mask *slice) {
    mask *sliceT = new mask[N];
    for (int col = 0; col < N; col++) {
        mask vertical = 0x0;
        for (int row = 0; row < N; row++) {
            mask b = (slice[row] & (0x1 << (N - col - 1)));
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
    return sliceT;
}

void SolverInterface::copySlice(mask *sliceTo, mask *sliceFrom) {
    for (int i = 0; i < N; i++) {
        sliceTo[i] = sliceFrom[i];
    }
}

bool SolverInterface::isSliceEqual(mask *lhs, mask *rhs) {
    for (int i = 0; i < N; i++) {
        if (rhs[i] != lhs[i]) {
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
    SliceIterator<SlicerInterface> sliceItLhs = slicerLhs.createIterator();
    SliceIterator<SlicerInterface> sliceItRhs = slicerRhs.createIterator();
    mask **sliceLhs;
    mask **sliceRhs;
    for (sliceLhs = sliceItLhs.Next(), sliceRhs = sliceItRhs.Next();
            !sliceItLhs.isDone() && !sliceItRhs.isDone();
            sliceLhs = sliceItLhs.Next(), sliceRhs = sliceItRhs.Next()
            ) {
        if (!isSliceEqual(*sliceLhs, *sliceRhs)) {
            return false;
        }
    }
    return true;
}

bool SolverInterface::isSolved(mask *slice) {
    bool isFilled = isSolvedDetail(slice);
    bool isCorrect = isSolvedDetail(transposeSlice(slice));
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

bool SolverInterface::isSolved(mask **board) {
    // TODO
    return true;
}

bool SolverInterface::isSolvedDetail(mask *slice) {
    for (int i = 0; i < N; i++) {
        if (!Parser::isOnlyOneBit(slice[i])) {
            return false;
        }
    }
    return true;
}


/**
 * DetermineSolver
 */

bool DetermineSolver::solveSlice(mask *slice) {
    //std::cout << "DetermineSolver before: " << Visualizer::printSlice(slice) << std::endl;
    EliminateSolver es;
    mask *sliceT = es.transposeSlice(slice);
    //std::cout << "DetermineSolver T: " << Visualizer::printSlice(sliceT) << std::endl;
    es.solveSlice(sliceT);
    mask *sliceAfter = es.transposeSlice(sliceT);
    //std::cout << "DetermineSolver after:  " << Visualizer::printSlice(sliceAfter) << std::endl;
    bool hasChanged = !isSliceEqual(slice, sliceAfter);
    if (hasChanged) {
        copySlice(slice, sliceAfter);
    }
    return hasChanged;
}

/**
 * EliminateSolver
 */

bool EliminateSolver::solveSlice(mask *slice) {
    bool hasChanged = false;
    for (int i = 0; i < N; i++) {
        if (Parser::isOnlyOneBit(slice[i])) {
            unsigned int valueToEliminate = Parser::log2(slice[i]);
            hasChanged |= eliminate(slice, valueToEliminate, i);
        }
    }
    return hasChanged;
}

bool EliminateSolver::eliminate(mask *slice, unsigned int valueToEliminate,
                      int exceptIndex) {
    bool hasChanged = false;
    mask eliminateMask = Parser::getNotMask(valueToEliminate);
    for (int i = 0; i < N; i++) {
        if (i != exceptIndex) {
            mask before = slice[i];
            slice[i] = slice[i] & eliminateMask;
            if (before != slice[i])
                hasChanged = true;
        }
    }
    return hasChanged;
}

