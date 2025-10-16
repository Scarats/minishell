# minishell

## Testing
```
valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all ./minishell
```

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

test 4:

```
(((echo start && (printf "inside\n" > tmp1.txt && cat tmp1.txt)) | tr a-z A-Z) || ((grep root /etc/passwd || echo no_root) && (echo fallback > tmp2.txt))) && ((cat tmp1.txt tmp2.txt | wc -l > lines.txt) || echo count_failed)
```

to test cd:

```
ls && (cd src && ls)
```

## Return values

Functions should return integers, 1 for error and 0 for success.
They should receive pointers.
It's not mandatory, just simplify everything imo.

# ToDo

**Thomas**
Matrix:

- [ ] handle enclosed quotes : echo "'lol'".
- [ ] if / in command, return bash: `/src`: No such file or directory instead of `command not found`.
- [ ] `$?`
	- [ ] should not be in the env.
	- [ ] should be set to 0 at the beginning.
	- [ ] should not be modifiable.
- [ ] env_var should only contain chars.
- [ ] exit codes.
- [ ] pass env to execve.
- [ ] use execve for builtins.
- [ ] Builtins.
  - [ ] exit
  - [x] unset
  - [x] env
  - [x] echo
  - [x] export
  - [x] pwd
  - [x] cd
    - [ ] Check access permissions for builtins.
- [ ] Signals.
- [ ] See exit_status / code.
- [ ] $ in quotes "
- [x] Append >> 
- [x] $? exit status.
- [x] Handle PATH (not the current hardcoded version).
- [x] Make find bin look at the current dir as well.
- [x] Check output when command doesn't exist. (just input `lol` or `$donotexist`).
- [x] Execution.
- [x] Shells variables, 1 list, bool exported, non exported var are erased at the end of the process. Exported are kept.
- [x] Error handling system.
- [x] Create subshell. 

**Albert**

- [ ] Heredoc.
- [ ] Readline & continuous input system (it's currently a AI generated one for testing)
