#include <string>
#include "parser.hpp"
#include "slicer.hpp"


/**
 * SlicerInterface
 */
mask ** SlicerInterface::init(mask *board) {
    this->reset();
    this->board = board;
    return this->nextSlice();
}

/**
 * HorizontalSlicer
 */
mask ** HorizontalSlicer::nextSlice() {
    mask **slice = new mask*[N];
    for (int i = 0; i < N; i++) {
        slice[i] = &board[i + row*N];
    }
    ++row;
    return slice;
}

bool HorizontalSlicer::isDone() {
    return row > N;
}

void HorizontalSlicer::reset() {
    row = 0;
}


/**
 * VerticalSlicer
 */
mask ** VerticalSlicer::nextSlice() {
    mask **slice = new mask*[N];
    for (int i = 0; i < N; i++) {
        slice[i] = &board[i*N + col];
    }
    ++col;
    return slice;
}

bool VerticalSlicer::isDone() {
    return col > N;
}

void VerticalSlicer::reset() {
    col = 0;
}


/**
 * BoxSlicer
 */
mask ** BoxSlicer::nextSlice() {
    mask **slice = new mask*[N];
    int col = field % 3;
    int row = field / 3;
    int topleft = col * 3 + row * 3 * N;
    for (int i = 0; i < N; i++) {
        int plusCol = i % 3;
        int plusRow = i / 3;
        slice[i] = &board[topleft + plusCol + plusRow * N];
    }
    ++field;
    return slice;
}

bool BoxSlicer::isDone() {
    return field > N;
}

void BoxSlicer::reset() {
    field = 0;
}


/**
 * DiagonalSlicer
 */
mask ** DiagonalSlicer::nextSlice() {
    mask **slice = new mask*[N];
    if (selection == 0) {
        // top left to bottom right
        for (int i = 0; i < N; i++) {
            slice[i] = &board[i*N + i];
        }
    }
    else if (selection == 1) {
        // top right to bottom left
        for (int i = 0; i < N; i++) {
            slice[i] = &board[(i+1)*N - i - 1];
        }
    }
    ++selection;
    return slice;
}

bool DiagonalSlicer::isDone() {
    return selection > 2;
}

void DiagonalSlicer::reset() {
    selection = 0;
}

