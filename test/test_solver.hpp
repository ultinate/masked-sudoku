#include <bitset>
#include "../sTest/test.h"
#include "../parser.hpp"
#include "../slicer.hpp"
#include "../solver.hpp"
#include "../visualizer.hpp"


/**
 * solver unit tests
 */
mask ** getTestSlice() {
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
    mask **slicePtr = new mask*[N];
    for (int i = 0; i < N; i++) {
        slicePtr[i] = &slice[i];
    }
    return slicePtr;
}

mask ** getTestSliceRow() {
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
    mask **slicePtr = new mask*[N];
    for (int i = 0; i < N; i++) {
        slicePtr[i] = &slice[i];
    }
    return slicePtr;
}

mask ** getTestSliceFull() {
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
    mask **slicePtr = new mask*[N];
    for (int i = 0; i < N; i++) {
        slicePtr[i] = &slice[i];
    }
    return slicePtr;
}

void test_OverlapSolver_boxToColumn() {
    std::string input =
        "........."
        "2........"
        "3........"
        "4........"
        "5........"
        "6........"
        "7......89"
        ".34......"
        ".56......";
    Parser *p = new Parser(input);
    p->parse();
    mask *board = p->getBoard();
    std::cout << Visualizer::printBoardMini(board) << std::endl;

    SolverInterface *solver = new OverlapSolver();
    solver->solveBoard(board);
    std::string expected =
        "1........"
        "2........"
        "3........"
        "4........"
        "5........"
        "6........"
        "7......89"
        ".34......"
        ".56......";
    TEST(expected == Visualizer::printBoardMini(board));
    std::cout << Visualizer::printBoardMini(board) << std::endl;

    // TODO: Check the following in isolation
    /*
    OverlapSolver *solver = new OverlapSolver();
    SlicerInterface *origin;
    SlicerInterface *target;
    // Box-to-vertical
    origin = new BoxSlicer(board);
    target = new VerticalSlicer(board);
    solver->solveBoard(board, origin, target);
    */
}

void test_EliminateSolver_box() {
    std::string input =
        "123......"
        "456......"
        "78......."
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    Parser *p = new Parser(input);
    p->parse();
    mask *board = p->getBoard();
    SlicerInterface *slicer = new BoxSlicer(board);
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer);
    std::string expected =
        "123......"
        "456......"
        "789......"
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    TEST(expected == Visualizer::printBoardMini(board));
    // std::cout << Visualizer::printBoardMini(board) << std::endl;
}

void test_solvers_combined() {
    std::string input =
        "12...69.."
        "45...9..."
        "78......."
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    Parser *p = new Parser(input);
    p->parse();
    mask *board = p->getBoard();

    SlicerInterface *slicer = new HorizontalSlicer(board);
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer);
    
    slicer = new BoxSlicer(board);
    solver->solveAllSlices(slicer);

    solver = new DetermineSolver();
    slicer = new BoxSlicer(board);
    solver->solveAllSlices(slicer);

    solver = new EliminateSolver();
    slicer = new BoxSlicer(board);
    solver->solveAllSlices(slicer);

    std::string expected =
        "123..69.."
        "456..9..."
        "789......"
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    TEST(expected == Visualizer::printBoardMini(board));
}

void test_EliminateSolver_horizontal() {
    std::string inputString =
        "12345678."
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    Parser *p = new Parser(inputString);
    p->parse();
    mask *board = p->getBoard();
    SlicerInterface *slicer = new HorizontalSlicer(board);
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer);
    std::string expected =
        "123456789"
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    TEST(expected == Visualizer::printBoardMini(board));
}

void test_EliminateSolver_vertical() {
    std::string inputString =
        "........."
        "........2"
        "........3"
        "........4"
        "........5"
        "........6"
        "........7"
        "........8"
        "........9";
    Parser *p = new Parser(inputString);
    p->parse();
    mask *board = p->getBoard();
    SlicerInterface *slicer = new VerticalSlicer(board);
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer);
    std::string expected =
        "........1"
        "........2"
        "........3"
        "........4"
        "........5"
        "........6"
        "........7"
        "........8"
        "........9";
    TEST(expected == Visualizer::printBoardMini(board));
}

