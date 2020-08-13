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
class SlicerInterface {
    public:
        mask *board;
        virtual void init(mask *board, mask **slice);
        virtual void nextSlice(mask **slice) = 0;
        virtual bool isDone() = 0;
        virtual void reset() = 0;
};


class HorizontalSlicer : public SlicerInterface {
    private:
        int row;
    public:
        HorizontalSlicer() {}
        ~HorizontalSlicer() {}
        void nextSlice(mask **slice);
        bool isDone();
        void reset();
};


class VerticalSlicer : public SlicerInterface {
    private:
        int col;
    public:
        VerticalSlicer() {}
        ~VerticalSlicer() {}
        void nextSlice(mask **slice);
        bool isDone();
        void reset();
};


class BoxSlicer : public SlicerInterface {
    private:
        int field;
    public:
        BoxSlicer() {}
        ~BoxSlicer() {}
        void nextSlice(mask **slice);
        bool isDone();
        void reset();
};

class DiagonalSlicer : public SlicerInterface {
    private:
        int selection;
    public:
        DiagonalSlicer() {}
        ~DiagonalSlicer() {}
        void nextSlice(mask **slice);
        bool isDone();
        void reset();
};

#endif
