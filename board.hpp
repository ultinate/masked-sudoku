#ifndef BOARD_HPP
#define BOARD_HPP

#include "parser.hpp"
#include "slicer.hpp"


/**
 * BoardManager
 *
 * Provide general functions to interact with a board and solvers.
 */
class BoardManager {
    private:
        static bool isSolvedDetail(mask **slice);

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
         * (Deep) Copy values in boardFrom to boardTo
         */
        static void deepCopyBoard(mask *boardTo, mask *boardFrom);

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
         * Checks whether a slice is legal
         *
         * This means that the slice does not contain any duplicate numbers.
         * This method does not care if the slice is completely filled yet.
         */
        static bool isSliceLegal(mask **slice);
         
        /**
         * Check whether a board is solved correctly
         *
         * Just checks all slices for isSliceSolved().
         */
        static bool isBoardSolved(mask *board);

        /**
         * Checks whether a board is legal
         *
         * Just checks all slices for isSliceLegal().
         */
        static bool isBoardLegal(mask *board);

        /**
         * Returns true if a mask is contained in mask pointer list
         */
        static bool isInsideList(mask *needle, mask **haystack,
                unsigned int haystackLength);
};

#endif
