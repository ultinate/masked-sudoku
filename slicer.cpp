#include <string>
#include "parser.hpp"
#include "slicer.hpp"


/**
 * SlicerInterface
 */
void SlicerInterface::init(mask *board, mask **slice) {
    this->reset();
    this->board = board;
    this->nextSlice(slice);
}

/**
 * HorizontalSlicer
 */
void HorizontalSlicer::nextSlice(mask ** slice) {
    for (int i = 0; i < N; i++) {
        slice[i] = &board[i + row*N];
    }
    ++row;
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
void VerticalSlicer::nextSlice(mask ** slice) {
    for (int i = 0; i < N; i++) {
        slice[i] = &board[i*N + col];
    }
    ++col;
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
void BoxSlicer::nextSlice(mask ** slice) {
    int col = field % 3;
    int row = field / 3;
    int topleft = col * 3 + row * 3 * N;
    for (int i = 0; i < N; i++) {
        int plusCol = i % 3;
        int plusRow = i / 3;
        slice[i] = &board[topleft + plusCol + plusRow * N];
    }
    ++field;
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
void DiagonalSlicer::nextSlice(mask ** slice) {
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
}

bool DiagonalSlicer::isDone() {
    return selection > 2;
}

void DiagonalSlicer::reset() {
    selection = 0;
}

