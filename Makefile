CXX=g++
RM=rm -f
CFLAGS = -Wall

OUTFILE=sudoku
OUTFILE_TEST=test/test_sudoku
SRCS=parser.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all : $(OUTFILE)

$(OUTFILE) : parser.o main.cpp
	$(CXX) $(CFLAGS) main.cpp -o $(OUTFILE) $(OBJS)

parser.o : parser.cpp parser.hpp

clean:
	$(RM) *.o *.d

$(OUTFILE_TEST) : parser.o test/test_sudoku.o
	g++ -Wall test/test_sudoku.cpp -o ${OUTFILE_TEST} parser.o

test/test_sudoku.o : test/test_sudoku.cpp

test : $(OUTFILE_TEST)
	./$(OUTFILE_TEST)

run: $(OUTFILE)
	cat in-2.txt | ./$(OUTFILE)

