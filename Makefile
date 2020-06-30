OUTFILE=sudoku.out

build:
	g++ -Wall main.cpp -o ${OUTFILE}

test:
	./${OUTFILE} test

run:
	./${OUTFILE}