void test_EliminateSolver_horizontalVertical() {
    std::string inputString =
        "6........"
        "........2"
        "........3"
        "........4"
        "........5"
        "1........"
        "........7"
        "........8"
        "........9";
    Parser *p = new Parser(inputString);
    p->parse();
    mask *board = p->getBoard();

    SlicerInterface *slicer = new HorizontalSlicer(board);
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer);

    slicer = new VerticalSlicer(board);
    solver->solveAllSlices(slicer);

    std::string expected =
        "6.......1"
        "........2"
        "........3"
        "........4"
        "........5"
        "1.......6"
        "........7"
        "........8"
        "........9";
    TEST(expected == Visualizer::printBoardMini(board));
}

void test_EliminateSolver_solveSlice() {
    mask **slice = getTestSlice();

    // have we got the slice correctly?
    TEST(*slice[0] = 0b000000001);
    TEST(*slice[1] = 0b000000011);
    TEST(*slice[2] = 0b100000100);
    TEST(*slice[3] = 0b100000000);
    TEST(*slice[4] = 0b100000001);
    TEST(*slice[5] = 0b111110111);
    TEST(*slice[6] = 0b100110001);
    TEST(*slice[7] = 0b100010001);
    TEST(*slice[8] = 0b111110011);

    // let's solve it
    EliminateSolver es;
    es.solveSlice(slice);
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000010);
    TEST(*slice[2] == 0b000000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b000000000);
    TEST(*slice[5] == 0b011100100);
    TEST(*slice[6] == 0b000100000);
    TEST(*slice[7] == 0b000010000);
    TEST(*slice[8] == 0b011100000);
    /*
    for (int i = 0; i < N; i++) {
        std::bitset<9> b(slice[i]);
        std::cout << "slice " << i << ": " << b << std::endl;
    }
    */
}

void test_EliminateSolver_solveSlice_2() {
    mask **slice = getTestSliceRow();
    EliminateSolver es;
    es.solveSlice(slice);
    // no change
    TEST(*slice[0] == 0b111101111);
    TEST(*slice[1] == 0b111101111);
    TEST(*slice[2] == 0b111101111);
    TEST(*slice[3] == 0b111101111);
    TEST(*slice[4] == 0b111111111);
    TEST(*slice[5] == 0b111101111);
    TEST(*slice[6] == 0b111101111);
    TEST(*slice[7] == 0b111101111);
    TEST(*slice[8] == 0b111101111);
} 

void test_DetermineSolver_solveSlice() {
    mask **slice = getTestSliceRow();
    DetermineSolver ds;
    ds.solveSlice(slice);
    TEST(*slice[0] == 0b111101111);
    TEST(*slice[1] == 0b111101111);
    TEST(*slice[2] == 0b111101111);
    TEST(*slice[3] == 0b111101111);
    TEST(*slice[4] == 0b000010000);  // changed
    TEST(*slice[5] == 0b111101111);
    TEST(*slice[6] == 0b111101111);
    TEST(*slice[7] == 0b111101111);
    TEST(*slice[8] == 0b111101111);
} 

void test_DetermineSolver_solveSliceNoChange() {
    mask **slice = getTestSliceFull();
    mask **slicePtr = slice;
    mask **sliceBefore = getTestSliceRow();  // initialize with any values
    BoardManager::deepCopySlice(sliceBefore, slicePtr);
    DetermineSolver ds;
    ds.solveSlice(slicePtr);
    TEST(BoardManager::areSliceValuesEqual(slicePtr, sliceBefore));
}
 
void test_EliminateSolver_solveSliceNoChange() {
    mask **slice = getTestSliceFull();
    mask **slicePtr = slice;
    mask **sliceBefore = getTestSliceRow();  // initialize with any values
    BoardManager::deepCopySlice(sliceBefore, slicePtr);
    EliminateSolver es;
    es.solveSlice(slicePtr);
    TEST(BoardManager::areSliceValuesEqual(slicePtr, sliceBefore));
}

