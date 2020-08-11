#include <bitset>
#include <chrono>
#include <thread>
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

void BoardManager::deepCopyBoard(mask *boardTo, mask *boardFrom) {
    for (int i = 0; i < N*N; i++) {
        boardTo[i] = boardFrom[i];
    }
}

bool BoardManager::areSlicesEqual(mask **sliceLhs, mask **sliceRhs) {
    for (int i = 0; i < N; i++) {
        if (sliceLhs[i] != sliceRhs[i]) {
            return false;
        }
    }
    return true;
}

bool BoardManager::areSliceValuesEqual(mask **sliceLhs, mask **sliceRhs) {
    for (int i = 0; i < N; i++) {
        if (*sliceLhs[i] != *sliceRhs[i]) {
            return false;
        }
    }
    return true;
}

bool BoardManager::isSliceLegal(mask **slice) {
    mask solvedNumbers = 0x0;
    for (int i = 0; i < N; i++) {
        if (Parser::isOnlyOneBit(*slice[i])) {
            if (solvedNumbers & *slice[i]) {
                // duplicate number detected
                return false;
            }
            // mark number as taken
            solvedNumbers = solvedNumbers | *slice[i];
        }
    }
    return true;
}

bool BoardManager::isSliceSolved(mask **slice) {
    bool isFilled = isSolvedDetail(slice);
    bool isCorrect = isSolvedDetail(transposeSlice(slice));
    return isFilled && isCorrect;
}

bool BoardManager::isBoardLegal(mask *board) {
    // TODO: Contains duplicate code. Refactor.
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    for (int i = 0; i < slicerLength; i++) {
        for (mask **slice = slicer[i]->nextSlice();
                !slicer[i]->isDone();
                slice = slicer[i]->nextSlice()) {
            if (!isSliceLegal(slice)) {
                return false;
            }
        }
    }
    return true;
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

bool BoardManager::isInsideList(mask *needle, mask **haystack,
        unsigned int haystackLength) {
    for (int i = 0; i < haystackLength; i++) {
        if (needle == haystack[i]) {
            return true;
        }
    }
    return false;
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
    bool isEqual = BoardManager::areSliceValuesEqual(slice, sliceAfter);
    if (!isEqual) {
        BoardManager::deepCopySlice(slice, sliceAfter);
    }
    delete [] sliceT;
    delete [] sliceAfter;
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
        mask **listOfOverlaps = new mask*[N];
        int lengthOfOverlaps = getListOfOverlaps(candidate, listOfOverlaps,
                                                 sliceOrigin, sliceTarget);
        if (0 < lengthOfOverlaps and lengthOfOverlaps <= 3) {
            eliminate(sliceTarget, candidate, listOfOverlaps, lengthOfOverlaps);
        }
    }
}

int OverlapSolver::getListOfOverlaps(int candidate, mask **list,
        mask **sliceOrigin, mask **sliceTarget) {
    int length = 0;
    for (int i = 0; i < N; i++) {
        if (Parser::isBitSet(*sliceOrigin[i], candidate)) {
            // We have a possible location for _candiate_
            if (BoardManager::isInsideList(sliceOrigin[i], sliceTarget, N)) {
                // We have an overlap of a possible location with _s2_
                list[length] = sliceOrigin[i];
                length++;
            }
            else {
                // We have a possible location that does not overlap. Stop.
                length = 0;
                break;
            }
        }
    }
    return length;
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

void OverlapSolver::eliminate(mask **slice, unsigned int valueToEliminate,
        mask **exceptMasks, unsigned int exceptMasksLength) {
    mask eliminateMask = Parser::getNotMask(valueToEliminate);
    for (int i = 0; i < N; i++) {
        if (!BoardManager::isInsideList(slice[i], exceptMasks, exceptMasksLength)) {
            *slice[i] = *slice[i] & eliminateMask;
        }
    }
}


/**
 * GuessSolver
 */

GuessSolver::GuessSolver() {
    solvers[0] = new DetermineSolver();
    solvers[1] = new EliminateSolver();
    solvers[2] = new OverlapSolver();
}
/**
 * Apply all solvers to all slices.
 *
 * This method applies all deterministric solvers as long as some progress
 * is made. A maximum of N*N*N = 729 loops are needed if we assume that -
 * in each loop - at least one bit changes.
 */
void GuessSolver::runSolvers(mask *board, bool isVerbose) {
    int maxLoops = N * N * N;
    int infoLoops = 1;
    int numLoops;
    // TODO: If verbose option is used, users won't like waiting 730 seconds.
    // Skip loops if nothing happens anymore
    for (numLoops = 0; numLoops < maxLoops; numLoops++) {
        for (int i = 0; i < solverLength; i++) {
            solvers[i]->solveBoard(board);
        }
        if (isVerbose and (numLoops % infoLoops == 0)) {
            std::cout << "\n\n" << std::endl;
            std::cout << "Loop " << (numLoops + 1) << " ..." << std::endl;
            std::cout << Visualizer::printBoard(board) << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout.flush();
        }
    }
}


/**
 * Solve the board
 *
 * This method tries to apply all solvers, then guesses remaining
 * locations recursively.
 */
bool GuessSolver::solveBoard(mask *board, bool isVerbose) {
    return solveBoard(board, isVerbose, 0);
}
bool GuessSolver::solveBoard(mask *board, bool isVerbose, int numberOfGuesses) {

    runSolvers(board, isVerbose);

    if (BoardManager::isBoardSolved(board)) {
        return true;
    }
    if (!BoardManager::isBoardLegal(board)) {
        return false;
    }
    if (numberOfGuesses > N*N) {
        return false;
    }
    for (int i = 0; i < N * N; i++) {
        if (!Parser::isOnlyOneBit(board[i])) {
            // This is an unsolved field. Let's try a guess.
            for (int guess = 1; guess < N + 1; guess++) {
                if (Parser::isBitSet(board[i], guess)) {
                    // This is a legal guess. Let's apply it.
                    mask *backupBoard = new mask[N*N];
                    BoardManager::deepCopyBoard(backupBoard, board);
                    board[i] = Parser::getMaskFromInt(guess);
                    if (solveBoard(board, isVerbose, numberOfGuesses + 1)) {
                        return true;
                    }
                    else {
                        // revert guess
                        BoardManager::deepCopyBoard(board, backupBoard);
                    }
                    delete [] backupBoard;
                }
            }
        }
    }
    return false;
}
