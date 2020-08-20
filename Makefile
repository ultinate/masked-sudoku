CXX=g++
RM=rm -f
CFLAGS = -Wall -g --coverage -p

OUTFILE=sudoku
OUTFILE_TEST=test/test_sudoku
SRCS=parser.cpp slicer.cpp solver.cpp board.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
TEST_SRCS=test/test_parser.hpp test/test_slicer.hpp test/test_solver.hpp

all : $(OUTFILE) test check

$(OUTFILE) : main.cpp $(OBJS)
	$(CXX) $(CFLAGS) main.cpp -o $(OUTFILE) $(OBJS)

clean:
	$(RM) $(OUTFILE) $(OUTFILE_TEST)
	$(RM) *.o *.d
	$(RM) test/*.o test/*.d
	$(RM) *.gcno *.gcda

board.o : board.cpp board.hpp

parser.o : parser.cpp parser.hpp

slicer.o : slicer.cpp slicer.hpp

solver.o : solver.cpp solver.hpp

$(OUTFILE_TEST) : test/test_all.cpp $(TEST_SRCS) $(OBJS)
	$(CXX) $(CFLAGS) test/test_all.cpp -o $(OUTFILE_TEST) $(OBJS)

test : $(OUTFILE_TEST)
	$(OUTFILE_TEST)

run: $(OUTFILE)
	cat in/in-hr.txt | ./$(OUTFILE)

check : 
	cppcheck .

coverage :
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out

profile :
	gprof sudoku gmon.out | less
