# Guideline

## Testing

You can test the minishell with this inputs:

```
touch file1 && touch file2
```

```
rm file3 || rm file1
```

```
echo "Hello mgl" | cat > output
```

## Return values

Functions should return integers, 1 for error and 0 for success.
They should receive pointers.
It's not mandatory, just simplify everything imo.

# TODO

- [ ] Add the function swap_list(from, to) to optimize memory usage at parsing.
- [ ] Execution.
- [ ] Error handling system.
- [ ] Heredoc.
- [ ] Readline & continuous input system (it's currently a AI generated one for testing)
- [ ] See for subshell creation.
