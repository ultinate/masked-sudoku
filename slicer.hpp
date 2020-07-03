#ifndef SLICER_HPP
#define SLICER_HPP

#include <string>
#include "parser.hpp"


/**
 * "Iterator" to loop over slices
 *
 * Note: This is not an iterator that can be used by STL,
 * e.g. std::reduce. It's just something that comes handy for our needs.
 */
template <class T>
class SliceIterator {
    private:
        T *t;
    public:
        SliceIterator(T *t) : t(t) { }
        mask * Next() {
            return t->getSlice();
        }
        bool isDone() {
            return t->isDone();
        }
};

class SlicerInterface {
    public:
        virtual mask * getSlice() = 0;
        virtual bool isDone() = 0;
        virtual SliceIterator<SlicerInterface> createIterator() = 0;
        virtual std::string getName() = 0;
};

class BoxSlicer : public SlicerInterface {
    private:
        mask *board;
        int field;
    public:
        BoxSlicer(mask *board) : board(board) {
            field = 0;
        }
        ~BoxSlicer() {}
        mask * getSlice();
        bool isDone();
        SliceIterator<SlicerInterface> createIterator();
        std::string getName();
};

class DiagonalSlicer : public SlicerInterface {
    private:
        mask *board;
        int selection;
    public:
        DiagonalSlicer(mask *board) : board(board) {
            selection = 0;
        }
        ~DiagonalSlicer() {}
        mask * getSlice();
        bool isDone();
        SliceIterator<SlicerInterface> createIterator();
        std::string getName();
};

class VerticalSlicer : public SlicerInterface {
    private:
        mask *board;
        int col;
    public:
        VerticalSlicer(mask *board) : board(board) {
            col = 0;
        }
        ~VerticalSlicer() {}
        mask * getSlice();
        bool isDone();
        SliceIterator<SlicerInterface> createIterator();
        std::string getName();
};

class HorizontalSlicer : public SlicerInterface {
    private:
        mask *board;
        int row;
    public:
        HorizontalSlicer(mask *board) : board(board) {
            row = 0;
        }
        ~HorizontalSlicer() {}
        mask * getSlice();
        bool isDone();
        SliceIterator<SlicerInterface> createIterator();
        std::string getName();
};

#endif
