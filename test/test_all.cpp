#include "../sTest/test.h"
#include "test_parser.hpp"
#include "test_solver.hpp"
#include "test_slicer.hpp"


/**
 * helper function to modify a board
 */
void modifyBoard(mask *board) {
    board[0] = 0x22;
}


/** 
 * Run all unit tests 
 * 
 * based on light-weight sTest framework, 
 * https://github.com/greg-white/sTest 
 */ 
int runUnitTests() 
{ 
    try 
    {
        // set options
        TEST_WAIT(false);
        TEST_EXIT(true);

        TEST_SECTION("types");
        TEST_GROUP("mask");
        mask m;
        m = 0x0;
        TEST(m == 0);

        TEST_GROUP("slice");
        mask *slice = new mask[N];
        slice[0] = m;
        slice[1] = 0x2;
        slice[9] = 0x1ff;
        TEST(slice[1] == 0x2);
        mask **maskPtr = new mask*[N];
        mask *mPtr = new mask;
        *mPtr = 0x123;
        maskPtr[0] = mPtr;
        maskPtr[1] = &slice[1];
        TEST(*(maskPtr[0]) == *mPtr);
        TEST(*(maskPtr[1]) == slice[1]);
        TEST(*(maskPtr[1]) == 0x2);

        TEST_GROUP("board");
        mask *board = new mask[N*N];
        board[0] = m;
        board[1] = 0x3;
        TEST(board[0] == 0x0);
        TEST(board[1] == 0x3);
        modifyBoard(board);
        TEST(board[0] == 0x22);
        TEST(board[1] == 0x3);

        TEST_SECTION("parser");
        test_Parser();
        test_inputCharToInt(); 
        test_inputCharToMask(); 
        test_getIntFromMask();
        test_isOnlyOneBit();
        test_countBits();
        test_log2();
        test_getMaskFromInt();
        test_getNotMask();

        TEST_SECTION("slicer");
        test_HorizontalSlicer_Next();
        test_VerticalSlicer_Next();
        test_BoxSlicer_Next();
        test_DiagonalSlicer_Next();

        TEST_SECTION("solver");
        test_solvers_combined();
        test_EliminateSolver_horizontal();
        test_EliminateSolver_box();
        test_EliminateSolver_vertical();
        test_EliminateSolver_horizontalVertical();
        test_EliminateSolver_solveSlice();
        test_EliminateSolver_solveSlice_2();
        test_EliminateSolver_solveSliceNoChange();
        test_EliminateSolver_eliminate();
        test_DetermineSolver_solveSlice();
        test_DetermineSolver_solveSliceNoChange();
        test_SolverInterface_transposeSliceTwice();
        test_SolverInterface_transposeSlice();
        test_SolverInterface_transposeSlice_simple();
        test_SolverInterface_copySlice();
        test_SolverInterface_deepCopySlice();
        test_SolverInterface_isSliceEqual_equalAddresses();
        test_SolverInterface_isSliceEqual_equalValues();
        test_SolverInterface_isSliceSolved();
        test_SolverInterface_isBoardSolved_solved();
        test_SolverInterface_isBoardSolved_notSolved();
        test_SolverInterface_isBoardSolved_illegal();

        TEST_GROUP("hex and binary representation"); 
        TEST(0x1 == 1); 
        TEST(0x1 == 0b1); 
        TEST((0x1 << 1) == 2); 
        TEST(0x02 == 2); 
        TEST(0b1 == 1); 
        TEST((0b1 << 1) == 2); 
        TEST((0b10 << 1) == 0b100); 
        TEST((0b1 << 8) == 256); 
        mask b = 0b1;
        TEST((b << 8) == 256); 
        TEST(0b10 == 2);
        TEST(0x1ff == 0b111111111);

        // overflow of "mask" type
        m = 0xffffffff;
        TEST(m == 0xffff);

        m = 0xffff;
        TEST(m == 0xffff);
        m = 0x0;
        TEST (m == 0x0);
        TEST (m == 0b0000000000000000);

        TEST_SUMMARY; 
    } 
    catch (...) 
    { 
        TEST_EXCEPTION; 
    } 
    return 0; 
} 

int main(int argc, char **argv) {
    std::cout << "running unit tests ... ";
    int areUnitTestsPassed = runUnitTests();
    return areUnitTestsPassed;
}
