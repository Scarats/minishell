SRCS = src/minishell.c \
	   src/parsing/parse_line.c src/parsing/parse_utils.c \
	   src/parsing/tree.c src/parsing/tree_utils.c \
	   src/execution/and_and.c src/execution/exec_cmd.c \
	   src/execution/execution.c src/execution/or_or.c \
	   src/execution/pipes.c src/execution/exec_builtins.c \
	   src/builtins/cd.c src/builtins/pwd.c src/builtins/echo.c \
	   src/builtins/exit.c src/builtins/matrix.c src/env_var/get_env_var.c \
	   src/builtins/export.c src/builtins/env.c src/builtins/unset.c \
	   src/handlers/set_last_exit_status.c src/handlers/error_handler.c \
	   src/env_var/copy_env.c src/env_var/find_bin.c src/env_var/find_tenv_var.c \
	   src/handlers/redirections.c

FLAGS = -Wall -Wextra -Werror -fPIE
CC    = cc
RM    = rm -f
OBJ_DIR = dot_o
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
NAME = minishell

# Remove the CPPGLAGS
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(CPPFLAGS) -c $< -o $@

# Remove the LDFLAGS
$(NAME): $(OBJS)
	@make -C ./utils/libft
	$(CC) $(OBJS) $(LDFLAGS) -Lutils/libft -lft -lreadline -lhistory -o $(NAME)

all: $(NAME)

clean:	
	@make fclean -C ./utils/libft
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: re

.PHONY: all bonus clean fclean re