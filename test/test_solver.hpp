#include <bitset>
#include "../sTest/test.h"
#include "../solver.hpp"


/**
 * solver unit tests
 */
mask * getTestSlice() {
    mask *slice = new mask[N];
    slice[0] = 0b000000001;
    slice[1] = 0b000000011;
    slice[2] = 0b100000100;
    slice[3] = 0b100000000;
    slice[4] = 0b100000001;
    slice[5] = 0b111110111;
    slice[6] = 0b100110001;
    slice[7] = 0b100010001;
    slice[8] = 0b111110011;
    return slice;
}

void test_EliminateSolver_solveSlice() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    bool result = es.solveSlice(slice);
    TEST(result == true);
    TEST(slice[0] == 0b000000001);
    TEST(slice[1] == 0b000000010);
    TEST(slice[2] == 0b000000100);
    TEST(slice[3] == 0b100000000);
    TEST(slice[4] == 0b000000000);
    TEST(slice[5] == 0b011100100);
    TEST(slice[6] == 0b000100000);
    TEST(slice[7] == 0b000010000);
    TEST(slice[8] == 0b011100000);
    /*
    for (int i = 0; i < N; i++) {
        std::bitset<9> b(slice[i]);
        std::cout << "slice " << i << ": " << b << std::endl;
    }
    */
}

void test_EliminateSolver_solveSlice_2() {
    mask *slice = new mask[N];
    slice[0] = 0b111101111;
    slice[1] = 0b111101111;
    slice[2] = 0b111101111;
    slice[3] = 0b111101111;
    slice[4] = 0b111111111;
    slice[5] = 0b111101111;
    slice[6] = 0b111101111;
    slice[7] = 0b111101111;
    slice[8] = 0b111101111;
    EliminateSolver es;
    bool result = es.solveSlice(slice);
    TEST(result == false);
} 

void test_DetermineSolver_solveSlice() {
    mask *slice = new mask[N];
    slice[0] = 0b111101111;
    slice[1] = 0b111101111;
    slice[2] = 0b111101111;
    slice[3] = 0b111101111;
    slice[4] = 0b111111111;
    slice[5] = 0b111101111;
    slice[6] = 0b111101111;
    slice[7] = 0b111101111;
    slice[8] = 0b111101111;
    DetermineSolver ds;
    bool result = ds.solveSlice(slice);
    TEST(result == true);
    TEST(slice[0] == 0b111101111);
    TEST(slice[1] == 0b111101111);
    TEST(slice[2] == 0b111101111);
    TEST(slice[3] == 0b111101111);
    TEST(slice[4] == 0b000010000);
    TEST(slice[5] == 0b111101111);
    TEST(slice[6] == 0b111101111);
    TEST(slice[7] == 0b111101111);
    TEST(slice[8] == 0b111101111);
} 

void test_DetermineSolver_solveSliceNoChange() {
    mask *slice = new mask[N];
    slice[0] = 0b111111111;
    slice[1] = 0b111111111;
    slice[2] = 0b111111111;
    slice[3] = 0b111111111;
    slice[4] = 0b111111111;
    slice[5] = 0b111111111;
    slice[6] = 0b111111111;
    slice[7] = 0b111111111;
    slice[8] = 0b111111111;
    DetermineSolver ds;
    mask *sliceBefore = new mask[N];
    ds.copySlice(sliceBefore, slice);
    bool result = ds.solveSlice(slice);
    TEST(result == false);
    TEST(ds.isSliceEqual(slice, sliceBefore));
}
 
void test_EliminateSolver_solveSliceNoChange() {
    mask *slice = new mask[N];
    slice[0] = 0b111111111;
    slice[1] = 0b111111111;
    slice[2] = 0b111111111;
    slice[3] = 0b111111111;
    slice[4] = 0b111111111;
    slice[5] = 0b111111111;
    slice[6] = 0b111111111;
    slice[7] = 0b111111111;
    slice[8] = 0b111111111;
    EliminateSolver es;
    mask *sliceBefore = new mask[N];
    es.copySlice(sliceBefore, slice);
    bool result = es.solveSlice(slice);
    TEST(result == false);
    TEST(es.isSliceEqual(slice, sliceBefore));
}

