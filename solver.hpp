#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "board.hpp"
#include "parser.hpp"
#include "slicer.hpp"


/**
 * SolverInterface
 *
 * Have a common interface for all solvers.
 * This class is not intended to be instantiated.
 */
class SolverInterface {
    public:
        BoardManager *manager;
        SolverInterface();
        ~SolverInterface();

        /**
         * Tries to solve a board in-place.
         *
         * The board solving magic is happening here.
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
        void solveAllSlices(SlicerInterface *slicer, mask *board);
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
        
        // TODO: Make the following methods private (note: unit tests)
        void solveSlice(mask **sliceOrigin, mask **sliceTarget);
        void solveBoard(mask *board, SlicerInterface *origin,
                SlicerInterface *target);
        /**
         * Return list of overlapping masks of two slices which are
         * potential locations of number _candidate_;
         * IFF all potential locations of _candinate_ lie within _sliceTarget_.
         */
        int getListOfOverlaps(int candidate, mask **list,
                mask **sliceOrigin, mask **sliceTarget);
        void eliminate(mask **slice, unsigned int valueToEliminate,
                mask **exceptMasks, unsigned int exceptMasksLength);
};


/**
 * GuessSolver
 *
 * If stuck, guess a single (possible) number and continue with 
 * the other solvers until solved.
 * If this gives a solved (and valid) board, we're done. Otherwise,
 * revert the guess and guess differently.
 */

class GuessSolver : public SolverInterface {
    private:
        static const int solverLength = 3;
        SolverInterface *solvers[solverLength];
        static const int maxLoops = N * N * N;
        static const int infoLoops = 1;
        int maxGuessDepth = 0;

    // TODO: Make this inherit from SolverInterface, too.
    public:
        GuessSolver();
        ~GuessSolver();
        void solveBoard(mask*);
        bool solveBoard(mask*, bool);
        bool solveBoard(mask*, bool, int);
        void runSolvers(mask*);
};

#endif
