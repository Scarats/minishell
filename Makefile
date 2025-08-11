SRCS = minishell.c \
	   src/parsing/parse_line.c \
	   src/parsing/parse_utils.c

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