# minishell

## Testing

You can test the minishell with this inputs:

test 1:
```
touch file1 && touch file2
```
test 2:
```
rm file3 || rm file1
```
or do this as test 2:
```
rm file3 || (rm file1 && rm file2)
```

test 3:
```
echo "Hello mgl" | cat > output
```

## Return values

Functions should return integers, 1 for error and 0 for success.
They should receive pointers.
It's not mandatory, just simplify everything imo.

# ToDo

- [ ] See exit_status / code.
- [ ] Add the function swap_list(from, to) to optimize memory usage at parsing.
- [ ] Execution.
- [ ] Error handling system.
- [ ] Heredoc.
- [ ] Signals.
- [ ] Readline & continuous input system (it's currently a AI generated one for testing)
- [x] Create subshell.
