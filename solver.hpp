#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "parser.hpp"

/**
 * Solving Sudoku
 *
 * Method: See README.md
 */

/**
 * SolverInterface
 *
 * Have a common interface for all solvers.
 * This class is not intended to be instantiated.
 */
class SolverInterface {
    public:
        SolverInterface() {}
        ~SolverInterface();
        /**
         * The slice solving magic is happening here.
         *
         * Tries to solve a slice in-place.
         */
        virtual void solveSlice(mask **slice) = 0;

        /**
         * Helper to transpose a slice
         */
        mask ** transposeSlice(mask **slice);

        /**
         * Helper to copy sliceFrom to sliceTo
         *
         * Does not copy any values. Just points the elements in sliceTo to
         * the same addresses as sliceFrom.
         */
        void copySlice(mask **sliceTo, mask **sliceFrom);

        /**
         * Helper to check whether two slices are equal to every bit.
         */
        bool isSliceEqual(mask **sliceLhs, mask **sliceRhs);

         /**
         * Helper to copy boardFrom to boardTo
         *
         * Does not copy any values. Just points the elements in boardTo to
         * the same addresses as boardFrom.
         */
        void copyBoard(mask *boardTo, mask *boardFrom);
       
        /**
         * Helper to check whether two boards are equal to every bit.
         *
         * Just checks isEqual for a set of slices representing the
         * complete board (e.g. all horizonal ones).
         */
        bool isBoardEqual(mask *boardLhs, mask *boardRhs);

        /**
         * Helper to check whether a slice is legally solved.
         *
         * This means that the slide has a number fixed for every
         * field and that no number is duplicated.
         */
        bool isSliceSolved(mask **slice);
         
        /**
         * Helper to check whether a board is legally solved.
         *
         * Just checks all slices for isSolved().
         */
        bool isBoardSolved(mask *board);

    private:
        bool isSolvedDetail(mask **slice);
};


/**
 * DetermineSolver
 *
 * If a number only fits into one field of a slice,
 * fix this number in that field.
 */
class DetermineSolver : public SolverInterface {
    public:
        DetermineSolver() {}
        ~DetermineSolver() {}
        /**
         * The slice solving magic is happening here.
         *
         * Tries to solve a slice in-place.
         */
        void solveSlice(mask **slice);
};


/**
 * EliminateSolver
 *
 * If a number is fixed in a slice, eliminate the
 * possibility of it occuring in all other fields of this slice.
 */
class EliminateSolver : public SolverInterface {
    public:
        EliminateSolver() {}
        ~EliminateSolver() {}
        /**
         * The slice solving magic is happening here.
         *
         * Tries to solve a slice in-place.
         */
        void solveSlice(mask **slice);
        void eliminate(mask **slice, unsigned int valueToEliminate,
                          int exceptIndex);
};

/**
 * GuessSolver
 *
 * (not implemented yet)
 * If stuck, guess a single (possible) number and continue with 
 * the other two solvers until solved.
 * If this gives a solved (and valid) board, we're done. Otherwise,
 * revert the guess and guess differently.
 */


#endif
