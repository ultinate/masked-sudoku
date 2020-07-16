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
        virtual bool solveSlice(mask *slice) = 0;

        /**
         * Helper to transpose a slice
         */
        mask * transposeSlice(mask *slice);

        /**
         * Helper to copy sliceFrom to sliceTo
         */
        void copySlice(mask *sliceTo, mask *sliceFrom);

        /**
         * Helper to check whether two slices are equal to every bit.
         */
        bool isSliceEqual(mask *lhs, mask *rhs);

         /**
         * Helper to copy sliceFrom to sliceTo
         */
        void copyBoard(mask *boardTo, mask *boardFrom);
       
        /**
         * Helper to check whether two boards are equal to every bit.
         *
         * Just checks isEqual for a set of slices representing the
         * complete board (e.g. all horizonal ones).
         */
        bool isBoardEqual(mask *lhs, mask *rhs);

        /**
         * Helper to check whether a slice is legally solved.
         *
         * This means that the slide has a number fixed for every
         * field and that no number is duplicated.
         */
        bool isSolved(mask *slice);
         
        /**
         * Helper to check whether a board is legally solved.
         *
         * Just checks all slices for isSolved().
         */
        bool isSolved(mask **board);

    private:
        bool isSolvedDetail(mask *slice);
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
        bool solveSlice(mask *slice);
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
        bool solveSlice(mask *slice);
        bool eliminate(mask *slice, unsigned int valueToEliminate,
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
