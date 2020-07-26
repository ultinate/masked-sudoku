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
        virtual mask ** nextSlice() = 0;
        virtual bool isDone() = 0;
        virtual std::string getName() = 0;
        virtual void reset() = 0;
        virtual void printInfo(mask *board);
};


class HorizontalSlicer : public SlicerInterface {
    private:
        mask *board;
        int row;
    public:
        HorizontalSlicer(mask *board);
        ~HorizontalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
        void reset();
};


class VerticalSlicer : public SlicerInterface {
    private:
        mask *board;
        int col;
    public:
        VerticalSlicer(mask *board);
        ~VerticalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
        void reset();
};


class BoxSlicer : public SlicerInterface {
    private:
        mask *board;
        int field;
    public:
        BoxSlicer(mask *board);
        ~BoxSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
        void reset();
};

class DiagonalSlicer : public SlicerInterface {
    private:
        mask *board;
        int selection;
    public:
        DiagonalSlicer(mask *board);
        ~DiagonalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
        void reset();
};

#endif
