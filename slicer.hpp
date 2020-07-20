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
};


class HorizontalSlicer : public SlicerInterface {
    private:
        mask *board;
        int row;
    public:
        HorizontalSlicer(mask *board) : board(board) {
            row = 0;
            //std::cout << getName() << ". Board at: " << this->board << ". [0]: " << board[0] << std::endl;
        }
        ~HorizontalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
};


class VerticalSlicer : public SlicerInterface {
    private:
        mask *board;
        int col;
    public:
        VerticalSlicer(mask *board) : board(board) {
            col = 0;
            //std::cout << getName() << ". Board at: " << this->board << ". [0]: " << board[0] << std::endl;
        }
        ~VerticalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
};


class BoxSlicer : public SlicerInterface {
    private:
        mask *board;
        int field;
    public:
        BoxSlicer(mask *board) : board(board) {
            field = 0;
            //std::cout << getName() << ". Board at: " << this->board << ". [0]: " << board[0] << std::endl;
        }
        ~BoxSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
};

class DiagonalSlicer : public SlicerInterface {
    private:
        mask *board;
        int selection;
    public:
        DiagonalSlicer(mask *board) : board(board) {
            selection = 0;
            //std::cout << getName() << ". Board at: " << this->board << ". [0]: " << board[0] << std::endl;
        }
        ~DiagonalSlicer() {}
        mask ** nextSlice();
        bool isDone();
        std::string getName();
};

#endif
