SRCS = minishell.c src/execution/execution.c  src/execution/pipes.c \
		src/execution/and_and.c src/execution/or_or.c src/execution/exec_cmd.c \

FLAGS = -Wall -Wextra -Werror 
CC    = cc
RM    = rm -f
OBJ_DIR = dot_o
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
NAME = minishell

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@make -C ./utils/libft
	$(CC) $(OBJS) -Lutils/libft -lft -o $(NAME)

all: $(NAME)

clean:	
	@make fclean -C ./utils/libft
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: re

.PHONY: all bonus clean fclean re