void test_EliminateSolver_eliminate() {
    mask **slice = getTestSlice();
    EliminateSolver es;
    es.eliminate(slice, 1, 0);
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000010);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000000);
    TEST(*slice[5] == 0b111110110);
    TEST(*slice[6] == 0b100110000);
    TEST(*slice[7] == 0b100010000);
    TEST(*slice[8] == 0b111110010);

    // eliminating number "4" will not change anything
    es.eliminate(slice, 4, 0);
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000010);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000000);
    TEST(*slice[5] == 0b111110110);
    TEST(*slice[6] == 0b100110000);
    TEST(*slice[7] == 0b100010000);
    TEST(*slice[8] == 0b111110010);

    // eliminating number "5" will change two boxes
    es.eliminate(slice, 3, 5);
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000010);
    TEST(*slice[2] == 0b100000000); // changed
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000000);
    TEST(*slice[5] == 0b111110110); // changed
    TEST(*slice[6] == 0b100110000);
    TEST(*slice[7] == 0b100010000);
    TEST(*slice[8] == 0b111110010);
}

void test_SolverInterface_transposeSlice() {
    mask **slice = getTestSlice();
    mask **sliceT = BoardManager::transposeSlice(slice);
    TEST(*sliceT[0] == 0b001111111);
    TEST(*sliceT[1] == 0b000001001);
    TEST(*sliceT[2] == 0b000001001);
    TEST(*sliceT[3] == 0b000001101);
    TEST(*sliceT[4] == 0b000001111);
    TEST(*sliceT[5] == 0b000000000);
    TEST(*sliceT[6] == 0b001001000);
    TEST(*sliceT[7] == 0b010001001);
}

void test_SolverInterface_transposeSliceTwice() {
    mask **slice = getTestSlice();
    mask **sliceT = BoardManager::transposeSlice(slice);
    mask **sliceTT = BoardManager::transposeSlice(sliceT);
    TEST(BoardManager::areSliceValuesEqual(slice, sliceTT));
    TEST(!(BoardManager::areSlicesEqual(slice, sliceTT)));
}

void test_SolverInterface_transposeSlice_simple() {
    mask **slice = getTestSlice();
    *slice[0] = 0b111101111;
    *slice[1] = 0b111101111;
    *slice[2] = 0b111101111;
    *slice[3] = 0b111101111;
    *slice[4] = 0b111111111;
    *slice[5] = 0b111101111;
    *slice[6] = 0b111101111;
    *slice[7] = 0b111101111;
    *slice[8] = 0b111101111;
    mask **sliceT = BoardManager::transposeSlice(slice);
    TEST(*sliceT[0] == 0b111111111);
    TEST(*sliceT[1] == 0b111111111);
    TEST(*sliceT[2] == 0b111111111);
    TEST(*sliceT[3] == 0b111111111);
    TEST(*sliceT[4] == 0b000010000);
    TEST(*sliceT[5] == 0b111111111);
    TEST(*sliceT[6] == 0b111111111);
    TEST(*sliceT[7] == 0b111111111);
    TEST(*sliceT[8] == 0b111111111);
} 

void test_SolverInterface_copySlice() {
    mask **slice = getTestSliceFull();
    TEST(*slice[0] == 0b111111111);
    TEST(*slice[1] == 0b111111111);
    TEST(*slice[2] == 0b111111111);
    TEST(*slice[3] == 0b111111111);
    TEST(*slice[4] == 0b111111111);
    TEST(*slice[5] == 0b111111111);
    TEST(*slice[6] == 0b111111111);
    TEST(*slice[7] == 0b111111111);
    TEST(*slice[8] == 0b111111111);

    mask **sliceCopy = new mask*[N];
    BoardManager::copySlice(sliceCopy, slice);
    for (int i = 0; i < N; i++) {
        TEST(*sliceCopy[i] == 0b111111111);
    }
    TEST(BoardManager::areSliceValuesEqual(slice, sliceCopy));
    TEST(BoardManager::areSlicesEqual(slice, sliceCopy));
}

void test_SolverInterface_deepCopySlice() {
    mask **slice = getTestSliceRow();
    mask **sliceCopy = new mask*[N];
    for (int i = 0; i < N; i++) {
        sliceCopy[i] = new mask;
    }
    BoardManager::deepCopySlice(sliceCopy, slice);
    // values must be equal
    TEST(*sliceCopy[0] == 0b111101111);
    TEST(*sliceCopy[1] == 0b111101111);
    TEST(*sliceCopy[2] == 0b111101111);
    TEST(*sliceCopy[3] == 0b111101111);
    TEST(*sliceCopy[4] == 0b111111111);
    TEST(*sliceCopy[5] == 0b111101111);
    TEST(*sliceCopy[6] == 0b111101111);
    TEST(*sliceCopy[7] == 0b111101111);
    TEST(*sliceCopy[8] == 0b111101111);
    // pointers must not be equal
    for (int i = 0; i < N; i++) {
        TEST(sliceCopy[i] != slice[i]);
    }
    TEST(BoardManager::areSliceValuesEqual(slice, sliceCopy));
    TEST(!(BoardManager::areSlicesEqual(slice, sliceCopy)));
}