void test_EliminateSolver_eliminate() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    bool result = es.eliminate(slice, 1, 0);
    TEST(result == true);
    TEST(slice[0] == 0b000000001);
    TEST(slice[1] == 0b000000010);
    TEST(slice[2] == 0b100000100);
    TEST(slice[3] == 0b100000000);
    TEST(slice[4] == 0b100000000);
    TEST(slice[5] == 0b111110110);
    TEST(slice[6] == 0b100110000);
    TEST(slice[7] == 0b100010000);
    TEST(slice[8] == 0b111110010);
    result = es.eliminate(slice, 4, 0);
    TEST(result == false);
    result = es.eliminate(slice, 3, 5);
    TEST(result == true);
    TEST(slice[2] == 0b100000000);
    TEST(slice[5] == 0b111110110);
}

void test_SolverInterface_transposeSlice() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    mask *sliceT = es.transposeSlice(slice);
    TEST(sliceT[0] == 0b001111111);
    TEST(sliceT[1] == 0b000001001);
    TEST(sliceT[2] == 0b000001001);
    TEST(sliceT[3] == 0b000001101);
    TEST(sliceT[4] == 0b000001111);
    TEST(sliceT[5] == 0b000000000);
    TEST(sliceT[6] == 0b001001000);
    TEST(sliceT[7] == 0b010001001);
}

void test_SolverInterface_transposeSliceTwice() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    mask *sliceT = es.transposeSlice(slice);
    mask *sliceTT = es.transposeSlice(sliceT);
    TEST(es.isSliceEqual(slice, sliceTT));
}

void test_SolverInterface_transposeSlice_simple() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    slice[0] = 0b111101111;
    slice[1] = 0b111101111;
    slice[2] = 0b111101111;
    slice[3] = 0b111101111;
    slice[4] = 0b111111111;
    slice[5] = 0b111101111;
    slice[6] = 0b111101111;
    slice[7] = 0b111101111;
    slice[8] = 0b111101111;
    mask *sliceT = es.transposeSlice(slice);
    TEST(sliceT[0] == 0b111111111);
    TEST(sliceT[1] == 0b111111111);
    TEST(sliceT[2] == 0b111111111);
    TEST(sliceT[3] == 0b111111111);
    TEST(sliceT[4] == 0b000010000);
    TEST(sliceT[5] == 0b111111111);
    TEST(sliceT[6] == 0b111111111);
    TEST(sliceT[7] == 0b111111111);
    TEST(sliceT[8] == 0b111111111);
} 

void test_SolverInterface_copySlice() {
    mask *slice = new mask[N];
    slice[0] = 0b111111111;
    EliminateSolver es;
    mask *sliceCopy = new mask[N];
    es.copySlice(sliceCopy, slice);
    TEST(sliceCopy[0] == 0b111111111);
    TEST(sliceCopy[1] == 0b000000000);
    TEST(es.isSliceEqual(slice, sliceCopy));
}

void test_SolverInterface_isSliceEqual() {
    mask *slice = new mask[N];
    slice[0] = 0b111111111;
    mask *slice2 = new mask[N];
    slice2[0] = 0b111111111;
    EliminateSolver es;
    TEST(es.isSliceEqual(slice, slice2));
    slice2[0] = 0b111111110;
    TEST(!es.isSliceEqual(slice, slice2));
}

void test_SolverInterface_isSolved() {
    mask *slice = getTestSlice();
    EliminateSolver es;
    slice[0] = 0b100000100;
    slice[1] = 0b010000000;
    slice[2] = 0b001000000;
    slice[3] = 0b000100000;
    slice[4] = 0b000010000;
    slice[5] = 0b000001000;
    slice[6] = 0b000000100;
    slice[7] = 0b000000010;
    slice[8] = 0b000000001;
    // not solved
    TEST(!es.isSolved(slice));
    slice[0] = 0b100000000;
    // correctly solved
    TEST(es.isSolved(slice));
    slice[0] = 0b000000001;
    // solved, but illegal
    TEST(!es.isSolved(slice));
} 

