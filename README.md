# Sudoku-Solver

This program implements the sudoku solving algorithms with some heuristics that make the solving process to take less than a **microsecond**.
Firstly, rule based solution is implied in exactly the same manner as humans solves sudoku puzzles but much faster. If still some cells are 
left we need to guess. It is not brute force approach, its **recursive backtracking** as it is doing it only for the possible ones.
Also guessing starts with the cell having `least no. of possibilities`. This plays a huge role in reducing the solving time.
Moreover, after each guess, the algorithm again looks for the rule based solution. As a result, my implementation requires only `5-6 
guesses max for a hard Sudoku problem`. The program also tells if the no solutions are possible for a given problem and if multiple
solutions are possible. If multiple solutions, it can print all the solutions.
