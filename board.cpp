#include "board.hpp"
#include "parser.hpp"
#include "visualizer.hpp"
#include "parser.hpp"
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
    mask **sliceT = transposeSlice(slice);
    bool isCorrect = isSolvedDetail(sliceT);
    delete [] sliceT;
    return isFilled && isCorrect;
}

bool BoardManager::isBoardLegal(mask *board) {
    // TODO: Contains duplicate code. Refactor.
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    slicer[0] = new HorizontalSlicer();
    slicer[1] = new VerticalSlicer();
    slicer[2] = new BoxSlicer();
    mask **slice = new mask*[N];
    for (int i = 0; i < slicerLength; i++) {
        for (slicer[i]->init(board, slice);
                !slicer[i]->isDone();
                slicer[i]->nextSlice(slice)) {
            if (!isSliceLegal(slice)) {
                delete [] slice;
                return false;
            }
        }
    }
    delete [] slice;
    for (int i = 0; i < slicerLength; i++) {
        delete slicer[i];
    }
    return true;
}

bool BoardManager::isBoardSolved(mask *board) {
    int slicerLength = 3;
    SlicerInterface *slicer[slicerLength];
    slicer[0] = new HorizontalSlicer();
    slicer[1] = new VerticalSlicer();
    slicer[2] = new BoxSlicer();
    mask **slice = new mask*[N];
    for (int i = 0; i < slicerLength; i++) {
        for (slicer[i]->init(board, slice);
                !slicer[i]->isDone();
                slicer[i]->nextSlice(slice)) {
            if (!isSliceSolved(slice)) {
                delete [] slice;
                return false;
            }
        }
    }
    delete [] slice;
    for (int i = 0; i < slicerLength; i++) {
        delete slicer[i];
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

