# Guideline

## Testing

You can test the minishell with this input:

```
(echo "build" | tee log.txt && grep build log.txt) || (echo "fallback" | tee log.txt && cat log.txt) && (rm -f log.txt || echo "could not remove")
```

## Return values

Functions should return integers, 1 for error and 0 for success.
They should receive pointers.
It's not mandatory, just simplify everything imo.

# TODO

- [ ] Execution.
- [ ] Error handling system.
- [ ] Heredoc.
- [ ] Readline & continuous input system (it's currently a AI generated one for testing)
- [ ] See for subshell creation.
