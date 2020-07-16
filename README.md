# Simple Sudoku Solver in C++

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

# Method

  - Slice board into 29 different slices (horizontal, vertical,
    diagonal, small squares).
  - Run different solvers on all slices as long as something changes.
  - DetermineSolver: if a number only fits into one field of a slice,
    fix this number in that field.
  - EliminateSolver: if a number is fixed in a slice, eliminate the
    possibility of it occuring in all other fields of this slice.
  - GuessSolver (not implemented yet): If stuck, guess a single number
    in a slice and continue with the other two solvers until solved.
    If this gives a solved (and valid) board, we're done. Otherwise,
    revert the guess and guess differently.

