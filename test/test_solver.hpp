#include <bitset>
#include "../sTest/test.h"
#include "../board.hpp"
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

mask * getTestBoard() {
    std::string input =
        "123......"
        "456......"
        "789......"
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();
    return board;
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
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();
    SolverInterface *solver = new OverlapSolver;
    solver->solveBoard(board);
    std::string expected =
        "........."
        "2........"
        "3........"
        "4........"
        "5........"
        "6........"
        "7......89"
        ".34......"
        ".56......";
    TEST(expected == Visualizer::printBoardMini(board));
    delete parser;
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
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();
    SlicerInterface *slicer = new BoxSlicer();
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer, board);
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
    delete parser;
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
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();

    SlicerInterface *slicer = new HorizontalSlicer();
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer, board);
   
    slicer = new BoxSlicer();
    solver->solveAllSlices(slicer, board);

    solver = new DetermineSolver();
    slicer = new BoxSlicer();
    solver->solveAllSlices(slicer, board);

    solver = new EliminateSolver();
    slicer = new BoxSlicer();
    solver->solveAllSlices(slicer, board);

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
    delete parser;
}

void test_EliminateSolver_horizontal() {
    std::string input =
        "12345678."
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        "........."
        ".........";
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();

    SlicerInterface *slicer = new HorizontalSlicer();
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer, board);
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
    delete parser;
}

void test_EliminateSolver_vertical() {
    std::string input =
        "........."
        "........2"
        "........3"
        "........4"
        "........5"
        "........6"
        "........7"
        "........8"
        "........9";
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();

    SlicerInterface *slicer = new VerticalSlicer();
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer, board);
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
    std::string input =
        "6........"
        "........2"
        "........3"
        "........4"
        "........5"
        "1........"
        "........7"
        "........8"
        "........9";
    Parser *parser = new Parser(input);
    parser->parse();
    mask *board = parser->getBoard();

    SlicerInterface *slicer = new HorizontalSlicer();
    SliceSolverInterface *solver = new EliminateSolver();
    solver->solveAllSlices(slicer, board);

    slicer = new VerticalSlicer();
    solver->solveAllSlices(slicer, board);

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
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000011);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000001);
    TEST(*slice[5] == 0b111110111);
    TEST(*slice[6] == 0b100110001);
    TEST(*slice[7] == 0b100010001);
    TEST(*slice[8] == 0b111110011);

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
    DetermineSolver ds;
    ds.manager->deepCopySlice(sliceBefore, slicePtr);
    ds.solveSlice(slicePtr);
    TEST(ds.manager->areSliceValuesEqual(slicePtr, sliceBefore));
}

void test_EliminateSolver_solveSliceNoChange() {
    mask **slice = getTestSliceFull();
    mask **slicePtr = slice;
    mask **sliceBefore = getTestSliceRow();  // initialize with any values
    EliminateSolver es;
    es.manager->BoardManager::deepCopySlice(sliceBefore, slicePtr);
    es.solveSlice(slicePtr);
    TEST(es.manager->areSliceValuesEqual(slicePtr, sliceBefore));
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
    BoardManager manager;
    mask **sliceT = manager.transposeSlice(slice);
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
    BoardManager manager;
    mask **sliceT = manager.transposeSlice(slice);
    mask **sliceTT = manager.transposeSlice(sliceT);
    TEST(manager.areSliceValuesEqual(slice, sliceTT));
    TEST(!(manager.areSlicesEqual(slice, sliceTT)));
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
    BoardManager manager;
    mask **sliceT = manager.transposeSlice(slice);
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
    BoardManager manager;
    manager.copySlice(sliceCopy, slice);
    for (int i = 0; i < N; i++) {
        TEST(*sliceCopy[i] == 0b111111111);
    }
    TEST(manager.areSliceValuesEqual(slice, sliceCopy));
    TEST(manager.areSlicesEqual(slice, sliceCopy));
}

void test_SolverInterface_deepCopySlice() {
    mask **slice = getTestSliceRow();
    mask **sliceCopy = new mask*[N];
    for (int i = 0; i < N; i++) {
        sliceCopy[i] = new mask;
    }
    BoardManager manager;
    manager.deepCopySlice(sliceCopy, slice);
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
    TEST(manager.areSliceValuesEqual(slice, sliceCopy));
    TEST(!(manager.areSlicesEqual(slice, sliceCopy)));
}

