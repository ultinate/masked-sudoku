CXX=g++
RM=rm -f
CFLAGS = -Wall

OUTFILE=sudoku
OUTFILE_TEST=test/test_sudoku
SRCS=parser.cpp slicer.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all : $(OUTFILE)

$(OUTFILE) : main.cpp $(OBJS)
	$(CXX) $(CFLAGS) main.cpp -o $(OUTFILE) $(OBJS)

clean:
	$(RM) *.o *.d

parser.o : parser.cpp parser.hpp

slicer.o : slicer.cpp slicer.hpp

$(OUTFILE_TEST) : parser.o test/test_sudoku.o
	g++ -Wall test/test_sudoku.cpp -o ${OUTFILE_TEST} parser.o

test/test_sudoku.o : test/test_sudoku.cpp

test : $(OUTFILE_TEST)
	./$(OUTFILE_TEST)

run: $(OUTFILE)
	cat in-2.txt | ./$(OUTFILE)

