# Simple Sudoku Solver in C++

## Motivation

I wanted a playground exercise to practice working with pointers
and bit masks. Solving (all) sudokus also eliminates the need to actually
solving them by hand.


## Getting Started

Use `make` to build and `make run` to run an example.


Example input sudoku:

```
..37..4.6
...3.5...
92..6.8..
.5...2..4
..1...6..
4..98..7.
.3..71..8
..4...7.2
..6...1.3
```

## Unit Tests

Use `make test` to run unit tests.

# Basis

A sudoku must follow these rules:

  - A square board consists of 9 x 9 positions. When solved, each position
    is filled with an integer of the interval [1, 9].
  - In each horizontal row, vertical column and selected 3 x 3 sub-squares,
    each number must appear exactly once.
  - Some sudokus also implement a further rule: In both main diagonals,
    each number must appear exactly once.


# Method

  - Slice board into 27 different slices (horizontal, vertical, sub-squares
    "boxes"). We do not consider the two diagonal slices.
  - Run different solvers on all slices as long as something changes.
  - **DetermineSolver**: If a number only fits into one field of a slice,
    fix this number in that field.
  - **EliminateSolver**: If a number is fixed in a slice, eliminate the
    possibility of it occuring in all other fields of this slice.
  - **GuessSolver** (not implemented yet): If stuck, guess a single number
    in a slice and continue with the other two solvers until solved.
    If this gives a solved (and valid) board, we're done. Otherwise,
    revert the guess and guess differently.