void test_SolverInterface_deepCopyBoard() {
    mask *board = getTestBoard();
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

    mask *boardCopy = new mask[N*N];
    for (int i = 0; i < N*N; i++) {
        boardCopy[i] = 0x1;
    }
    BoardManager manager;
    manager.deepCopyBoard(boardCopy, board);

    // values must be equal
    TEST(expected == Visualizer::printBoardMini(boardCopy));
    TEST(boardCopy[0] == 0b000000001);
    TEST(boardCopy[1] == 0b000000010);
    TEST(boardCopy[2] == 0b000000100);
    TEST(boardCopy[3] == 0b111111111);
    TEST(boardCopy[4] == 0b111111111);
    TEST(boardCopy[5] == 0b111111111);
    TEST(boardCopy[6] == 0b111111111);
    TEST(boardCopy[7] == 0b111111111);
    TEST(boardCopy[8] == 0b111111111);
    TEST(boardCopy[9] == 0b000001000);
    TEST(boardCopy[80] == 0b111111111);

    // pointers must not be equal
    TEST(boardCopy != board);
}

void test_SolverInterface_areSlicesEqual_equalAddresses() {
    mask **slice = getTestSliceFull();
    mask **sliceCopy = new mask*[N];
    BoardManager manager;
    manager.copySlice(sliceCopy, slice);
    TEST(manager.areSlicesEqual(slice, sliceCopy));
    TEST(manager.areSliceValuesEqual(slice, sliceCopy));
}

void test_SolverInterface_areSlicesEqual_equalValues() {
    mask **slice = getTestSliceFull();
    mask **sliceCopy = getTestSliceFull();

    BoardManager manager;
    TEST(!(manager.areSlicesEqual(slice, sliceCopy)));
    TEST(manager.areSliceValuesEqual(slice, sliceCopy));
    *sliceCopy[0] = 0b111111110;
    TEST(!(manager.areSlicesEqual(slice, sliceCopy)));
    TEST(!(manager.areSliceValuesEqual(slice, sliceCopy)));
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

    // not solved yet
    BoardManager manager;
    TEST(!manager.isSliceSolved(slicePtr));
    TEST(manager.isSliceLegal(slicePtr));
   
    // correctly solved
    slice[0] = 0b100000000;
    TEST(manager.isSliceSolved(slicePtr));
    TEST(manager.isSliceLegal(slicePtr));

    // solved, but illegal
    slice[0] = 0b000000001;
    TEST(!manager.isSliceSolved(slicePtr));
    TEST(!manager.isSliceLegal(slicePtr));
}

void test_SolverInterface_isBoardSolved_solvedLegal() {
     std::string input =
        "491786325"
        "735294816"
        "826531479"
        "368172594"
        "142359687"
        "579468132"
        "954823761"
        "287615943"
        "613947258";
    Parser *parser = new Parser(input);
    int parseResult = parser->parse();
    mask *board = parser->getBoard();

    TEST(0 == parseResult);
    BoardManager manager;
    TEST(manager.isBoardSolved(board));
    TEST(manager.isBoardLegal(board));
}

void test_SolverInterface_isBoardSolved_notSolvedLegal() {
     std::string input =
        "..37..4.6"
        "...3.5..."
        "92..6.8.."
        ".5...2..4"
        "..1...6.."
        "4..98..7."
        ".3..71..8"
        "..4...7.2"
        "..6...1.3";
    Parser *parser = new Parser(input);
    int parseResult = parser->parse();
    mask *board = parser->getBoard();

    TEST(0 == parseResult);
    BoardManager manager;
    TEST(!manager.isBoardSolved(board));
    TEST(manager.isBoardLegal(board));
}

void test_SolverInterface_isBoardSolved_solvedIllegal() {
     std::string input =
        "999999999"
        "735294816"
        "826531479"
        "368172594"
        "142359687"
        "579468132"
        "954823761"
        "287615943"
        "613947258";
    Parser *parser = new Parser(input);
    int parseResult = parser->parse();
    mask *board = parser->getBoard();

    TEST(0 == parseResult);
    BoardManager manager;
    TEST(!manager.isBoardSolved(board));
    TEST(!manager.isBoardLegal(board));
}

void test_SolverInterface_isBoardSolved_notSolvedIllegal() {
     std::string input =
        "..36..4.6"
        "...3.5..."
        "92..6.8.."
        ".5...2..4"
        "..1...6.."
        "4..98..7."
        ".3..71..8"
        "..4...7.2"
        "..6...1.3";
    Parser *parser = new Parser(input);
    int parseResult = parser->parse();
    mask *board = parser->getBoard();

    TEST(0 == parseResult);
    BoardManager manager;
    TEST(!manager.isBoardSolved(board));
    TEST(!manager.isBoardLegal(board));
}
void test_BoardManager_isInsideList() {
    mask **slice = getTestSlice();
    mask *element = slice[3];
    BoardManager manager;
    TEST(manager.isInsideList(element, slice, N));
    element = new mask;
    TEST(!manager.isInsideList(element, slice, N));

    slice = getTestSlice();
    mask **sliceOther = getTestSlice();
    element = slice[3];
    TEST(!manager.isInsideList(element, sliceOther, N));
}