void test_SolverInterface_areSlicesEqual_equalAddresses() {
    mask **slice = getTestSliceFull();
    mask **sliceCopy = new mask*[N];
    BoardManager::copySlice(sliceCopy, slice);
    TEST(BoardManager::areSlicesEqual(slice, sliceCopy));
    TEST(BoardManager::areSliceValuesEqual(slice, sliceCopy));
}

void test_SolverInterface_areSlicesEqual_equalValues() {
    mask **slice = getTestSliceFull();
    mask **sliceCopy = getTestSliceFull();

    TEST(!(BoardManager::areSlicesEqual(slice, sliceCopy)));
    TEST(BoardManager::areSliceValuesEqual(slice, sliceCopy));
    *sliceCopy[0] = 0b111111110;
    TEST(!(BoardManager::areSlicesEqual(slice, sliceCopy)));
    TEST(!(BoardManager::areSliceValuesEqual(slice, sliceCopy)));
}

void test_SolverInterface_isSliceSolved() {
    mask *slice = new mask[N];
    slice[0] = 0b100000100;
    slice[1] = 0b010000000;
    slice[2] = 0b001000000;
    slice[3] = 0b000100000;
    slice[4] = 0b000010000;
    slice[5] = 0b000001000;
    slice[6] = 0b000000100;
    slice[7] = 0b000000010;
    slice[8] = 0b000000001;
    mask **slicePtr = new mask*[N];
    for (int i = 0; i < N; i++) {
        slicePtr[i] = &slice[i];
    }

    // not solved
    TEST(!BoardManager::isSliceSolved(slicePtr));
    
    // correctly solved
    slice[0] = 0b100000000;
    TEST(BoardManager::isSliceSolved(slicePtr));

    // solved, but illegal
    slice[0] = 0b000000001;
    TEST(!BoardManager::isSliceSolved(slicePtr));
}

void test_SolverInterface_isBoardSolved_solved() {
     std::string inputString = 
        "491786325"
        "735294816"
        "826531479"
        "368172594"
        "142359687"
        "579468132"
        "954823761"
        "287615943"
        "613947258";
    Parser *p = new Parser(inputString);
    int parseResult = p->parse();
    TEST(parseResult == 0);
    mask *board = p->getBoard();
    TEST(BoardManager::isBoardSolved(board));
}

void test_SolverInterface_isBoardSolved_notSolved() {
     std::string inputString = 
        "..37..4.6"
        "...3.5..."
        "92..6.8.."
        ".5...2..4"
        "..1...6.."
        "4..98..7."
        ".3..71..8"
        "..4...7.2"
        "..6...1.3";
    Parser *p = new Parser(inputString);
    int parseResult = p->parse();
    TEST(parseResult == 0);
    mask *board = p->getBoard();
    TEST(!BoardManager::isBoardSolved(board));
}

void test_SolverInterface_isBoardSolved_illegal() {
     std::string inputString = 
        "999999999"
        "735294816"
        "826531479"
        "368172594"
        "142359687"
        "579468132"
        "954823761"
        "287615943"
        "613947258";
    Parser *p = new Parser(inputString);
    int parseResult = p->parse();
    TEST(parseResult == 0);
    mask *board = p->getBoard();
    TEST(!BoardManager::isBoardSolved(board));
}

void test_BoardManager_isInsideList() {
    mask **slice = getTestSlice();
    mask *element = slice[3];
    TEST(BoardManager::isInsideList(element, slice, N));
    element = new mask;
    TEST(!BoardManager::isInsideList(element, slice, N));

    slice = getTestSlice();
    mask **sliceOther = getTestSlice();
    element = slice[3];
    TEST(!BoardManager::isInsideList(element, sliceOther, N));
}

