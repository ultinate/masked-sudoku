#include <bitset>
#include "parser.hpp"
#include "visualizer.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "slicer.hpp"


/**
 * BoardManager
 */

mask ** BoardManager::transposeSlice(mask **slice) {
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

void BoardManager::copySlice(mask **sliceTo, mask **sliceFrom) {
    for (int i = 0; i < N; i++) {
        sliceTo[i] = sliceFrom[i];
    }
}

void BoardManager::deepCopySlice(mask **sliceTo, mask **sliceFrom) {
    for (int i = 0; i < N; i++) {
        *sliceTo[i] = *sliceFrom[i];
    }
}

bool BoardManager::isSliceEqual(mask **sliceLhs, mask **sliceRhs) {
    for (int i = 0; i < N; i++) {
        if (*sliceLhs[i] != *sliceRhs[i]) {
            return false;
        }
    }
    return true;
}

void BoardManager::copyBoard(mask *boardTo, mask *boardFrom) {
    for (int i = 0; i < N * N; i++) {
        boardTo[i] = boardFrom[i];
    }
}

bool BoardManager::isBoardEqual(mask *lhs, mask *rhs) {
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

bool BoardManager::isSliceSolved(mask **slice) {
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

bool BoardManager::isBoardSolved(mask *board) {
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    for (int i = 0; i < slicerLength; i++) {
        for (mask **slice = slicer[i]->nextSlice();
                !slicer[i]->isDone();
                slice = slicer[i]->nextSlice()) {
            if (!isSliceSolved(slice)) {
                return false;
            }
        }
    }
    return true;
}

bool BoardManager::isSolvedDetail(mask **slice) {
    for (int i = 0; i < N; i++) {
        if (!Parser::isOnlyOneBit(*slice[i])) {
            return false;
        }
    }
    return true;
}



/**
 * SolverInterface
 */

// Empty Destructor to avoid `missing vtable` linker errors
SolverInterface::~SolverInterface() {
}


/**
 * SliceSolverInterface
 */

void SliceSolverInterface::solveAllSlices(SlicerInterface *slicer) {
    for (mask **slice = slicer->nextSlice();
            !slicer->isDone();
            slice = slicer->nextSlice()) {
        solveSlice(slice);
    }
}


/**
 * DetermineSolver
 */

void DetermineSolver::solveSlice(mask **slice) {
    // std::cout << "DetermineSolver before: " << Visualizer::printSlice(slice) << std::endl;
    mask **sliceT = BoardManager::transposeSlice(slice);
    EliminateSolver es;
    es.solveSlice(sliceT);
    mask **sliceAfter = BoardManager::transposeSlice(sliceT);
    bool isEqual = BoardManager::isSliceEqual(slice, sliceAfter);
    if (!isEqual) {
        BoardManager::deepCopySlice(slice, sliceAfter);
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

void SliceSolverInterface::solveBoard(mask *board) {
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    for (int j = 0; j < slicerLength; j++) {
        solveAllSlices(slicer[j]);
        slicer[j]->reset();
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


/**
 * OverlapSolver
 */

void OverlapSolver::solveSlice(mask **slice) {
    // TODO: Raise error here
}

void OverlapSolver::solveSlice(mask **sliceOrigin, mask **sliceTarget) {
    // for each number 1 through 9
    for (int candidate = 0; candidate < N; candidate++) {
        bool isCandidateToEliminate = true;
        int lengthOfOverlaps = 0;
        mask **listOfOverlaps = new mask*[N];
        // check each position
        for (int i = 0; i < N; i++) {
            if (Parser::isBitSet(candidate, i)) {
                if (isInsideList(sliceOrigin[i], sliceTarget, N)) {
                    // interesting case, we have an overlap of a possible
                    // location
                    listOfOverlaps[lengthOfOverlaps++] = sliceOrigin[i];
                }
                else {
                    // not interesting, we have a possible location that
                    // does not overlap
                    isCandidateToEliminate = false;
                    continue;
                }
            }
            else {
                // not interesting
            }
        }
        if (isCandidateToEliminate) {
            if (lengthOfOverlaps > 0 and lengthOfOverlaps <= 3) {
                eliminate(sliceTarget, candidate, listOfOverlaps,
                          lengthOfOverlaps);
            }
        }
    }
}

void OverlapSolver::solveBoard(mask *board) {
    SlicerInterface *origin;
    SlicerInterface *target;
    // Horizontal-to-box
    origin = new HorizontalSlicer(board);
    target = new BoxSlicer(board);
    solveBoard(board, origin, target);
    // Vertical-to-box
    origin = new VerticalSlicer(board);
    target = new BoxSlicer(board);
    solveBoard(board, origin, target);
    // Box-to-horizontal
    origin = new BoxSlicer(board);
    target = new HorizontalSlicer(board);
    solveBoard(board, origin, target);
    // Box-to-vertical
    origin = new BoxSlicer(board);
    target = new VerticalSlicer(board);
    solveBoard(board, origin, target);
}

void OverlapSolver::solveBoard(mask *board, SlicerInterface *origin,
        SlicerInterface *target) {
    mask **sliceOrigin;
    mask **sliceTarget;
    for (sliceOrigin = origin->nextSlice(),
            sliceTarget = target->nextSlice();
            !origin->isDone() && !target->isDone();
            sliceOrigin = origin->nextSlice(),
            sliceTarget = target->nextSlice()
            ) {
        solveSlice(sliceOrigin, sliceTarget);
    }
}

void OverlapSolver::eliminate(
        mask **slice, unsigned int valueToEliminate,
        mask **exceptMasks, unsigned int exceptMasksLength) {
    mask eliminateMask = Parser::getNotMask(valueToEliminate);
    for (int i = 0; i < N; i++) {
        if (!isInsideList(slice[i], exceptMasks, exceptMasksLength)) {
            *slice[i] = *slice[i] & eliminateMask;
        }
    }
}

bool OverlapSolver::isInsideList(mask *needle, mask **haystack,
        unsigned int haystackLength) {
    for (int i = 0; i < haystackLength; i++) {
        if (needle == haystack[i]) {
            return true;
        }
    }
    return false;
}

