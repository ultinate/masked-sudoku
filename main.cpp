#include <iostream>
#include <string>
#include <cstring>
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
        std::string getName() { return "Box slices"; }
        mask * getSlice() {
            int col = field % 3;
            int row = field / 3;
            int topleft = col * 3 + row * 3 * N;
            mask *slice = new mask[N];
            for (int i = 0; i < N; i++) {
                int plusCol = i % 3;
                int plusRow = i / 3;
                slice[i] = board[topleft + plusCol + plusRow * N];
            }
            ++field;
            return slice;
        }
        bool isDone() {
            return field > N;
        }
        SliceIterator<SlicerInterface> createIterator() {
            return SliceIterator<SlicerInterface>(this);
        }
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
        std::string getName() { return "Diagonal slices"; }
        mask * getSlice() {
            mask *slice = new mask[N];
            if (selection == 0) {
                // top left to bottom right
                for (int i = 0; i < N; i++) {
                    slice[i] = board[i*N + i];
                }
            }
            else if (selection == 1) {
                // top right to bottom left
                for (int i = 0; i < N; i++) {
                    slice[i] = board[(i+1)*N - i - 1];
                }
            }
            ++selection;
            return slice;
        }
        bool isDone() {
            return selection > 2;
        }
        SliceIterator<SlicerInterface> createIterator() {
            return SliceIterator<SlicerInterface>(this);
        }
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
        std::string getName() { return "Vertical slices"; }
        mask * getSlice() {
            mask *slice = new mask[N];
            for (int i = 0; i < N; i++) {
                slice[i] = board[i*N + col];
            }
            ++col;
            return slice;
        }
        bool isDone() {
            return col > N;
        }
        SliceIterator<SlicerInterface> createIterator() {
            return SliceIterator<SlicerInterface>(this);
        }
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
        std::string getName() { return "Horizontal slices"; }
        mask * getSlice() {
            mask *slice = new mask[N];
            for (int i = 0; i < N; i++) {
                slice[i] = board[i + row*N];
            }
            ++row;
            return slice;
        }
        bool isDone() {
            return row > N;
        }
        SliceIterator<SlicerInterface> createIterator() {
            return SliceIterator<SlicerInterface>(this);
        }
};

class ReducerInterface {
    public:
        virtual bool reduceSlice(mask *slice) = 0;
};

class Reducer
    : public ReducerInterface {
    public:
        Reducer() {}
        ~Reducer() {}
        bool reduceSlice(mask *slice) {
            return true;
        }
};

/**
 * Visualizer
 */
class Visualizer {
    public:
        Visualizer() {
        }
        static std::string maskToStr(mask m) {
            unsigned int number = Parser::getIntFromMask(m);
            if (number > 0)
                return std::to_string(number);
            else
                return " ";
        }
        static std::string printSlice(mask *slice) {
            std::string outString = "";
            outString += "| ";
            for (int i = 0; i < N; i++) {
                outString += maskToStr(slice[i]);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
            }
            return outString;
        }
        static std::string printBoard(mask *board) {
            std::string outString = "";
            outString += "-------------------------\n";
            for (int i = 0; i < N*N; i++) {
                if (i % N == 0)
                    outString += "| ";
                outString += maskToStr(board[i]);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
                if ((i + 1) % N == 0)
                    outString += "\n";
                if ((i + 1) % (N*3) == 0)
                    outString += "-------------------------\n";
            }
            return outString;
        }
};

void printSlices(SlicerInterface *slicer) {
    std::cout << slicer->getName() << ": " << std::endl;
    SliceIterator<SlicerInterface> sliceIt = slicer->createIterator();
    for (mask *slice = sliceIt.Next();
            !sliceIt.isDone();
            slice = sliceIt.Next()) {
        std::cout << Visualizer::printSlice(slice) << std::endl;
    }
}

int main(int argc, char **argv) {
    std::cout << "sudoku solver, v 0.0.2" << std::endl;
    std::string inputString;
    std::string tmpString;
    while (std::getline(std::cin, tmpString))
    {
      inputString += tmpString;
    } 

    Parser * p;
    p = new Parser(inputString);
    int parseResult = p->parse();
    if (parseResult != 0) {
        std::cout << "Parsing not successful. Terminating." << std::endl;
        return parseResult;
    }
    mask *board = p->unsolvedBoard;

    std::cout << "Parsed result: " << std::endl;
    std::cout << Visualizer::printBoard(board) << std::endl;

    SlicerInterface **slicer;
    slicer[0] = new HorizontalSlicer(board);
    slicer[1] = new VerticalSlicer(board);
    slicer[2] = new BoxSlicer(board);
    slicer[3] = new DiagonalSlicer(board);
    for (int i = 0; i < 4; i++) {
        printSlices(slicer[i]);
    }
 
    ReducerInterface * r;
    r = new Reducer();
    std::cout << "Reducer info: " << r << std::endl;

    return 0;
}

