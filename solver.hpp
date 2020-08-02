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
         * Helper to transpose a slice
         */
        static mask ** transposeSlice(mask **slice);

        /**
         * Helper to copy sliceFrom to sliceTo
         *
         * Does not copy any values. Just points the elements in sliceTo to
         * the same addresses as sliceFrom.
         */
        static void copySlice(mask **sliceTo, mask **sliceFrom);

        /**
         * Helper to copy values in sliceFrom to sliceTo
         *
         * Does not change any pointers. Just (deep) copies values to which the
         * elements in sliceFrom point to to the addresses of sliceTo.
         */
        static void deepCopySlice(mask **sliceTo, mask **sliceFrom);

        /**
         * Helper to check whether two slices are equal to every bit.
         */
        static bool isSliceEqual(mask **sliceLhs, mask **sliceRhs);

         /**
         * Helper to copy boardFrom to boardTo
         *
         * Does not copy any values. Just points the elements in boardTo to
         * the same addresses as boardFrom.
         */
        static void copyBoard(mask *boardTo, mask *boardFrom);
       
        /**
         * Helper to check whether two boards are equal to every bit.
         *
         * Just checks isEqual for a set of slices representing the
         * complete board (e.g. all horizonal ones).
         */
        static bool isBoardEqual(mask *boardLhs, mask *boardRhs);

        /**
         * Helper to check whether a slice is legally solved.
         *
         * This means that the slide has a number fixed for every
         * field and that no number is duplicated.
         */
        static bool isSliceSolved(mask **slice);
         
        /**
         * Helper to check whether a board is legally solved.
         *
         * Just checks all slices for isSolved().
         */
        static bool isBoardSolved(mask *board);

    private:
        /**
         * Helper to check whether a silce is solved (internal)
         */
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
         * The slice solving magic is happening here.
         *
         * Tries to solve a slice in-place.
         * TODO: Make this private
         */
        virtual void solveSlice(mask **slice) = 0;

        /**
         * Helper to apply a solver to all slices of a board
         */
        void solveAllSlices(SlicerInterface *slicer);
        void solveBoard(mask *board);
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
        bool isInsideList(mask *needle, mask **haystack,
                unsigned int haystackLength);
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
