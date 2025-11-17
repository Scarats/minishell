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

- [X] echo << ho$la$"$a"$$"b"
- [X] echo cat << hola > bonjour /* and CTRL+C */
- [X] tcardair@c2r7s1:minishell> echo << ho$la$"$a"$$"b"
		tcardair@c2r7s1:minishell> exit
		[1]    81087 segmentation fault (core dumped)  ./minishell
- [ ] free() remove them
- [X] exit code
- [X] exit invalid 
- [ ] bash-5.1$ exit djbndh
		exit
		bash: exit: djbndh: numeric argument required

		╭─  │  ~/goinfre/minikirk │   master !3 ?1 ▓▒░································░▒▓ 2 ✘ │ 16:37:26  
		╰─ echo $?
		2
- [X] forbidden function read_history
- [ ] rl_clear_history
- [X] sleep 4 | ls /* ctr + C = wrong error code */
	- [X] ctrl + c in exec not working.
- [ ] return code in pipes in the wrong order.