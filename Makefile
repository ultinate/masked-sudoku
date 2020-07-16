CXX=g++
RM=rm -f
CFLAGS = -Wall -g

OUTFILE=sudoku.out
OUTFILE_TEST=test/test_sudoku.out
SRCS=parser.cpp slicer.cpp solver.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all : $(OUTFILE) test check

$(OUTFILE) : main.cpp $(OBJS) solver.hpp
	$(CXX) $(CFLAGS) main.cpp -o $(OUTFILE) $(OBJS)

clean:
	$(RM) *.o *.d

parser.o : parser.cpp parser.hpp

slicer.o : slicer.cpp slicer.hpp

solver.o : solver.cpp solver.hpp

$(OUTFILE_TEST) : $(OBJS) test/test_all.o test/test_parser.hpp test/test_slicer.hpp test/test_solver.hpp
	g++ $(CFLAGS) test/test_all.cpp -o $(OUTFILE_TEST) $(OBJS)

test/test_sudoku.o : test/test_sudoku.cpp

test : $(OUTFILE_TEST)
	./$(OUTFILE_TEST)

run: $(OUTFILE)
	cat in-1.txt | ./$(OUTFILE)

check : 
	cppcheck .
