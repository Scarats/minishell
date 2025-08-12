minishell

# Guideline

## Return values

Functions should return integers, 1 for error and 0 for success.
They should receive pointers.
It's not mandatory, just simplify everything imo.

src/execution/execution.c src/execution/pipes.c \
 src/execution/and_and.c src/execution/or_or.c src/execution/exec_cmd.c \

# TODO

1. Tokenize
   - When
2. Find the highest operator of the tokens
3. Recursively call this function each time there is no more sub operators.