void test_OverlapSolver_getListOfOverlaps_noOverlap() {
    mask **slice = getTestSlice();
    mask **sliceTarget = getTestSlice();

    // candidate "2" appears in three potential positions of _slice_
    // let's assume no overlap between the two slices.
    int candidate = 2;

    mask **listOfOverlaps = getTestSlice();  // initialize with any value
    OverlapSolver solver;
    int length = solver.getListOfOverlaps(candidate, listOfOverlaps,
            slice, sliceTarget);
    TEST(0 == length);
}

void test_OverlapSolver_getListOfOverlaps_noCandidateInOverlap() {
    mask **slice = getTestSlice();
    mask **sliceTarget = getTestSlice();

    // candidate "2" appears in three potential positions of _slice_
    // let's assume overlap between the two slices, however, in other positions
    int candidate = 2;
    sliceTarget[0] = slice[2];
    sliceTarget[1] = slice[3];
    sliceTarget[2] = slice[4];

    mask **listOfOverlaps = getTestSlice();  // initialize with any value
    OverlapSolver solver;
    int length = solver.getListOfOverlaps(candidate, listOfOverlaps,
            slice, sliceTarget);
    TEST(0 == length);
}
void test_OverlapSolver_getListOfOverlaps_threeOverlapWithCandidate() {
    mask **slice = getTestSlice();
    mask **sliceTarget = getTestSlice();  // initialize with any value

    // candidate "2" appears in three potential positions of _slice_
    // let's assume these three positions also overlap with _sliceTarget_
    int candidate = 2;
    sliceTarget[0] = slice[1];
    sliceTarget[1] = slice[5];
    sliceTarget[2] = slice[8];

    mask **listOfOverlaps = getTestSlice();  // initialize with any value
    OverlapSolver solver;
    int length = solver.getListOfOverlaps(candidate, listOfOverlaps,
            slice, sliceTarget);
    TEST(3 == length);
}

void test_OverlapSolver_eliminate_exceptNothing() {
    mask **slice = getTestSlice();

    // have we got the slice correctly?
    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000011);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000001);
    TEST(*slice[5] == 0b111110111);
    TEST(*slice[6] == 0b100110001);
    TEST(*slice[7] == 0b100010001);
    TEST(*slice[8] == 0b111110011);

    OverlapSolver solver;
    mask **sliceOther = new mask*[N];
    solver.eliminate(slice, 1, sliceOther, 0);

    TEST(*slice[0] == 0b000000000);
    TEST(*slice[1] == 0b000000010);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000000);
    TEST(*slice[5] == 0b111110110);
    TEST(*slice[6] == 0b100110000);
    TEST(*slice[7] == 0b100010000);
    TEST(*slice[8] == 0b111110010);
}

void test_OverlapSolver_eliminate_exceptOne() {
    mask **slice = getTestSlice();
    OverlapSolver solver;
    mask **sliceOther = new mask*[N];
    sliceOther[0] = slice[3];
    solver.eliminate(slice, 9, sliceOther, 1);

    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000011);
    TEST(*slice[2] == 0b000000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b000000001);
    TEST(*slice[5] == 0b011110111);
    TEST(*slice[6] == 0b000110001);
    TEST(*slice[7] == 0b000010001);
    TEST(*slice[8] == 0b011110011);
}

void test_OverlapSolver_eliminate_exceptAll() {
    mask **slice = getTestSlice();
    OverlapSolver solver;
    mask **sliceOther = new mask*[N];
    BoardManager manager;
    manager.copySlice(sliceOther, slice);
    solver.eliminate(slice, 1, sliceOther, N);

    TEST(*slice[0] == 0b000000001);
    TEST(*slice[1] == 0b000000011);
    TEST(*slice[2] == 0b100000100);
    TEST(*slice[3] == 0b100000000);
    TEST(*slice[4] == 0b100000001);
    TEST(*slice[5] == 0b111110111);
    TEST(*slice[6] == 0b100110001);
    TEST(*slice[7] == 0b100010001);
    TEST(*slice[8] == 0b111110011);
}
