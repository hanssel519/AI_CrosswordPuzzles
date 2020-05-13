# crossword puzzle
### Rules
1. Any words from a given word list can be used in the solution as long as the constraints are satisfied.
2. A puzzle is specified in the format of (start_x, start_y, length, direction) for each word.
3. the sample input puzzle would be like
  025A 115D 307D 145A 364A 543D
4. The origin (0,0) is at the top-left corner. For directions, "A" means "across" and "D" means "down"
5. To start, each word in the puzzle is a variable, and the provided word list is the initial domain.
### Hint
Consider the length of each word to be its unary constraint.
Each block where two words intersect gives a binary constraint.
