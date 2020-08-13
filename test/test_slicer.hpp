#include <string>
#include <sstream>
#include "../sTest/test.h"
#include "../slicer.hpp"
#include "../visualizer.hpp"


/** Returns a board like this:
 *
 * -------------------------
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * -------------------------
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * -------------------------
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * | 1 2 3 | 4 5 6 | 7 8 9 |
 * -------------------------
*/
void getBoard(mask *board) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            board[row*N + col] = 0x1 << col;
        }
    }
}

/**
 * Returns a board like this:
 *
 * -------------------------
 * | 1 1 1 | 2 2 2 | 3 3 3 |
 * | 1 1 1 | 2 2 2 | 3 3 3 |
 * | 1 1 1 | 2 2 2 | 3 3 3 |
 * -------------------------
 * | 4 4 4 | 5 5 5 | 6 6 6 |
 * | 4 4 4 | 5 5 5 | 6 6 6 |
 * | 4 4 4 | 5 5 5 | 6 6 6 |
 * -------------------------
 * | 7 7 7 | 8 8 8 | 9 9 9 |
 * | 7 7 7 | 8 8 8 | 9 9 9 |
 * | 7 7 7 | 8 8 8 | 9 9 9 |
 * -------------------------
*/
void getBoxedBoard(mask *board) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            unsigned int val = 1;
            if (row < 3 and col < 3) val = 1;
            if (row < 3 and col >= 3 and col < 6) val = 2;
            if (row < 3 and col >= 6) val = 3;
            if (row >= 3 and row < 6 and col < 3) val = 4;
            if (row >= 3 and row < 6 and col >= 3 and col < 6) val = 5;
            if (row >= 3 and row < 6 and col >= 6) val = 6;
            if (row >= 6 and col < 3) val = 7;
            if (row >= 6 and col >= 3 and col < 6) val = 8;
            if (row >= 6 and col >= 6) val = 9;
            board[row*N + col] = 0x1 << (val - 1);
        }
    }
}

/**
 * Horizontal Slicer unit tests
 */
void test_HorizontalSlicer_Next() {
    mask *board = new mask[N*N];
    getBoard(board);
    SlicerInterface *slicer = new HorizontalSlicer();
    mask **slice = new mask*[N];
    for (slicer->init(board, slice);
            !slicer->isDone();
            slicer->nextSlice(slice)) {
        TEST("| 1 2 3 | 4 5 6 | 7 8 9 | " == Visualizer::printSlice(slice));
    }
    delete [] slice;
    // iterate again
    mask **slice2 = new mask*[N];
    SlicerInterface *slicer2 = new HorizontalSlicer();
    for (slicer2->init(board, slice2);
            !slicer2->isDone();
            slicer2->nextSlice(slice2)) {
        TEST("| 1 2 3 | 4 5 6 | 7 8 9 | " == Visualizer::printSlice(slice2));
    }
    delete [] slice2;
    delete [] board;
}

std::string getSliceRepresentation(unsigned int *values) {
    std::string outString = "";
    outString += "| ";
    for (int i = 0; i < N; i++) {
        outString += std::to_string(values[i]);
        outString += " ";
        if ((i + 1) % 3 == 0)
            outString += "| ";
    }
    return outString;
}


/**
 * Vertical Slicer unit tests
 */
void test_VerticalSlicer_Next() {
    mask *board = new mask[N*N];
    getBoard(board);
    SlicerInterface *slicer = new VerticalSlicer();
    unsigned int i = 0;
    mask **slice = new mask*[N];
    for (slicer->init(board, slice);
            !slicer->isDone();
            slicer->nextSlice(slice), i++) {
        unsigned int val = i + 1;
        unsigned int valPtr[9] = {val, val, val, val, val, val, val, val, val};
        std::string expected = getSliceRepresentation(valPtr);
        TEST(expected == Visualizer::printSlice(slice));
    }
    delete [] board;
}


/**
 * Box Slicer unit tests
 */
void test_BoxSlicer_Next() {
    mask *board = new mask[N*N];
    getBoxedBoard(board);
    SlicerInterface *slicer = new BoxSlicer();
    unsigned int i = 0;
    mask **slice = new mask*[N];
    for (slicer->init(board, slice);
            !slicer->isDone();
            slicer->nextSlice(slice), i++) {
        unsigned int val = i + 1;
        unsigned int valPtr[9] = {val, val, val, val, val, val, val, val, val};
        std::string expected = getSliceRepresentation(valPtr);
        TEST(expected == Visualizer::printSlice(slice));
    }
    delete [] slice;
    delete [] board;
}


/**
 * Diagonal Slicer unit tests
 */
void test_DiagonalSlicer_Next() {
    mask *board = new mask[N*N];
    getBoard(board);
    SlicerInterface *slicer = new DiagonalSlicer();
    unsigned int valPtr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::string expected1 = getSliceRepresentation(valPtr);
    unsigned int valPtr2[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::string expected2 = getSliceRepresentation(valPtr2);
    std::string expected[2] = {expected1, expected2};
    unsigned int i = 0;
    mask **slice = new mask*[N];
    for (slicer->init(board, slice);
            !slicer->isDone();
            slicer->nextSlice(slice), i++) {
        TEST(expected[i] == Visualizer::printSlice(slice));
    }
    delete [] slice;
    delete [] board;
}
