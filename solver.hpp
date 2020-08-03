#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "parser.hpp"
#include "slicer.hpp"


/**
 * BoardManager
 *
 * Provide general functions to interact with a board and solvers.
 */
class BoardManager {
    public:
        /**
         * Transpose a slice
         *
         * Transpose is used in the sense of matrix operations.
         * Creates a new slice in a new memory location.
         * Users should delete result to avoid memory leaks.
         */
        static mask ** transposeSlice(mask **slice);

        /**
         * (Shallow) Copy sliceFrom to sliceTo
         *
         * Does not copy any values. Points the elements in sliceTo to
         * the same addresses as sliceFrom.
         */
        static void copySlice(mask **sliceTo, mask **sliceFrom);

        /**
         * (Deep) Copy values in sliceFrom to sliceTo
         *
         * Does not change any pointers. Assigns values that sliceFrom
         * points to to elements in sliceTo.
         */
        static void deepCopySlice(mask **sliceTo, mask **sliceFrom);

        /**
         * Check whether two slices point to the same addresses
         *
         * This implies that the two slices also have the same values.
         */
        static bool areSlicesEqual(mask **sliceLhs, mask **sliceRhs);

        /**
         * Check whether two slices have the same values
         *
         * Does not check whether the slices point to the same addresses.
         */
        static bool areSliceValuesEqual(mask **sliceLhs, mask **sliceRhs);

        /**
         * Check whether a slice is legally solved.
         *
         * This means that the slide has a number fixed for every
         * field and that no number is duplicated.
         */
        static bool isSliceSolved(mask **slice);
         
        /**
         * Check whether a board is solved correctly
         *
         * Just checks all slices for isSolved().
         */
        static bool isBoardSolved(mask *board);

        /**
         * Returns true if a mask is contained in mask pointer list
         */
        static bool isInsideList(mask *needle, mask **haystack,
                unsigned int haystackLength);
    private:
        static bool isSolvedDetail(mask **slice);
};


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
         * Returns name of class
         *
         * Mostly used for debugging.
         */
        virtual std::string getName() = 0;

        /**
         * The board solving magic is happening here.
         *
         * Tries to solve a board in-place.
         */
        virtual void solveBoard(mask *board) = 0;
};

/**
 * SliceSolverInterface
 *
 * A type of solver which operates slice-wise
 */
class SliceSolverInterface : public SolverInterface {
    public:
        SliceSolverInterface() {}
        ~SliceSolverInterface() {}
        /**
         * Helper to apply a solver to all slices of a board
         */
        void solveAllSlices(SlicerInterface *slicer);
        void solveBoard(mask *board);

        /**
         * Tries to solve a slice in-place.
         *
         * TODO: Make this private. Note: unit tests
         */
        virtual void solveSlice(mask **slice) = 0;
};


/**
 * DetermineSolver
 *
 * If a number only fits into one field of a slice,
 * fix this number in that field.
 */
class DetermineSolver : public SliceSolverInterface {
    public:
        DetermineSolver() {}
        ~DetermineSolver() {}
        void solveSlice(mask **slice);
        std::string getName() { return "DetermineSolver"; }
};


/**
 * EliminateSolver
 *
 * If a number is fixed in a slice, eliminate the
 * possibility of it occuring in all other fields of this slice.
 */
class EliminateSolver : public SliceSolverInterface {
    public:
        EliminateSolver() {}
        ~EliminateSolver() {}
        void solveSlice(mask **slice);
        std::string getName() { return "EliminateSolver"; }

        // TODO: Make the following methods private (note: unit tests)
        void eliminate(mask **slice, unsigned int valueToEliminate,
                          int exceptIndex);
};


/** OverlapSolver
 *
 * If in a slice _s1_, all possible locations of a
 * certain number lie within another slice _s2_, eliminate that number
 * from other locations inside _s2_ which do not overlap with _s1_.
 */
class OverlapSolver : public SolverInterface {
    public:
        OverlapSolver() {}
        ~OverlapSolver() {}
        void solveSlice(mask **slice);
        void solveBoard(mask *board);
        std::string getName() { return "OverlapSolver"; }
        
        // TODO: Make the following methods private (note: unit tests)
        void solveSlice(mask **sliceOrigin, mask **sliceTarget);
        void solveBoard(mask *board, SlicerInterface *origin,
                SlicerInterface *target);
        void eliminate(mask **slice, unsigned int valueToEliminate,
                mask **exceptMasks, unsigned int exceptMasksLength);
};


/**
 * GuessSolver
 *
 * (not implemented yet)
 * If stuck, guess a single (possible) number and continue with 
 * the other solvers until solved.
 * If this gives a solved (and valid) board, we're done. Otherwise,
 * revert the guess and guess differently.
 */


#endif
