CXX=g++
RM=rm -f
CFLAGS = -Wall -g

OUTFILE=sudoku.out
OUTFILE_TEST=test/test_sudoku.out
SRCS=parser.cpp slicer.cpp solver.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
TEST_SRCS=test/test_parser.hpp test/test_slicer.hpp test/test_solver.hpp

all : $(OUTFILE) test check

$(OUTFILE) : main.cpp $(OBJS)
	$(CXX) $(CFLAGS) main.cpp -o $(OUTFILE) $(OBJS)

clean:
	$(RM) *.o *.d
	$(RM) test/*.o test/*.d
	$(RM) *.gcno *.gcda

parser.o : parser.cpp parser.hpp

slicer.o : slicer.cpp slicer.hpp

solver.o : solver.cpp solver.hpp

$(OUTFILE_TEST) : test/test_all.cpp $(TEST_SRCS) $(OBJS)
	$(CXX) $(CFLAGS) test/test_all.cpp -o $(OUTFILE_TEST) $(OBJS)

test : $(OUTFILE_TEST)
	$(OUTFILE_TEST)

run: $(OUTFILE)
	cat in-3.txt | ./$(OUTFILE)

check : 
	cppcheck .

coverage :
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out
