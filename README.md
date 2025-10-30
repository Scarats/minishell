# minishell

## Testing
```
valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp --trace-children=yes --track-fds=yes ./minishell
valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all ./minishell
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell
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
# ToDo

**Thomas**

- [ ] norminette.
- [ ] if / in command, return bash: `/src`: No such file or directory instead of `command not found`.
- [ ] `$?`
	- [ ] should not be modifiable.
- [ ] Check access permissions for builtins.
**Albert**

- [ ] fds not closed after exit(some given exit code).
- [ ] Heredoc.
