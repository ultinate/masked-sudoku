#include <string>
#include <sstream>
#include "../sTest/test.h"
#include "../slicer.hpp"
#include "../visualizer.hpp"


mask * getBoard() {
    mask *board = new mask[N*N];
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            board[row*N + col] = 0x1 << col;
        }
    }
    return board;
}

mask * getDummyBoard() {
    mask *board = new mask[N*N];
    unsigned int i = 0;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            board[row*N + col] = i;
            ++i;
        }
    }
    return board;
}



/**
 * Horizontal Slicer unit tests
 */
void test_HorizontalSlicer_Next() {
    mask *board = getBoard();
    std::cout << Visualizer::printBoard(board) << std::endl;
    SlicerInterface *slicer = new HorizontalSlicer(board);
    std::cout << slicer->getName() << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next()) {
        std::cout << Visualizer::printSlice(*slice) << std::endl;
        TEST("| 1 2 3 | 4 5 6 | 7 8 9 | " == Visualizer::printSlice(*slice));
    }
}


/**
 * Vertical Slicer unit tests
 */
void test_VerticalSlicer_Next() {
    mask *board = getBoard();
    std::cout << Visualizer::printBoard(board) << std::endl;
    SlicerInterface *slicer = new VerticalSlicer(board);
    std::cout << slicer->getName() << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    unsigned int i = 0;
    std::ostringstream oss;
    std::string expected;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next(), i++) {
        oss << "| " << i << " " << i << " " << i << " ";
        expected = oss.str();
        expected = expected + expected + expected + "| ";
        std::cout << Visualizer::printSlice(*slice) << std::endl;
        TEST(expected == Visualizer::printSlice(*slice));
    }
}


/**
 * Box Slicer unit tests
 */
void test_BoxSlicer_Next() {
    mask *board = getBoard();
    std::cout << Visualizer::printBoard(board) << std::endl;
    SlicerInterface *slicer = new BoxSlicer(board);
    std::cout << slicer->getName() << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    unsigned int i = 0;
    std::ostringstream oss;
    std::string expected;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next(), i++) {
        oss << "| " << i << " " << i << " " << i << " ";
        expected = oss.str();
        expected = expected + expected + expected + "| ";
        std::cout << Visualizer::printSlice(*slice) << std::endl;
        TEST(expected == Visualizer::printSlice(*slice));
    }
}


/**
 * Diagonal Slicer unit tests
 */
void test_DiagonalSlicer_Next() {
    mask *board = getBoard();
    std::cout << Visualizer::printBoard(board) << std::endl;
    SlicerInterface *slicer = new DiagonalSlicer(board);
    std::cout << slicer->getName() << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    mask **slice;
    unsigned int i = 0;
    std::ostringstream oss;
    std::string expected;
    for (slice = sliceIt.Next(); !sliceIt.isDone(); slice = sliceIt.Next(), i++) {
        oss << "| " << i << " " << i << " " << i << " ";
        expected = oss.str();
        expected = expected + expected + expected + "| ";
        std::cout << Visualizer::printSlice(*slice) << std::endl;
        TEST(expected == Visualizer::printSlice(*slice));
    }
}

